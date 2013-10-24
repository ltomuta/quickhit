/**
 * Copyright (c) 2011 Nokia Corporation.
 */

#ifndef LEVELTWO_H
#define LEVELTWO_H

#include <QImageReader>
#include "levelplugininterface.h"


class LevelTwo : public LevelPluginInterface
{
    Q_OBJECT

    // This macro tells Qt which interfaces the class implements.
    // This is used when implementing plugins.
    Q_INTERFACES(LevelPluginInterface)

public:
    LevelTwo();
    ~LevelTwo();

public:
    static QString pathPrefix();

public: // From LevelPluginInterface
    QStringList levelSounds();

    Q_INVOKABLE QVariant getData(QVariant key);
    Q_INVOKABLE QVariant graphSize(QVariant pathToGraph);

    Q_INVOKABLE QVariant enemyCount() { return QVariant(35); }
    Q_INVOKABLE QVariant enemyRowCount() { return QVariant(0); }
    Q_INVOKABLE QVariant enemySpeed() { return QVariant(20000); }
    Q_INVOKABLE QVariant enemyFireSpeed() { return QVariant(500); }

    // Root paths for the QML, Pictures and Sounds
    Q_INVOKABLE QVariant qmlRootPath() { return QVariant(pathPrefix() + "quickhitdata/leveltwo/"); }
    Q_INVOKABLE QVariant pictureRootPath() { return QVariant(pathPrefix() + "quickhitdata/leveltwo/gfx/"); }

    // Full paths
    Q_INVOKABLE QVariant pathToTransparentEnemyPic() { return QVariant(pathPrefix() + "quickhitdata/leveltwo/gfx/transparent.png"); }
    Q_INVOKABLE QVariant pathToMissilePic() { return QVariant(pathPrefix() + "quickhitdata/leveltwo/gfx/missile2.png"); }
    Q_INVOKABLE QVariant pathToEnemyMissilePic() { return QVariant(pathPrefix() + "quickhitdata/leveltwo/gfx/enemy_missile2.png"); }
    Q_INVOKABLE QVariant pathToMyShipPic() { return QVariant(pathPrefix() + "quickhitdata/leveltwo/gfx/ship.png"); }
    Q_INVOKABLE QVariant pathToLevelThumbnailPic() { return QVariant(pathPrefix() + "quickhitdata/leveltwo/gfx/thumbnail.png"); }
    Q_INVOKABLE QVariant displayname() { return QVariant("Level Two"); }

    Q_INVOKABLE QVariant iapId() { return QVariant(IAP_ID); }

protected: // Data
    QImageReader m_imageReader;
};


#endif // LEVELTWO_H
