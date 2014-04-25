/**
 * Copyright (c) 2011-2014 Microsoft Mobile.
 */

#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QDesktopWidget>
#include <QDir>
#include <QLibraryInfo>
#include <QMessageBox>
#include <QPluginLoader>

#include "gameengine.h"
#include "../plugins/levelplugininterface.h"


/*!
  \class MainWindow
  \brief The main window of the application.
*/


/*!
  Constructor.
*/
MainWindow::MainWindow(QWidget *parent)
    : QDeclarativeView(parent),
      m_gameEngine(0),
      m_levelPlugin(0),
      m_eventFilter(0)
{
#ifdef Q_WS_MAEMO_5
    window()->setAttribute(Qt::WA_Maemo5PortraitOrientation, true);
#endif

    // Game engine
    m_gameEngine = new GameEngine(this);

    // Set game engine visible to QML
    rootContext()->setContextProperty("GameEngine", m_gameEngine);
    rootContext()->setContextProperty("LevelPlugin", 0);

    // In-App Purchase
#ifdef IA_PURCHASE
    rootContext()->setContextProperty("IAP", 1);
#else
    rootContext()->setContextProperty("IAP", 0);
#endif

    // Load all levels plugins
    loadLevelPlugins();
    
    // QML main window
    engine()->addImportPath("./imports");
    setResizeMode(QDeclarativeView::SizeRootObjectToView);

    // Set QML source
    setSource(QUrl("qrc:/Game.qml"));

    // Store QML root object for game engine
    QObject *rootObj = static_cast<QObject*>(rootObject());
    m_gameEngine->setGameQml(rootObj);
    m_gameEngine->findQmlObjects();

    // Application foreground / background event filter for filterin incoming
    // call (window) when game will be paused
    m_eventFilter = new MyEventFilter(this);
    connect(m_eventFilter, SIGNAL(activationChangeFiltered()),
            this, SLOT(handleActivationChanged()));
    qApp->installEventFilter(m_eventFilter);

    // Remove context menu from the all widgets
    QWidgetList widgets = QApplication::allWidgets();
    QWidget *widget(0);

    foreach (widget, widgets) {
        widget->setContextMenuPolicy(Qt::NoContextMenu);
    }
}


/*!
  Destructor.
*/
MainWindow::~MainWindow()
{
    deletePlugins();
}


/*!
  Sets the level according to \a index.
*/
void MainWindow::levelActivated(int index)
{
    // Set level for the game engine
    createPlugin(index);
    rootContext()->setContextProperty("LevelPlugin", m_levelPlugin);
    m_gameEngine->setGameLevel(m_levelPlugin);
}


/*!
  Deletes the current plugins and reloads all plugins.
*/
void MainWindow::reloadPlugins()
{
    deletePlugins();
    loadLevelPlugins();
}


/*!
  Handles the activation changed event.
*/
void MainWindow::handleActivationChanged()
{
    m_gameEngine->pauseGame();
    m_gameEngine->foreground();
}


/*!
  Deletes the plugins.
*/
void MainWindow::deletePlugins()
{
    for (int i = 0; i < m_plugins.count(); i++) {
        m_plugins[i]->unload();
        m_plugins[i] = 0;
    }

    m_plugins.clear();
    m_gameEngine->clearPluginList();
}


/*!
  Loads the level plugins.
*/
void MainWindow::loadLevelPlugins()
{
#if defined(Q_OS_SYMBIAN)
    bool existsC = loadPlugins("c", "quickhitlevels");
    bool existsE = loadPlugins("e", "quickhitlevels");
    bool existsF = loadPlugins("f", "quickhitlevels");

    Q_UNUSED(existsC);
    Q_UNUSED(existsE);
    Q_UNUSED(existsF);
//    if (existsC || existsE || existsF) {
//        createPlugin();
//    }
#else
    if (loadPlugins("c", "quickhitlevels")) {
//        createPlugin();
    }
#endif
}


/*!
  Loads all plugins from \a pluginDir in \a drive. Returns true if plugin(s)
  were loaded, false otherwise.
*/
bool MainWindow::loadPlugins(QString drive, QString pluginDir)
{
#if defined (Q_OS_SYMBIAN)
    QDir pluginsDir(drive + ":" +
                    QLibraryInfo::location(QLibraryInfo::PluginsPath));
#elif defined (Q_OS_WIN32) || defined(Q_WS_SIMULATOR)
    Q_UNUSED(drive);
    QDir pluginsDir = QDir::currentPath();
#else
    Q_UNUSED(drive);
    QDir pluginsDir(QLibraryInfo::location(QLibraryInfo::PluginsPath));
#endif

    pluginsDir.cd(pluginDir);

    qDebug() << "MainWindow::loadPlugins(): Will load plugins from:"
             << pluginsDir.path();

    bool newPluginsLoaded(false);

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        // Accept only plugin files
#if defined (Q_OS_SYMBIAN)
        if (fileName.contains(".qtplugin", Qt::CaseInsensitive)) {
#elif defined (Q_WS_MAEMO_5) || defined (Q_WS_MAEMO_6)
        if (fileName.contains(".so", Qt::CaseInsensitive)) {
#else
        if (fileName.contains(".dll", Qt::CaseInsensitive)) {
#endif
            // Create plugin loader
            QPluginLoader *pluginLoader =
                    new QPluginLoader(pluginsDir.absoluteFilePath(fileName));

            // Load plugin
            bool ret = pluginLoader->load();

            if (!ret) {
                // Loading failed
                qDebug() << "MainWindow::loadPlugins(): Could not load plugin"
                         << fileName << "!";
            }
            else {
                // Loading done
                // Test creating plugin
                QObject *plugin(0);
                LevelPluginInterface *pluginInterface(0);
                plugin = pluginLoader->instance();
                pluginInterface = qobject_cast<LevelPluginInterface*>(plugin);

                if (pluginInterface) {
                    qDebug() << "MainWindow::loadPlugins(): Plugin can be created;"
                             << "IAP ID:" << pluginInterface->iapId() << ";"
                             << "Display name:" << pluginInterface->displayname();

                    m_gameEngine->addPlugin(pluginInterface);

                    // Store loader to array
                    m_plugins.append(pluginLoader);
                    newPluginsLoaded = true;
                }
                else {
                    pluginLoader->unload();
                    qDebug() << "MainWindow::loadPlugins(): Plugin can NOT be created!";
                }
            }
        }
    } // foreach

    return newPluginsLoaded;
}


/*!
  Tries to create a plugin with \a index.
*/
void MainWindow::createPlugin(int index /* = -1 */)
{
    if (index == -1) {
        return;
    }

    m_levelPlugin = 0;

    if (m_plugins.count() - 1 < index) {
        return;
    }

    // Try to create a plugin instance
    QPluginLoader *pluginLoader = m_plugins[index];
    QObject *plugin = pluginLoader->instance();

    if (plugin) {
        // Plugin instance created
        // Cast plugin to LevelPluginInterface, that is common for all plugins
        LevelPluginInterface *pluginInterface =
                qobject_cast<LevelPluginInterface*>(plugin);
        m_levelPlugin = pluginInterface;

        qDebug() << "MainWindow::createPlugin(): Plugin with index"
                 << index << "created.";
    }
    else {
        qDebug() << "MainWindow::createPlugin(): Failed to create plugin!";
        QMessageBox::information(this, "QuickHit", "Could not create QuickHit levels!");
    }
}

