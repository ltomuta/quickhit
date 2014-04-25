/**
 * Copyright (c) 2011-2014 Microsoft Mobile.
 */

import QtQuick 1.0
import Qt.labs.particles 1.0

// Level specific JavaScript file
import "Level.js" as LevelScript

Rectangle {
    id: enemies

    property int enemiesCreated: 0
    property int enemiesEnabled: 0

    // NOTE: "level" as an object name is mandatory for all Level.qml files!
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
        // TODO: Handle pausing/resuming here.
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

    // Enemy placeholder. At least one enemy has to exist to keep the level
    // alive (otherwise it's naturally "game over" and the player wins).
    Enemy {
        x: parent.width / 2
        y: parent.height / 2
        opacity: 1
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
}
