/**
 * Copyright (c) 2011 Nokia Corporation.
 */

import QtQuick 1.0

Item {
    id: message
    smooth: true
    opacity: 0

    function showMessageOngoing(text)
    {
        clear();
        textLabel.border.color = "white";
        message.width = gameArea.xDim - 10;
        message.height = gameArea.yDim * 0.1;
        textLabel.text = text;
        textLabel.fontPixelSize = message.height * 0.33;
        ongoingMessageAnim.restart();
    }

    function showMessageProgress(received, total)
    {
        if (message.opacity>0){
            progressBar.opacity = 1;
            progressBar.value = received;
            if (total!=-1)
                progressBar.maximum = total;
            else
                progressBar.maximum = 2000000;

            if (progressBar.value > progressBar.maximum)
                progressBar.maximum = progressBar.maximum * 1.2

            progressBar.color = "blue";
            progressBar.secondColor = "white";

            if (received==total) {
                textLabel.text = "Downloaded";
            }
        }
    }

    function hideOngoingMessage()
    {
        clear();
    }

    function clear()
    {
        ongoingMessageAnim.stop();
        message.opacity = 0;
    }

    SequentialAnimation {
        id: ongoingMessageAnim
        loops: Animation.Infinite

        PropertyAction { target: message; properties: "opacity"; value: 1 }
        PropertyAction { target: message; properties: "x";
                         value: (gameArea.xDim - message.width) * 0.5 }
        PropertyAction { target: message; properties: "y";
                         value: gameArea.yDim - message.height - 5 }
        PropertyAnimation {
            target: message
            property: "opacity"
            from: 0.5; to: 1
            duration: 2000
        }
        PropertyAnimation {
            target: message
            property: "opacity"
            from: 1; to: 0.5
            duration: 2000
        }
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

    ProgressBar {
        id: progressBar
        opacity: 0
        width: textLabel.width
        height: 20
        anchors.bottom: textLabel.bottom
        anchors.left: textLabel.left

    }
}
