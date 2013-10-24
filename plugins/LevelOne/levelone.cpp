/**
 * Copyright (c) 2011 Nokia Corporation.
 */

#include "levelone.h"

#include <QDir>
#include <QtCore/qplugin.h>


/*!
  \class LevelOne
  \brief The main class of the plugin.
*/


/*!
  Constructor.
*/
LevelOne::LevelOne()
{
}


/*!
  Destructor.
*/
LevelOne::~LevelOne()
{
}


/*!
  Returns the platform dependent path prefix.
*/
QString LevelOne::pathPrefix()
{
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
  \see levelplugininterface.h
*/
QStringList LevelOne::levelSounds()
{
    QStringList list;

    // 0 = Enemy explode
    list.append(LevelOne::pathPrefix()
                + "quickhitdata/levelone/sound/rocket_explosion.wav");

    // 1 = You explode
    list.append(LevelOne::pathPrefix()
                + "quickhitdata/levelone/sound/crash.wav");

    // 2 = Missile fires
    list.append(LevelOne::pathPrefix()
                + "quickhitdata/levelone/sound/rocket_explosion.wav");

    // 3 = Emeny missile fires
    list.append(LevelOne::pathPrefix()
                + "quickhitdata/levelone/sound/rocket.wav");

    // Additional sounds
    // Level starts
    list.append(LevelOne::pathPrefix()
                + "quickhitdata/levelone/sound/levelonestart.wav");

    return list;
}


/*!
  Returns the value matching \a key.
  \see levelplugininterface.h
*/
QVariant LevelOne::getData(QVariant key)
{
    // No need for implementation in this plugin
    Q_UNUSED(key);
    return QVariant();
}


/*!
  Returns the size of the image in \a pathToGraph.
  \see levelplugininterface.h
*/
QVariant LevelOne::graphSize(QVariant pathToGraph)
{
    QVariant ret;
    m_imageReader.setFileName(pathToGraph.toString());
    QSize imageSize = m_imageReader.size();
    ret.setValue(imageSize);
    return ret;
}


// This Qt macro exports the plugin class LevelOne with the name levelplugins.
// There should be exactly one occurrence of this LevelOne macro in a Qt plugin's source code.
Q_EXPORT_PLUGIN2(levelplugins, LevelOne);
