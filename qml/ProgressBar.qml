/**
 * Copyright (c) 2011-2014 Microsoft Mobile.
 */

import QtQuick 1.0

 Item {
     id: progressbar

     property int minimum: 0
     property int maximum: 100
     property int value: 0
     property alias color: gradient1.color
     property alias secondColor: gradient2.color

     width: 250; height: 23
     clip: true

     Rectangle {
         id: highlight

         property int widthDest: ((progressbar.width * (value - minimum)) / (maximum - minimum) - 6)

         width: highlight.widthDest
         Behavior on width { SmoothedAnimation { velocity: 1200 } }

         anchors { left: parent.left; top: parent.top; bottom: parent.bottom; margins: 3 }
         radius: 1
         gradient: Gradient {
             GradientStop { id: gradient1; position: 0.0 }
             GradientStop { id: gradient2; position: 1.0 }
         }

     }
 }
