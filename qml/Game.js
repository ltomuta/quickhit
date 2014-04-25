/**
 * Copyright (c) 2011-2014 Microsoft Mobile.
 */

/**
 * Hides the missiles.
 */
function hideMissiles()
{
    missile_1.stop();
    missile_1.opacity = 0;

    missile_2.stop();
    missile_2.opacity = 0;

    missile_3.stop();
    missile_3.opacity = 0;

    missile_4.stop();
    missile_4.opacity = 0;

    missile_5.stop();
    missile_5.opacity = 0;

    enemy_missile_1.opacity = 0;
    enemy_missile_2.opacity = 0;
}


/**
 *
 */
function gameOver()
{
    // Stop GameEngine timers
    GameEngine.enableEngineTimer(false);

    // Delete loaded plugin level
    deleteLevel();

    // Hiding
    myShip.opacity = 0;
    btnPause.opacity = 0;
}


/**
 * Pauses the game.
 *
 * @param doPause
 */
function pauseGame(doPause)
{
    missile_1.pause(doPause)
    missile_2.pause(doPause)
    missile_3.pause(doPause)
    missile_4.pause(doPause)
    missile_5.pause(doPause)
    enemy_missile_1.pause(doPause)
    enemy_missile_2.pause(doPause)

    if (doPause) {
        btnPause.opacity = 0
        myShip.focus = false;
    }
    else {
        btnPause.opacity = 1
        myShip.focus = true;
    }

    GameEngine.pauseLevel(doPause)
}


/**
 * Fires the player's missile.
 *
 * @param aXpox
 * @param aFromYpos
 * @param aToYpos
 */
function fireMissile(aXpox, aFromYpos, aToYpos)
{
    if (missile_1.opacity < 1) {
        missile_1.fire(aXpox,aFromYpos,aToYpos)
    }
    else if (missile_2.opacity < 1) {
        missile_2.fire(aXpox,aFromYpos,aToYpos)
    }
    else if (missile_3.opacity < 1) {
        missile_3.fire(aXpox,aFromYpos,aToYpos)
    }
    else if (missile_4.opacity < 1) {
        missile_4.fire(aXpox,aFromYpos,aToYpos)
    }
    else if (missile_5.opacity < 1) {
        missile_5.fire(aXpox,aFromYpos,aToYpos)
    }
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
    if (enemy_missile_1.opacity == 0) {
        enemy_missile_1.enemyFire(aXpox, aFromYpos, aToYpos)
    }
    else if (enemy_missile_2.opacity == 0) {
        enemy_missile_2.enemyFire(aXpox, aFromYpos, aToYpos)
    }
}


/**
 * Creates the level QML component.
 */
function createLevel()
{
    var levelComponent =
        Qt.createComponent("file:/" + LevelPlugin.qmlRootPath() + "Level.qml");

    if (levelComponent.status == Component.Ready) {
        if (gameArea.levelPlugin) {
            deleteLevel();
        }

        gameArea.levelPlugin = levelComponent.createObject(levelId);
        if (gameArea.levelPlugin == null) {
            messageBox.showErrorMessage("Error: Failed to create level!", 6000);
            return 1;
        }
    }
    else {
        messageBox.showErrorMessage("Error: Failed to create level!", 6000);
        return 1;
    }

    missile_1.createGraphicsForLevel();
    missile_1.setToDefaultPos();
    missile_2.createGraphicsForLevel();
    missile_2.setToDefaultPos();
    missile_3.createGraphicsForLevel();
    missile_3.setToDefaultPos();
    missile_4.createGraphicsForLevel();
    missile_4.setToDefaultPos();
    missile_5.createGraphicsForLevel();
    missile_5.setToDefaultPos();
    enemy_missile_1.createGraphicsForLevel();
    enemy_missile_2.createGraphicsForLevel();
    myShip.createGraphicsForLevel();

    return 0;
}


/**
 * Deletes the current level.
 */
function deleteLevel()
{
    if (gameArea.levelPlugin) {
        GameEngine.clearQmlObjects();
        gameArea.levelPlugin.opacity = 0;
        gameArea.levelPlugin.destroy();
        gameArea.levelPlugin = 0;
    }
}
