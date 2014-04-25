/**
 * Copyright (c) 2011-2014 Microsoft Mobile.
 */

import QtQuick 1.0

Item {
    id: message
    smooth: true
    opacity: 0

    function showMessage(text, speed)
    {
        clear();
        textLabel.border.color = "white";
        message.width = gameArea.xDim - 10
        message.height = gameArea.yDim * 0.1
        textLabel.text = text;
        textLabel.fontPixelSize = message.height * 0.33;
        messageAnim.restart();
    }

    function showErrorMessage(text, speed)
    {
        clear();
        textLabel.border.color = "red";
        message.width = gameArea.xDim - 10
        message.height = gameArea.yDim * 0.1
        textLabel.text = text;
        textLabel.fontPixelSize = message.height * 0.33;
        messageAnim.restart();
    }

    function clear()
    {
        messageAnim.stop();
        message.opacity = 0;
    }

    transform: Rotation {
        id: rotation
        origin.x: message.width * 0.5;
        origin.y: message.height * 0.5;
        axis.x:1; axis.y:0; axis.z:0
        angle:90
    }

    SequentialAnimation {
        id: messageAnim
        PropertyAction { target: message; properties: "opacity"; value: 1 }
        PropertyAction { target: message; properties: "x";
                         value: (gameArea.xDim - message.width) * 0.5 }
        PropertyAction { target: message; properties: "y"; value: 5 }
        NumberAnimation { target: rotation; property: "angle"; from: 90; to:0; duration: 600 }
        PauseAnimation { duration: 3000 }
        NumberAnimation { target: rotation; property: "angle"; from: 0; to:90; duration: 600 }
        PropertyAction { target: message; properties: "opacity"; value: 0 }
    }

    TextLabel {
        id: textLabel
        anchors.fill: parent
        opacity: 1
        border.color: "black"

        gradient: Gradient {
            GradientStop { position: 0.0; color: "transparent" }
            GradientStop { position: 0.6; color: "black" }
            GradientStop { position: 0.8; color: "black" }
            GradientStop { position: 1.0; color: "#ee01e6" }
        }

        mouseAreaEnabled: false
        textVerticalAlignment: Text.AlignVCenter
    }

    MouseArea {
        anchors.fill: parent
        onClicked: message.clear();
    }
}
