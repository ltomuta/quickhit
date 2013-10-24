/**
 * Copyright (c) 2011 Nokia Corporation.
 */

#ifndef LEVELPLUGININTERFACE_H
#define LEVELPLUGININTERFACE_H

#include <QObject>
#include <QVariant>


class LevelPluginInterface : public QObject
{
public:
    // Full paths to level sounds
    virtual QStringList levelSounds() = 0;

    // Generic key/value data
    virtual QVariant getData(QVariant key) = 0;

    // Request picture size
    virtual QVariant graphSize(QVariant pathToGraph) = 0;

    // Enemy count in the level
    virtual QVariant enemyCount() = 0;
    // Enemy row count in the level
    virtual QVariant enemyRowCount() = 0;
    // Enemy speed
    virtual QVariant enemySpeed() = 0;
    // Enemy fire speed
    virtual QVariant enemyFireSpeed() = 0;

    // QML root path
    virtual QVariant qmlRootPath() = 0;
    // Picture root path
    virtual QVariant pictureRootPath() = 0;

    // Paths for GameEngine and QuickHit internal QML files
    virtual QVariant pathToTransparentEnemyPic() = 0;
    virtual QVariant pathToMissilePic() = 0;
    virtual QVariant pathToEnemyMissilePic() = 0;
    virtual QVariant pathToMyShipPic() = 0;
    virtual QVariant pathToLevelThumbnailPic() = 0;
    virtual QVariant displayname() = 0;
    virtual QVariant iapId() = 0;
};

// This Qt macro associates the given Identifier "Forum.Nokia.com.QuickHit.LevelPluginInterface/1.0"
// to the interface class called LevelPluginInterface. The Identifier must be unique.
Q_DECLARE_INTERFACE(LevelPluginInterface,
    "Forum.Nokia.com.QuickHit.LevelPluginInterface/1.0");


#endif // LEVELPLUGININTERFACE_H
