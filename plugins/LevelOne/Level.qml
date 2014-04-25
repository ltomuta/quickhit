/**
 * Copyright (c) 2011-2014 Microsoft Mobile.
 */

import QtQuick 1.0
import Qt.labs.particles 1.0

Rectangle {
    id: enemies

    property int enemySpeed: LevelPlugin.enemySpeed()
    property int enemyMaxWidth

    objectName: "level"
    anchors.fill: parent
    color: "black"

    /**
     * Handles pausing/resuming.
     *
     * NOTE: This function must be implemented in all Level.qml files!
     *
     * @param doPause If true, will pause, otherwise will resume.
     */
    function pause(doPause)
    {
        if (doPause) {
            rightLeftAnim.pause()
            upToDownAnim.pause()
        }
        else {
            rightLeftAnim.resume()
            upToDownAnim.resume()
        }
    }

    /**
     * Executes the explosion particle effect at the given coordinates.
     *
     * NOTE: This function must be implemented in all Level.qml files!
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     */
    function explode(x,y)
    {
        explodeParticles.x = x;
        explodeParticles.y = y;
        explodeParticles.burst(20);
        explodeParticles.opacity = 1;
    }

    /**
     * Creates enemies dynamically.
     * @param amount The number of enemies to create.
     */
    function createEnemies(amount)
    {
        var b = false;

        for (var i = 0; i < amount; i++) {
            if (b) {
                Qt.createQmlObject(
                    'import QtQuick 1.0; Image { property bool hit: false; ' +
                    'width: 50; objectName: "enemy"; smooth: true; ' +
                    'fillMode: Image.PreserveAspectFit; ' +
                    'source: "file:/" + LevelPlugin.pictureRootPath() + "enemy1.png"; }',
                    enemiesGrid);
            }
            else {
                Qt.createQmlObject(
                    'import QtQuick 1.0; Image { property bool hit: false; ' +
                    'width: 50; objectName: "enemy"; smooth: true; ' +
                    'fillMode: Image.PreserveAspectFit; ' +
                    'source: "file:/" + LevelPlugin.pictureRootPath() + "enemy2.png"; }',
                    enemiesGrid);
            }

            b = !b;
        }
    }

    /**
     * Calculates the maximum width of an enemy.
     */
    function calEnemyMaxWidth()
    {
        var enemyCountInCol = LevelPlugin.enemyCount()
                              / LevelPlugin.enemyRowCount();
        enemyCountInCol++; // Add some extra space needs for width

        if (enemyCountInCol
                * LevelPlugin.graphSize(
                    LevelPlugin.pictureRootPath() + "enemy1.png").width
                > gameArea.xDim) {
            enemyMaxWidth = gameArea.xDim / (enemyCountInCol + 1);
        }
        else {
            enemyMaxWidth = LevelPlugin.graphSize(
                        LevelPlugin.pictureRootPath() + "enemy1.png").width;
        }
    }

    // Background image for the level
    Image {
        id: background1
        source: "file:/" + LevelPlugin.pictureRootPath() + "background3.png"
        fillMode: Image.PreserveAspectCrop
        smooth: true
        width: parent.width < parent.height ? parent.width : parent.height;
        height: parent.height > parent.width ? parent.height : parent.width;
    }

    Timer {
        id: starBurstTimer
        running: false
        repeat: true
        interval: starParticles.lifeSpan
                  + GameEngine.randInt(100, 2000);

        onTriggered: {
            starParticles.burst(GameEngine.randInt(4, 10));
        }
     }

    // Right-left animation
    SequentialAnimation {
        id: rightLeftAnim
        loops: Animation.Infinite

        NumberAnimation {
            target: enemiesGrid
            property: "x"
            from: 0; to: gameArea.xDim - enemiesGrid.width
            easing.type: Easing.OutQuad
            duration: 3000
        }
        NumberAnimation {
            target: enemiesGrid
            property: "x"
            to: 0
            easing.type: Easing.OutQuad
            duration: 3000
        }
    }

    // Up to down animation
    PropertyAnimation {
        id: upToDownAnim
        target: enemiesGrid
        property: "y"
        to: gameArea.height
        easing.type: Easing.Linear
        duration: enemies.enemySpeed
    }

    Grid {
        id: enemiesGrid
        objectName: "enemiesGrid"
        columns: LevelPlugin.enemyCount() / LevelPlugin.enemyRowCount()
        spacing: 8
    }

    // Explosion particle effect
    // NOTE: This function must be implemented in all Level.qml files!
    Particles {
        id: explodeParticles
        opacity: 0
        width: 1
        height: 1
        emissionRate: 0
        source: "file:/" + LevelPlugin.pictureRootPath() + "red_fire.png"
        lifeSpan: 500
        lifeSpanDeviation: 2000
        count: -1
        angle: 270
        angleDeviation: 65
        velocity: 100
        velocityDeviation: 100

        ParticleMotionGravity {
            yattractor: 1000
            xattractor: 300
            acceleration: 50
        }
    }

    Particles {
        id: starParticles
        y: -50
        x: 0
        width: parent.width < parent.height ? parent.width : parent.height;
        height: 50
        emissionRate: 0
        source: "file:/" + LevelPlugin.pictureRootPath() + "star3.png"
        lifeSpan: 3000
        count: -1
        angle: 90
        velocity: 100

        ParticleMotionGravity {
            yattractor: 1000
            xattractor: 500
        }
    }

    Component.onCompleted: {
        // Calculate enemy max width
        calEnemyMaxWidth();

        // Create enemies dynamically after component created
        createEnemies(LevelPlugin.enemyCount());

        // Start animations
        enemiesGrid.y = enemiesGrid.height * -1;
        rightLeftAnim.restart();
        upToDownAnim.restart();

        // Flying stars animation, not in Maemo
        if (!GameEngine.isMaemo()) {
            starBurstTimer.restart();
        }

        GameEngine.playSound(4); // Level start sound
    }
}
