/**
 * Copyright (c) 2011 Nokia Corporation.
 */

import QtQuick 1.0
import "Game.js" as GameScript

Item {
    id: menu

    signal levelSelected(int levelIndex)

    opacity: 0
    smooth: true
    state: "hidden"

    /**
     * Creates the items for the main menu (including the levels if any plugins
     * are installed) and displays the menu.
     */
    function showMainMenu()
    {
        mainMenuData.clear();
        listView.model = mainMenuData;
        var plugins = GameEngine.pluginList();

        // Plugin list have to be first in the menu (because usage of index)
        for (var i = 0; i < plugins.length; i++) {
            mainMenuData.append({ "name": plugins[i].displayname,
                                  "type": 0,
                                  "thumbnail": "file:/" + plugins[i].thumbnailPath,
                                  "icon": "gfx/icon_resume.png" });
        }

        // And then rest of level items
        if (IAP) {
            mainMenuData.append({ "name": "Buy Levels",
                                  "type": 5,
                                  "thumbnail": "qrc:/gfx/thumbnail.png",
                                  "icon":"gfx/icon_buy.png" });
        }

        mainMenuData.append({ "name": "About",
                              "type": 4,
                              "thumbnail": "qrc:/gfx/thumbnail.png",
                              "icon": "gfx/icon_about.png" });

        if (!GameEngine.isHarmattan()) {
            // No exit menu item on Harmattan
            mainMenuData.append({ "name": "Exit",
                                  "type": 1,
                                  "thumbnail": "qrc:/gfx/thumbnail.png",
                                  "icon":"gfx/icon_exit.png" });
        }

        menu.state = "mainMenuShown";
    }

    /**
     * Creates the items for a pause menu and displays the menu.
     */
    function showPauseMenu()
    {
        listView.model = menuData;

        if (menuData.count > 0) {
            // Items already created.
            menu.state = "pauseMenuShown";
            return;
        }

        menuData.append({ "name": "Resume",
                          "type": 2,
                          "thumbnail": "qrc:/gfx/thumbnail.png",
                          "icon": "gfx/icon_resume.png" });
        menuData.append({ "name": "End Game",
                          "type": 3,
                          "thumbnail": "qrc:/gfx/thumbnail.png",
                          "icon": "gfx/icon_end_game.png" });

        if (!GameEngine.isHarmattan()) {
            // No exit menu item on Harmattan
            menuData.append({ "name": "Exit",
                              "type": 1,
                              "thumbnail": "qrc:/gfx/thumbnail.png",
                              "icon": "gfx/icon_exit.png" });
        }

        menu.state = "pauseMenuShown";
    }

    /**
     * Hides the menu.
     */
    function hideMenu()
    {
        menu.state = "hidden";
        listView.focus = false;
    }

    /**
     * Takes action according to the selected menu item.
     *
     * @param index If the menu item is a level selection item, defines which
     *              of the levels was selected.
     * @param type The type of the menu item (e.g. level selection item).
     */
    function menuItemSelected(index, type)
    {
        switch (type) {
        case 0:
            // Level selection
            menu.state = "hidden";
            menu.levelSelected(index);
            break;
        case 1:
            // Exit
            Qt.quit();
            break;
        case 2:
            // Resume
            GameScript.pauseGame(false);
            menu.state = "hidden";
            break;
        case 3:
            // End game
            showMainMenu();
            gameArea.endGame(true);
            break;
        case 4:
            // Show about
            var infoComp = Qt.createComponent("qrc:/InfoView.qml");
            var infoObj = infoComp.createObject(infoView);
            infoObj.opacity = 1;
            break;
        case 5:
            // Buy levels
            if (btnBack.opacity == 0) {
                btnBack.opacity = 1;
                menu.state = "hidden";
                var levelComp = Qt.createComponent("qrc:/Buy.qml");
                gameArea.buyView = levelComp.createObject(buyLevels);
            }
            break;
        default:
            break;
        }
    }

    ListModel {
        id: mainMenuData
    }

    ListModel {
        id: menuData
    }

    Component {
        id: menuDelegate

        Item {
            id: container

            property int menuIconX : icon.x + icon.width

            x: (listView.width - width) / 2
            width: listView.width * 0.9
            height: 80

            // Thumbnail image
            Image {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.fill: parent
                opacity: model.type === 0 ? 1 : 0
                source: model.thumbnail
                smooth: true
            }

            // Menu item icon
            Image {
                id: icon
                source: model.icon
                smooth: true
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
            }

            // Borders and text of the menu item
            TextLabel {
                id: menuItem

                property int type: model.type

                anchors.fill: parent
                mouseAreaEnabled: true
                text: model.name
                textLeftMargin: parent.menuIconX

                onClicked: {
                    GameEngine.playInternalSound(1);
                    menu.menuItemSelected(index, menuItem.type);
                }
            }
        }
    }

    Rectangle {
        id: menuBackground
        anchors.fill: parent
        border.color: "white"
        border.width: 2
        radius: 8
        smooth: true
        color: "black"
        opacity: 0.7
    }

    ListView {
        id: listView
        anchors.centerIn: menu
        width: menu.width * 0.95
        height: menu.height * 0.9
        clip: true
        delegate: menuDelegate
        model: menuData
        spacing: 15
    }

    ScrollBar { flickableItem: listView }

    states: [
        State {
            name: "hidden"
        },
        State {
            name: "mainMenuShown"
        },
        State {
            name: "pauseMenuShown"
        }
    ]

    transitions: [
        Transition {
            from: "*"; to: "hidden"

            ParallelAnimation {
                NumberAnimation { target: menu; property: "x";
                                  to: menu.width * -0.8;
                                  easing.type: Easing.InOutBack;
                                  duration: 500; }
                PropertyAnimation { target: menu; property: "opacity"; to: 0;
                                    duration: 500 }
            }
        },
        Transition {
            from: "*"; to: "mainMenuShown"

            SequentialAnimation {
                PropertyAction { target: menu; property: "opacity"; value: 1 }
                PropertyAction { target: menu; property: "x";
                                 value: menu.width * -1.2 }
                PropertyAction { target: menu; property: "y";
                                 value: (gameArea.yDim - menu.height) * 0.7 }
                NumberAnimation { target: menu; property: "x";
                                  to: (gameArea.xDim - menu.width) * 0.5;
                                  easing.type: Easing.InOutBack;
                                  duration: 500 }
            }
        },
        Transition {
            from: "*"; to: "pauseMenuShown"

            SequentialAnimation {
                PropertyAction { target: menu; property: "opacity"; value: 1 }
                PropertyAction { target: menu; property: "x";
                                 value: menu.width * -1.2 }
                PropertyAction { target: menu; property: "y";
                                 value: (gameArea.yDim - menu.height) * 0.7 }
                NumberAnimation { target: menu; property: "x";
                                  to: (gameArea.xDim - menu.width) * 0.5;
                                  easing.type: Easing.InOutBack;
                                  duration: 500 }
            }
        }
    ]
}
