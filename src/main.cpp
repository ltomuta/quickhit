/**
 * Copyright (c) 2011 Nokia Corporation.
 */

#include <QDesktopWidget>
#include <QtDeclarative>
#include <QtGui/QApplication>
#include "mainwindow.h"

#ifdef Q_OS_SYMBIAN
    #include <eikenv.h>
    #include <eikappui.h>
    #include <aknenv.h>
    #include <aknappui.h>
#endif

#ifdef Q_WS_MAEMO_5
    #include <QGLWidget>
#endif

#ifdef IA_PURCHASE
    #include "iapurchase.h"
#endif



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#ifdef IA_PURCHASE
    qmlRegisterType<IAPurchase>("IAP", 1, 0, "IAPurchase");
#endif

#ifdef Q_OS_SYMBIAN
    // Lock the app to portrait on Symbian.
    CAknAppUi* appUi = dynamic_cast<CAknAppUi*>( CEikonEnv::Static()->AppUi() );

    if ( appUi )
        {
        TRAPD( error, appUi->SetOrientationL( CAknAppUi::EAppUiOrientationPortrait ); );
        }
#endif

    MainWindow window;


#ifdef Q_WS_MAEMO_5
    // Use QGLWidget to get the OpenGL in Maemo 5
    QGLFormat format = QGLFormat::defaultFormat();
    format.setSampleBuffers(false);

    QGLWidget *glWidget = new QGLWidget(format);
    glWidget->setAutoFillBackground(false);
    window.setViewport(glWidget); // Ownership of glWidget is taken
#endif


    window.setAutoFillBackground(false);

    QObject *rootObject = dynamic_cast<QObject*>(window.rootObject());

    // For Qt.exit() from QML
    QObject::connect((QObject*)window.engine(), SIGNAL(quit()),
                     &app, SLOT(quit()));

    // Signals from QML
    QObject::connect(rootObject, SIGNAL(reloadPlugins()),
                     &window, SLOT(reloadPlugins()));
    QObject::connect(rootObject, SIGNAL(levelActivated(int)),
                     &window, SLOT(levelActivated(int)));


#if defined (Q_OS_SYMBIAN) || defined (Q_WS_MAEMO_5) || defined (Q_WS_MAEMO_6)
    window.setGeometry(QApplication::desktop()->screenGeometry());
    window.showFullScreen();
#elif defined(Q_WS_SIMULATOR)
    window.showFullScreen();
#else
    window.setGeometry(100, 100, 360, 640);
    window.setFixedSize(360, 640);
    window.show();
#endif

    return app.exec();
}
