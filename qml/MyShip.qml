/**
 * Copyright (c) 2011 Nokia Corporation.
 */

import QtQuick 1.0
import "Game.js" as GameScript

Item {
    id: myShip
    objectName: "myShip"

    property variant myShipSize
    property int originalY

    /**
     * Fires missile if the ship exists.
     */
    function fire()
    {
        if (myShip.opacity == 1)
        {
            GameScript.fireMissile(myShip.x + myShip.width / 2,
                                   myShip.y, myShip.height * -1);

            if (myShip.y + myShip.height < gameArea.yDim + 5) {
                goDownAnim.restart();
            }
        }
    }

    /**
     * Sets the graphics and the position of the ship.
     */
    function createGraphicsForLevel()
    {
        if (LevelPlugin) {
            myShip.myShipSize =
                    LevelPlugin.graphSize(LevelPlugin.pathToMyShipPic());
            myShip.height = myShipSize.height;
            myShip.width = myShipSize.width;
            image.source = "file:/" + LevelPlugin.pathToMyShipPic();
            myShip.y = gameArea.yDim - myShip.height - 10;
            originalY = myShip.y;
            myShip.x = (gameArea.xDim - myShip.width) / 2;
        }
    }

    // The visual representation of the ship. Initialized by
    // createGraphicsForLevel() function.
    Image {
        id: image
        smooth: true
    }

    // Handle keys
    Keys.onSpacePressed: fire();
    Keys.onSelectPressed: fire();
    Keys.onRightPressed: {
        if (myShip.x < (gameArea.xDim - myShip.width - 20)) {
            toRightAnim.restart();
        }
    }
    Keys.onLeftPressed: {
        if (myShip.x > 20) {
            toLeftAnim.restart();
        }
    }

    // To right animation
    PropertyAnimation {
        id: toRightAnim
        target: myShip
        easing.type: Easing.OutQuint
        properties: "x"
        from: myShip.x; to: myShip.x + 20
        duration: 500
    }

    // To left animation
    PropertyAnimation {
        id: toLeftAnim
        target: myShip
        easing.type: Easing.OutQuint
        properties: "x"
        from: myShip.x; to: myShip.x - 20
        duration: 500
    }

    // Go down on fire animation
    SequentialAnimation {
        id: goDownAnim

        NumberAnimation {
            target: myShip
            property: "y"
            from: myShip.y; to: myShip.y + 5
            easing.type: Easing.Linear
            duration: 200
        }
        NumberAnimation {
            target: myShip
            property: "y";
            from: myShip.y; to: originalY
            easing.type: Easing.Linear
            duration: 200
        }
    }
}
