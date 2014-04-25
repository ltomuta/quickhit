/**
 * Copyright (c) 2011-2014 Microsoft Mobile.
 */

#include "leveltwo.h"

#include <QDir>
#include <QtCore/qplugin.h>


/*!
  \class LevelTwo
  \brief The main class of the plugin.
*/


/*!
  Constructor.
*/
LevelTwo::LevelTwo()
{
}


/*!
  Destructor.
*/
LevelTwo::~LevelTwo()
{
}


/*!
  Returns the platform dependent path prefix.
*/
QString LevelTwo::pathPrefix()
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
QStringList LevelTwo::levelSounds()
{
    QStringList list;

    // 0 = Enemy explode
    list.append(pathPrefix() + "quickhitdata/leveltwo/sound/enemy_explosion.wav");

    // 1 = You explode
    list.append(pathPrefix() + "quickhitdata/leveltwo/sound/myship_explosion.wav");

    // 2 = Missile fires
    list.append(pathPrefix() + "quickhitdata/leveltwo/sound/laser.wav");

    // 3 = Emeny Missile fires
    list.append(pathPrefix() + "quickhitdata/leveltwo/sound/rocket.wav");

    // Additional sounds
    list.append(pathPrefix() + "quickhitdata/leveltwo/sound/enableship.wav");
    list.append(pathPrefix() + "quickhitdata/leveltwo/sound/level2.wav");

    return list;
}


/*!
  Returns the value matching \a key.
  \see levelplugininterface.h
*/
QVariant LevelTwo::getData(QVariant key)
{
    // No need for implementation in this plugin
    Q_UNUSED(key);
    return QVariant();
}


/*!
  Returns the size of the image in \a pathToGraph.
  \see levelplugininterface.h
*/
QVariant LevelTwo::graphSize(QVariant pathToGraph)
{
    QVariant ret;
    m_imageReader.setFileName(pathToGraph.toString());
    QSize imageSize = m_imageReader.size();
    ret.setValue(imageSize);
    return ret;
}


// This Qt macro exports the plugin class LevelTwo with the name levelplugins.
// There should be exactly one occurrence of this LevelTwo macro in a Qt plugin's source code.
Q_EXPORT_PLUGIN2(levelplugins, LevelTwo);
