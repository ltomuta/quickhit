/**
 * Copyright (c) 2011 Nokia Corporation.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDeclarativeView>
#include "myeventfilter.h"

// Forward declarations
class GameEngine;
class LevelPluginInterface;
class QPluginLoader;


class MainWindow : public QDeclarativeView
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void levelActivated(int);
    void reloadPlugins();
    void handleActivationChanged();

private:
    void loadLevelPlugins();
    bool loadPlugins(QString drive, QString pluginDir);
    void createPlugin(int index = -1);
    void deletePlugins();

private:
    GameEngine *m_gameEngine;
    LevelPluginInterface *m_levelPlugin;
    MyEventFilter *m_eventFilter;
    QList<QPluginLoader*> m_plugins;
};

#endif // MAINWINDOW_H
