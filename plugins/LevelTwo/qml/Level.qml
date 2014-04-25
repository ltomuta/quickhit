/**
 * Copyright (c) 2011-2014 Microsoft Mobile.
 */

import QtQuick 1.0
import Qt.labs.particles 1.0
import "Level.js" as LevelScript

Item {
    id: enemies

    property int enemiesCreated: 0
    property int enemiesEnabled: 0
    
    property int backOneTop
    property int backTwoTop

    // NOTE: "level" as an object name is mandatory for all Level.qml files!
    objectName: "level"

    anchors.fill: parent

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
            enemyCreationTimer.stop();
            backgroundAnim.stop();
        }
        else {
            enemyCreationTimer.restart();
            backgroundAnim.restart();
        }

        for (var i = 0; i < LevelScript.hiddenEnemies.length; i++) {
            var enemy = LevelScript.hiddenEnemies[i];
            enemy.pause(doPause);
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
     * Enables a hidden enemy.
     */
    function enableEnemy()
    {
        if (enemiesEnabled < enemiesCreated) {
            var enemy = LevelScript.hiddenEnemies[enemiesEnabled];
            enemy.opacity = 1;
            enemy.x = GameEngine.randInt(0, gameArea.xDim - enemy.width - 50);
            enemy.startMovingAnim();
            enemyCreationTimer.stop();
            enemyCreationTimer.interval = GameEngine.randInt(500, 2000);
            enemyCreationTimer.restart();
            enemiesEnabled = enemiesEnabled + 1;
            GameEngine.playSound(4); // Enable enemy sound
        }
        else {
            enemyCreationTimer.stop();
        }
    }

    /**
     * Creates the group of hidden enemies.
     */
    function createHiddenEnemiesGroup()
    {
        for (var i = 0; i < LevelPlugin.enemyCount(); i++) {
            var enemyComponent =
                Qt.createComponent("file:/" + LevelPlugin.qmlRootPath() + "Enemy.qml");
            var enemyObject = enemyComponent.createObject(enemies);
            LevelScript.hiddenEnemies.push(enemyObject);
            enemiesCreated = enemiesCreated + 1;
        }
    }

    // Long background 1
    Image {
        id: longBackground1
        source: "file:/" + LevelPlugin.pictureRootPath() + "background_loop.png"
        fillMode: Image.Stretch
        smooth: false
        width: gameArea.xDim
        height: gameArea.yDim
    }

    // Long background 2
    Image {
        id: longBackground2
        source: "file:/" + LevelPlugin.pictureRootPath() + "background_loop.png"
        fillMode: Image.Stretch
        smooth: false
        width: gameArea.xDim
        height: gameArea.yDim
    }
    
    // Up to down animation of the background
    ParallelAnimation {
        id: backgroundAnim
        loops: Animation.Infinite
        PropertyAnimation { target: longBackground1; property: "y";
                            from: backOneTop; to: gameArea.yDim;
                            easing.type: Easing.Linear; duration: 10000 }
        PropertyAnimation { target: longBackground2; property: "y";
                            from: backTwoTop; to: backOneTop;
                            easing.type: Easing.Linear; duration: 10000 }
    }
        
    // Enemy creation timer
    Timer {
        id: enemyCreationTimer
        interval: 1000
        running: false
        repeat: true
        onTriggered: enableEnemy();
    }
    
    Timer {
        id: starBurstTimer
        interval: starParticles.lifeSpan + GameEngine.randInt(100, 2000)
        running: false
        repeat: true
        onTriggered: starParticles.burst(GameEngine.randInt(4, 6));
    }

    // Explosion particle effect
    // NOTE: This function must be implemented in all Level.qml files!
    Particles {
        id: explodeParticles
        opacity: 0
        width: 1
        height: 1
        emissionRate: 0
        source: "file:/" + LevelPlugin.pictureRootPath() + "blue_fire.png"
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
        width: parent.width
        height: 50
        emissionRate: 0
        source: "file:/" + LevelPlugin.pictureRootPath() + "star3.png"
        lifeSpan: 5000
        count: -1
        angle: 90
        velocity: 100
        opacity: 0.7

        ParticleMotionGravity {
            yattractor: 1000
            xattractor: 500
        }
     }
    
    Component.onCompleted: {
        // Create hidden enemies
        createHiddenEnemiesGroup();

        // Flying moons animation
        starBurstTimer.restart();

        // Create enemies timer
        enemyCreationTimer.restart();

        // Start background loop animation
        backOneTop = 0;
        longBackground1.y = backOneTop;
        backTwoTop = gameArea.yDim * -1;
        longBackground2.y = backTwoTop;
        backgroundAnim.restart();

        GameEngine.playSounds(5, 3); // Level starts sound, 3 times
    }
}
