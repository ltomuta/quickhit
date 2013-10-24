/**
 * Copyright (c) 2011 Nokia Corporation.
 */

import QtQuick 1.0
import "Game.js" as GameScript

Item {
    property string buttonPath // for example "qrc:/gfx/exit.png"
    property int buttonId: 0
    property bool animationEnabled: true

    signal btnClicked(int buttonId)

    width: 30 // Default width
    height: 30 // Default height
    smooth: true

    Image {
        id: image
        anchors.fill: parent
        source: buttonPath
        fillMode: Image.PreserveAspectFit
        smooth: true
        scale: 1.0
    }

    SequentialAnimation {
        id:anim
        running: false
        PropertyAction  { target: image; property: "scale"; value: 0.7 }
        PauseAnimation { duration: 200 }
        PropertyAction  { target: image; property: "scale"; value: 1.0 }
    }

    Timer {
        id: buttonPressedTimer
        interval: 300; running: false; repeat: false
        onTriggered: btnClicked(buttonId)
    }

    MouseArea {
        anchors.fill: parent

        onPressed: {
            if (animationEnabled && !anim.running) {
                anim.restart();
            }

            buttonPressedTimer.restart();
        }
    }
}
