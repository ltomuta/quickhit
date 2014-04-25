/**
 * Copyright (c) 2011-2014 Microsoft Mobile.
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
        if (doPause) {
            upToDownAnim.pause();
            rightLeftAnim.pause();
        }
        else {
            upToDownAnim.resume();
            rightLeftAnim.resume();
        }
    }
    
    /**
     * Starts the moving animations.
     */
    function startMovingAnim()
    {
        upToDownAnim.restart();
        rightLeftAnim.restart();
    }

    // Right-left animation
    SequentialAnimation {
        id: rightLeftAnim
        loops: Animation.Infinite
        NumberAnimation { target: enemy; property: "x";
                          to: gameArea.xDim - enemy.width;
                          easing.type: Easing.OutQuad; duration: 3000 }
        NumberAnimation { target: enemy; property: "x"; to: 0;
                          easing.type: Easing.OutQuad; duration: 3000 }
    }

    // Up to down animation
    SequentialAnimation {
        id: upToDownAnim
        PropertyAnimation { target: enemy; property: "y"; to: gameArea.yDim;
                            easing.type: Easing.Linear;
                            duration: enemy.enemySpeed }
        PropertyAction { target: enemy; properties: "opacity"; value: 0 }
    }
}
