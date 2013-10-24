/**
 * Copyright (c) 2011 Nokia Corporation.
 */

import QtQuick 1.0
import "Game.js" as GameScript


AppWindow {
    id: gameArea

    property bool gameOverProperty: false // Who wins, you or computer
    property variant messageBox // For info message to user
    property variant buyView: 0
    property variant levelPlugin: 0

    // Width and height are the other way around on Harmattan compared to
    // Symbian.
    property int xDim: width < height ? width : height;
    property int yDim: width < height ? height : width;

    // Signals from QML to Qt side
    signal levelActivated(int index)
    signal reloadPlugins()

    objectName: "gameArea"

    function foreground()
    {
        if (buyView) {
            buyView.foreground();
        }
    }

    function closeBuyView()
    {
        buyView.doBack();

        btnBack.opacity = 0;
        endGame();
        reloadPlugins();

        menu.showMainMenu();

        buyView.destroy();
        buyView = 0;
    }

    function pauseGame()
    {
        if (levelPlugin) {
            GameScript.pauseGame(true)
            myShip.focus = false
            btnPause.opacity = 0
            menu.showPauseMenu();
        }
    }

    function showMainMenu()
    {
        menu.showMainMenu();
    }

    /**
     * This function is called from the Qt C++ code in order for the QML code
     * to take a new level into use.
     */
    function levelReadyForCreation()
    {
        // Hide menu
        menu.hideMenu();

        // Show message
        message.showMessage("Loading level...", 2000);

        // Wait before create level
        levelCreationTimer.restart();
    }

    /**
     * Fires enemy missile. This function is called from the Qt C++ code.
     *
     * @param aXpox
     * @param aFromYpos
     * @param aToYpos
     */
    function fireEnemyMissile(aXpox, aFromYpos, aToYpos)
    {
        GameScript.fireEnemyMissile(aXpox, aFromYpos, aToYpos);
    }

    /**
     * Game Over handling. This function is called from the Qt C++ code.
     *
     * @param youWin If true, the player won the game. Otherwise the computer
     *               has won.
     */
    function gameOver(youWin)
    {
        gameOverProperty = youWin;
        // Delayed game over for to user see all explositions
        gameOverTimer.restart();

        if (gameOverProperty) {
            GameEngine.playInternalSound(4);
            message.showMessage("Game over, you win!", 2000);
        }
        else {
            GameEngine.playInternalSounds(3,3);
            message.showMessage("Game over, you lost", 2000);
        }
    }

    /**
     * Ends the game and shows the main menu.
     *
     * @param showMessage If true will show a message.
     */
    function endGame(showMessage)
    {
        backgroundPic.opacity = 1;

        // Do game over and hide the missiles
        GameScript.gameOver();
        GameScript.hideMissiles();

        // Clear GameEngine QML objects
        myShip.opacity = 0;
        myShip.focus = false;
        btnPause.opacity = 0;
        idMainLogo.opacity = 1;

        if (showMessage) {
            message.showMessage("Game over", 2000)
        }

        menu.showMainMenu();
    }

    // Timer for level creationing
    Timer {
        id: levelCreationTimer
        running: false
        repeat: false
        interval: 2500

        onTriggered: {
            btnPause.opacity = 1;
            myShip.opacity = 1;
            myShip.focus = true;

            // Create new level
            var ret = GameScript.createLevel();

            if (!ret) {
                backgroundPic.opacity = 0;

                // Find QML object for GameEngine
                GameEngine.findQmlObjects();

                // Enable GameEngine timer
                GameEngine.enableEngineTimer(true);
            }
            else {
                console.debug("Failed to load a level");
                endGame(false);
            }
        }
    }

    // Timer for game over
    Timer {
        id: gameOverTimer
        running: false
        repeat: false
        interval: 2000

        onTriggered: {
            GameScript.gameOver();
            GameScript.hideMissiles();

            backgroundPic.opacity = 1;
            idMainLogo.opacity = 1;

            menu.showMainMenu();
        }
    }

    // Background image for the game
    Image {
        id: backgroundPic
        z: 1
        source: "qrc:/gfx/background2.png"
        fillMode: Image.PreserveAspectCrop
        smooth: true
        anchors.fill: parent
    }

    // The animated big ship in the menu view
    Image {
        id: bigShip
        source: "qrc:/gfx/bigship.png"
        z: 1.5
        smooth: true
        x: width * -1
        y: parent.height * 0.55
    }

    SequentialAnimation {
        id: bigShipAnim

        NumberAnimation {
            target: bigShip
            property: "x"
            to: gameArea.xDim
            easing.type: Easing.Linear
            duration: 80000
        }

        PropertyAction {
            target: bigShip
            properties: "opacity"
            value: 0
        }
    }

    // Enemies grid
    Item {
        // This is general level QML plaseholder
        // Into this is level QML created in createLevel()
        id: levelId
        anchors.fill: parent
        z: 2
    }

    // Buy levels
    Item {
        id: buyLevels
        opacity: 1
        z: 2.1
    }

    Image {
        id: idMainLogo
        z: 19
        source: "qrc:/gfx/quickhit_logo.png"
        smooth: true
        anchors.horizontalCenter: parent.horizontalCenter
        y: gameArea.yDim * 0.10
    }

    // Game menu
    Menu {
        id: menu
        z: 20
        opacity: 0
        width: parent.width * 0.8
        height: parent.height * 0.6

        // Level selected
        onLevelSelected: {
            // Stop bigship animation
            bigShipAnim.stop();
            bigShip.opacity = 0;

            // Hide logo
            idMainLogo.opacity = 0;

            // Signal level activated
            gameArea.levelActivated(levelIndex);
        }
    }

    // Info
    Item {
        z: 21
        id: infoView
        anchors.fill: parent
    }

    // My ship
    MyShip {
        z: 10
        id: myShip
        opacity: 0
    }

    // Mouse area of your ship
    MouseArea {
        property int startPos: 0
        property int dragCount: 0

        width: gameArea.xDim
        height: myShip.height
        x: 0
        y: gameArea.yDim - myShip.height

        drag.target: myShip
        drag.axis: Drag.XAxis
        drag.minimumX: 0
        drag.maximumX: gameArea.xDim - myShip.width

        onPressed: {
            startPos = mouseX;
            dragCount = 0;
        }
        onReleased: {
            myShip.fire();
        }
    }

    // Toggle sound button
    Button {
        id: btnSound
        z: 12
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 15
        buttonPath: "qrc:/gfx/soundOn.png"
        buttonId: 4
        width: gameArea.xDim / 10
        height: width
        opacity: 1

        onBtnClicked: {
            if (btnSound.buttonId == 4) {
                // Sound off
                btnSound.buttonPath = "qrc:/gfx/soundOff.png";
                btnSound.buttonId = 5;
                GameEngine.enableSounds(false);
            }
            else {
                // Sound on
                btnSound.buttonPath = "qrc:/gfx/soundOn.png";
                btnSound.buttonId = 4;
                GameEngine.enableSounds(true);
            }
        }
    }

    // Pause button
    Button {
        id: btnPause
        z: 13
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.right: btnSound.left
        anchors.rightMargin: 15
        buttonPath: "qrc:/gfx/pause.png"
        buttonId: 3
        width: btnSound.width
        height: width
        opacity: 0

        onBtnClicked: {
            GameScript.pauseGame(true);
            myShip.focus = false;
            btnPause.opacity = 0;
            menu.showPauseMenu();
        }
    }

    // Back button
    Button {
        id: btnBack
        z: 13.1
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.right: btnSound.left
        anchors.rightMargin: 15
        buttonPath: "qrc:/gfx/back.png"
        width: btnSound.width
        height: width
        opacity: 0

        onBtnClicked: {
            closeBuyView();
        }
    }

    // Hidden missiles ready for to be launched
    Missile {
        z: 3
        id: missile_1
        x: 0
        y: 10
    }
    Missile {
        z: 4
        id: missile_2
        x: 20
        y: 10
    }
    Missile {
        z: 5
        id: missile_3
        x: 40
        y: 10
    }
    Missile {
        z: 6
        id: missile_4
        x: 60
        y: 10
    }
    Missile {
        z: 7
        id: missile_5
        x: 80
        y: 10
    }
    Missile {
        z: 8
        objectName: "enemy_missile"
        id: enemy_missile_1
        enemyMissile: true
    }
    Missile {
        z: 9
        objectName: "enemy_missile"
        id: enemy_missile_2
        enemyMissile: true
    }

    // Messages to the user
    MessageProgress {
        id: messageProgress
        z: 19.9
    }
    Message {
        id: message
        z: 21
    }

    // Splach screen
    Rectangle {
        id: blackFace
        x: -2
        y: 0
        width: parent.width + 2
        height: parent.height
        z: 100
        color: "black"
        opacity: 1

        MouseArea {
            anchors.fill: parent
            onPressed: {
                mouse.accepted = true;
                fadeAnim.stop();
                rotAnim.stop();
                blackFace.opacity = 0;
                idLogo.opacity = 1;
                menu.showMainMenu();
            }
        }

        Image {
            id: idLogo
            source: "qrc:/gfx/quickhit_logo.png"
            smooth: true
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            opacity: 0
        }

        NumberAnimation {
            id: rotAnim
            target: idLogo
            property: "rotation"
            to: 20
            duration: 9000
        }
    }

    // Splach screen animation
    SequentialAnimation {
        id: fadeAnim
        NumberAnimation { target: idLogo; property: "opacity"; from: 0; to: 1; duration: 2000 }
        PauseAnimation { duration: 1000 }
        NumberAnimation { target: idLogo; property: "opacity"; from: 1; to: 0; duration: 2000 }
        NumberAnimation { target: blackFace; property: "opacity"; from: 1; to: 0; duration: 1000 }
        PauseAnimation { duration: 200 }
        ScriptAction { script: showMainMenu() }
    }

    Component.onCompleted: {
        // Start big ship animation
        bigShipAnim.restart();

        // Set variable
        messageBox = message;

        // Play game start sound
        GameEngine.gameStartSound();

        // Fade splash screen
        fadeAnim.restart();

        // Rotate splash logo
        rotAnim.restart();
    }
}
