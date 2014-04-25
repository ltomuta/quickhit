/**
 * Copyright (c) 2011-2014 Microsoft Mobile.
 */

import QtQuick 1.0

Item {
    id: scrollBar

    property Flickable flickableItem: null
    property int defaultWidth: 5
    property real defaultOpacity: 0.5

    anchors {
        fill: flickableItem
        margins: defaultWidth
    }

    clip: true

    Rectangle {
        id: barRect
        x: parent.width - width
        y: parent.height * (flickableItem ? flickableItem.contentY : 0)
           / (flickableItem ? flickableItem.contentHeight : 1);
        width: scrollBar.defaultWidth
        height: flickableItem.visibleArea.heightRatio * flickableItem.height
        opacity: defaultOpacity
        color: "#8855ff"

        Behavior on opacity { NumberAnimation { duration: 100 } }

        Connections {
            target: flickableItem

            onMovementStarted: {
                if (hideTimer.running) {
                    hideTimer.stop();
                }

                barRect.opacity = scrollBar.defaultOpacity;
            }

            onMovementEnded: hideTimer.restart();
        }

        Timer {
            id: hideTimer
            repeat: false
            running: false
            interval: 750
            onTriggered: barRect.opacity = 0;
        }
    }

    Component.onCompleted: hideTimer.start();
}
