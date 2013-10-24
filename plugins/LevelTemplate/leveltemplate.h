/**
 * Copyright (c) 2011 Nokia Corporation.
 */

#ifndef LEVELTEMPLATE_H
#define LEVELTEMPLATE_H

#include <QImageReader>

// Common interface header for all level plugins
#include "levelplugininterface.h"

// Level name
#define LEVEL_NAME "leveltemplate"


class LevelTemplate : public LevelPluginInterface
{
    Q_OBJECT

    // This macro tells Qt which interfaces the class implements.
    // This is used when implementing plugins.
    Q_INTERFACES(LevelPluginInterface)

public:
    LevelTemplate();
    virtual ~LevelTemplate();

public:
    static QString pathPrefix();

public: // From LevelPluginInterface
    QStringList levelSounds();

    Q_INVOKABLE QVariant getData(QVariant key);
    Q_INVOKABLE QVariant graphSize(QVariant pathToGraph);

    Q_INVOKABLE QVariant enemyCount() { return QVariant(1); }
    Q_INVOKABLE QVariant enemyRowCount() { return QVariant(0); }
    Q_INVOKABLE QVariant enemySpeed() { return QVariant(20000); }
    Q_INVOKABLE QVariant enemyFireSpeed() { return QVariant(2000); }

    // Root paths for the QML, Pictures and Sounds
    Q_INVOKABLE QVariant qmlRootPath() { return QVariant(LevelTemplate::pathPrefix() + "quickhitdata/" + LEVEL_NAME + "/"); }
    Q_INVOKABLE QVariant pictureRootPath() { return QVariant(LevelTemplate::pathPrefix()+"quickhitdata/" + LEVEL_NAME + "/gfx/"); }

    // Full paths
    Q_INVOKABLE QVariant pathToTransparentEnemyPic() { return QVariant(LevelTemplate::pathPrefix() + "quickhitdata/" + LEVEL_NAME + "/gfx/transparent.png"); }
    Q_INVOKABLE QVariant pathToMissilePic() { return QVariant(LevelTemplate::pathPrefix() + "quickhitdata/" + LEVEL_NAME + "/gfx/missile2.png"); }
    Q_INVOKABLE QVariant pathToEnemyMissilePic() { return QVariant(LevelTemplate::pathPrefix() + "quickhitdata/" + LEVEL_NAME + "/gfx/enemy_missile2.png"); }
    Q_INVOKABLE QVariant pathToMyShipPic() { return QVariant(LevelTemplate::pathPrefix() + "quickhitdata/" + LEVEL_NAME + "/gfx/ship.png"); }
    Q_INVOKABLE QVariant pathToLevelThumbnailPic() { return QVariant(LevelTemplate::pathPrefix() + "quickhitdata/" + LEVEL_NAME + "/gfx/thumbnail.png"); }
    Q_INVOKABLE QVariant displayname() { return QVariant("Level Template"); }

    Q_INVOKABLE QVariant iapId() { return QVariant(IAP_ID); }

protected: // Data
    QImageReader m_imageReader;
};


#endif // LEVELTEMPLATE_H
