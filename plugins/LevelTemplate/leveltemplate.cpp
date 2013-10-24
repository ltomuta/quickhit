/**
 * Copyright (c) 2011 Nokia Corporation.
 */

#include "leveltemplate.h"

#include <QDir>
#include <QtCore/qplugin.h>


/*!
  \class LevelTemplate
  \brief The main class of the plugin.
*/


/*!
  Constructor.
*/
LevelTemplate::LevelTemplate()
{
}


/*!
  Destructor.
*/
LevelTemplate::~LevelTemplate()
{
}


/*!
  Returns the platform dependent path prefix.
*/
QString LevelTemplate::pathPrefix()
{
    /*
    QDir::rootPath()
    For Unix operating systems this returns "/".
    For Windows file systems this normally returns "c:/".
    On Symbian this typically returns "c:/data",
    */

#if defined (Q_WS_MAEMO_5) || defined (Q_WS_MAEMO_6)
    return "/home/user/.";
#elif defined Q_OS_WIN32
    return QDir::rootPath();
#else
    //return QDir::rootPath() + "/";
    return "c:/System/";
#endif
}


/*!
  Returns the list of (paths to) game audio effects.
  NOTE: This is mandatory (sounds and indexes) for all the levels!
*/
QStringList LevelTemplate::levelSounds()
{
    QStringList list;

    // 0 = Enemy explode
    list.append(LevelTemplate::pathPrefix() + "quickhitdata/"
                + LEVEL_NAME + "/sound/enemy_explosion.wav");

    // 1 = You explode
    list.append(LevelTemplate::pathPrefix() + "quickhitdata/"
                + LEVEL_NAME + "/sound/myship_explosion.wav");

    // 2 = Missile fires
    list.append(LevelTemplate::pathPrefix( ) +"quickhitdata/"
                + LEVEL_NAME + "/sound/laser.wav");

    // 3 = Emeny missile fires
    list.append(LevelTemplate::pathPrefix() + "quickhitdata/"
                + LEVEL_NAME + "/sound/rocket.wav");

    // Additional sounds
    list.append(LevelTemplate::pathPrefix() + "quickhitdata/"
                + LEVEL_NAME + "/sound/enableship.wav");
    list.append(LevelTemplate::pathPrefix() + "quickhitdata/"
                + LEVEL_NAME + "/sound/level2.wav");

    return list;
}


/*!
  Returns the value matching \a key.
*/
QVariant LevelTemplate::getData(QVariant key)
{
    QVariant ret;

    switch (key.toInt()) {
    case 1: {
        // Any data what you need for this key
        // Set your data into QVariant (ret)
        break;
    }
    default: {
        break;
    }
    }
    return ret;
}


/*!
  Returns the size of the image in \a pathToGraph.
*/
QVariant LevelTemplate::graphSize(QVariant pathToGraph)
{
    QVariant ret;
    m_imageReader.setFileName(pathToGraph.toString());
    QSize imageSize = m_imageReader.size();
    ret.setValue(imageSize);
    return ret;
}


// This Qt macro exports the plugin class level with the name levelplugins.
// There should be exactly one occurrence of this macro in a Qt plugin's source code.
Q_EXPORT_PLUGIN2(levelplugins, LevelTemplate);
