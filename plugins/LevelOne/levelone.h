/**
 * Copyright (c) 2011 Nokia Corporation.
 */

#ifndef LEVELONE_H
#define LEVELONE_H

#include <QImageReader>
#include "levelplugininterface.h"


class LevelOne : public LevelPluginInterface
{
    Q_OBJECT

    // This macro tells Qt which interfaces the class implements.
    // This is used when implementing plugins.
    Q_INTERFACES(LevelPluginInterface)

public:
    LevelOne();
    ~LevelOne();

public:
    static QString pathPrefix();

public: // From LevelPluginInterface
    QStringList levelSounds();

    Q_INVOKABLE QVariant getData(QVariant key);
    Q_INVOKABLE QVariant graphSize(QVariant pathToGraph);

    Q_INVOKABLE QVariant enemyCount() { return QVariant(30); } // 6 x 5 = 30
    Q_INVOKABLE QVariant enemyRowCount() { return QVariant(6); } // 6 rows of enemies
    Q_INVOKABLE QVariant enemySpeed() { return QVariant(36000); }
    Q_INVOKABLE QVariant enemyFireSpeed() { return QVariant(200); }

    // Root paths for the QML, Pictures and Sounds
    Q_INVOKABLE QVariant qmlRootPath() { return QVariant(LevelOne::pathPrefix() + "quickhitdata/levelone/"); }
    Q_INVOKABLE QVariant pictureRootPath() { return QVariant(LevelOne::pathPrefix() + "quickhitdata/levelone/gfx/"); }

    // Full paths
    Q_INVOKABLE QVariant pathToTransparentEnemyPic() { return QVariant(LevelOne::pathPrefix() + "quickhitdata/levelone/gfx/transparent.png"); }
    Q_INVOKABLE QVariant pathToMissilePic() { return QVariant(LevelOne::pathPrefix() + "quickhitdata/levelone/gfx/missile2.png"); }
    Q_INVOKABLE QVariant pathToEnemyMissilePic() { return QVariant(LevelOne::pathPrefix() + "quickhitdata/levelone/gfx/enemy_missile2.png"); }
    Q_INVOKABLE QVariant pathToMyShipPic() { return QVariant(LevelOne::pathPrefix() + "quickhitdata/levelone/gfx/ship.png"); }
    Q_INVOKABLE QVariant pathToLevelThumbnailPic() { return QVariant(LevelOne::pathPrefix() + "quickhitdata/levelone/gfx/thumbnail.png"); }
    Q_INVOKABLE QVariant displayname() { return QVariant("Level One"); }

    Q_INVOKABLE QVariant iapId() { return QVariant(IAP_ID); }

protected: // Data
    QImageReader m_imageReader;
};


#endif // LEVELONE_H
