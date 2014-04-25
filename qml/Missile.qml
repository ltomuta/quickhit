/**
 * Copyright (c) 2011-2014 Microsoft Mobile.
 */

import QtQuick 1.0

Item {
    id: missile
    objectName: "missile"
    opacity: 0 // Not visible (transparent) by default

    property int fromYpos
    property int toYpos
    property variant myMissileSize
    property bool enemyMissile: false
    property int defaultX: 0
    property int defaultY: 0

    /**
     * Stores the default position of the missile.
     */
    function storeDefaultPos()
    {
        defaultX = missile.x;
        defaultY = missile.y;
    }

    /**
     * Sets the missile to the default position.
     */
    function setToDefaultPos()
    {
        missile.x = defaultX;
        missile.y = defaultY;
        opacity = 0.5;
    }

    /**
     * Sets the graphics of the missile.
     */
    function createGraphicsForLevel()
    {
        if (LevelPlugin) {
            missile.myMissileSize =
                    LevelPlugin.graphSize(LevelPlugin.pathToMissilePic());
            missile.height = myMissileSize.height;
            missile.width = myMissileSize.width;

            if (missile.enemyMissile) {
                missileImage.source =
                        "file:/" + LevelPlugin.pathToEnemyMissilePic();
            }
            else {
                missileImage.source =
                        "file:/" + LevelPlugin.pathToMissilePic();
            }
        }
    }

    /**
     * Fires a player's missile.
     */
    function fire(aXpox, aFromYpos, aToYpos)
    {
        missile.x = aXpox - missile.width / 2;
        missile.y = aFromYpos;
        missile.fromYpos = aFromYpos;
        missile.toYpos = aToYpos;
        missile.opacity = 1;
        GameEngine.playSound(2); // 2 for player's missile sound
        flying.restart();
    }

    /**
     * Fires an enemy missile.
     */
    function enemyFire(aXpox, aFromYpos, aToYpos)
    {
        missile.x = aXpox - missile.width / 2;
        missile.y = aFromYpos;
        missile.fromYpos = aFromYpos;
        missile.toYpos = aToYpos;
        missile.opacity = 1;
        GameEngine.playSound(3); // 3 for enemy missile sound
        flyingEnemy.restart();
    }

    /**
     * Stops the missile.
     */
    function stop()
    {
        flying.stop();
    }

    /**
     * Pauses the missile.
     */
    function pause(doPause)
    {
        if (doPause) {
            flying.pause();
            flyingEnemy.pause();
        }
        else {
            flying.resume();
            flyingEnemy.resume();
        }
    }

    Component.onCompleted: {
        if (!enemyMissile) {
            storeDefaultPos();
            setToDefaultPos();
        }
    }

    // The visual representation of the missile. Initialized by
    // createGraphicsForLevel() function.
    Image {
        id: missileImage
        smooth: true
    }

    // Animates missile flying to the target
    SequentialAnimation {
        id: flying
        PropertyAnimation { target: missile; properties: "y";
                            from: fromYpos; to: toYpos; duration: 2000;
                            easing.type: Easing.InCubic }
        //PropertyAction { target: missile; properties: "opacity"; value: 0 }
        ScriptAction { script: setToDefaultPos() }
    }

    SequentialAnimation {
        id: flyingEnemy
        PropertyAnimation { target: missile; properties: "y";
                            from: fromYpos; to: toYpos; duration: 2000;
                            easing.type: Easing.InCubic }
        PropertyAction { target: missile; properties: "opacity"; value: 0 }
    }
}
