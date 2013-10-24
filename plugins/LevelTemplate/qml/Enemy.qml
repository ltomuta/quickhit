/**
 * Copyright (c) 2011 Nokia Corporation.
 */

import QtQuick 1.0

Image {
    id: enemy

    property int enemySpeed: LevelPlugin.enemySpeed()

    // NOTE: "enemy" as an object name is mandatory for all Enemy.qml files!
    objectName: "enemy"

    smooth: true
    opacity: 0 // 0 = hidden by default
    fillMode: Image.PreserveAspectFit
    source: "file:/" + LevelPlugin.pictureRootPath() + "enemy1.png"
    
    /**
     * Handles pausing/resuming.
     *
     * NOTE: This function must be implemented in all Enemy.qml files!
     *
     * @param doPause If true, will pause, otherwise will resume.
     */
    function pause(doPause)
    {
        // TODO: Handle pausing/resuming here.
    }
}
