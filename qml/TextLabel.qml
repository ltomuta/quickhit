/**
 * Copyright (c) 2011-2014 Microsoft Mobile.
 */

import QtQuick 1.0

Rectangle {
    id: textLabel

    property alias mouseAreaEnabled: textLabelMouseArea.enabled
    property alias text: textLabelText.text
    property alias textVerticalAlignment: textLabelText.verticalAlignment
    property int textLeftMargin: 0
    property int fontPixelSize: height * 0.33
    signal clicked()

    width: 200 // Default width
    height: 80 // Default height
    radius: 5
    color: "transparent"
    border.color: textLabelMouseArea.pressed ? "red" : "white";
    border.width: textLabelMouseArea.pressed ? 4 : 2;
    smooth: true

    gradient: Gradient {
        GradientStop { position: 0.0; color: "transparent" }
        GradientStop { position: 0.8; color: "transparent" }
        GradientStop { position: 1.0; color: "#ee01e6" }
    }

    Text {
        id: textLabelText

        anchors {
            fill: parent
            leftMargin: 10 + textLabel.textLeftMargin
            rightMargin: 10
        }

        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.WordWrap
        color: "white";
        font.pixelSize: textLabel.fontPixelSize
        font.bold: true
        onLinkActivated: GameEngine.openLink(link);
    }

    MouseArea {
        id: textLabelMouseArea
        anchors.fill: parent
        onClicked: textLabel.clicked();
    }
}
