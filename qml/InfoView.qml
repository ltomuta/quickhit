/**
 * Copyright (c) 2011 Nokia Corporation.
 */

import QtQuick 1.0

Item {
    id: container
    objectName: "infoView"
    anchors.fill: parent
    opacity: 0

    // Mouse area for hiding this info view
    MouseArea {
        anchors.fill: parent
        onClicked: container.opacity = 0;
    }

    Rectangle {
        anchors.fill: parent
        anchors.margins: 30
        radius: 5
        color: "black"
        border.color: "white"
        border.width: 2

        Behavior on opacity {
            PropertyAnimation {
                duration: 500
            }
        }

        Flickable {
            id: flickable

            anchors {
                fill: parent
                topMargin: 20
                leftMargin: 10
                rightMargin: 5
                bottomMargin: 10
            }

            contentHeight: infoText.height + closeButton.height
            clip: true

            Text {
                id: infoText
                width: flickable.width - 10 // Save room for scroll bar
                color: "white"
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                font.pixelSize: container.width * 0.055

                text: {
                    infoText.text =
                        "<a href=\"https://projects.developer.nokia.com/quickhit\">" +
                        "QuickHit</a>" +
                        "<p>A Nokia Developer Qt Quick Game Example</p>" +
                        "<p>QuickHit is essentially a basic arcade-style " +
                        "shooting game, but differentiated from other such " +
                        "games by its levels which are implemented as " +
                        "separate Qt plug-ins.</p>";

                    if (GameEngine.isSymbian()) {
                        // IAP API is only available on Symbian
                        infoText.text +=
                                "<p>Try In-App Purchase to buy more " +
                                "levels by selecting 'Buy Levels' from " +
                                "the main menu.</p>";
                    }

                    infoText.text +=
                        "</p>Sounds were loaded from freesound.org, under " +
                        "Creative Commons Sampling Plus 1.0 license.</p>" +
                        "<p>The sounds were created by:<br />" +
                        "HardPCM, Matt_G, klankbeeld, steveygos93, " +
                        "joe93barlow, ljudman, Jovica, patchen and " +
                        "nthompson</p><br />";
                }

                onLinkActivated: Qt.openUrlExternally(link);
            }

            // Close button
            TextLabel {
                id: closeButton
                width: parent.width * 0.75
                height: 60

                anchors {
                    horizontalCenter: parent.horizontalCenter
                    rightMargin: 5
                    bottom: parent.bottom
                }

                // Close icon
                Image {
                    id: icon

                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                    }

                    source: "gfx/icon_exit.png"
                    smooth: true
                }

                mouseAreaEnabled: true
                text: "Close"
                textLeftMargin: icon.width
                onClicked: container.opacity = 0;
            }
        } // Flickable

        ScrollBar { flickableItem: flickable }
    }
}
