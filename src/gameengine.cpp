/**
 * Copyright (c) 2011 Nokia Corporation.
 */

#include "gameengine.h"

#include <QDebug>
#include <QDeclarativeContext>
#include <QDesktopServices>
#include <QPluginLoader>
#include <QTime>
#include <QTimerEvent>

#include "invsounds.h"
#include "plugindata.h"
#include "../plugins/levelplugininterface.h"

// Constants
const int TimerSpeed(85);
const int DesktopFixedWidth(480);
const int DesktopFixedHeight(854);
const char *LevelObjectName("level");
const char *PlayerShipObjectName("myShip");
const char *EnemyObjectName("enemy");
const char *EnemyGridObjectName("enemiesGrid");
const char *MissileObjectName("missile");
const char *EnemyMissileObjectName("enemy_missile");


/*!
  \class GameEngine
  \brief What it says in the name of this class.
*/


/*!
  Constructor.
*/
GameEngine::GameEngine(QObject *parent)
    : QObject(parent),
      m_levelQml(0),
      m_gameQml(0),
      m_myShipQml(0),
      m_enemiesGridQml(0),
      m_gameLevel(0),
      m_soundEngine(0),
      m_enableEnemyMissiles(false),
      m_doEnemyMissile(1500 / TimerSpeed),
      m_timerId(0),
      m_silent(false)
#if defined Q_OS_SYMBIAN || defined Q_WS_MAEMO_5 || defined (Q_WS_MAEMO_6)
      , m_systemDeviceInfo(0)
#endif
#ifdef Q_OS_SYMBIAN
      , iVibrate(0)
#endif
{
    clearQmlObjects();

    // Randomize
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    // Sound engine
    m_soundEngine = new InvSounds(this);

#ifdef Q_OS_SYMBIAN
    iVibrate = CHWRMVibra::NewL();
#endif

#if defined(Q_OS_SYMBIAN) || defined(Q_WS_MAEMO_5) || defined(Q_WS_MAEMO_6)
    // Get device profile, is it silent?
    m_systemDeviceInfo = new QSystemDeviceInfo(this);
    connect(m_systemDeviceInfo, SIGNAL(currentProfileChanged(QSystemDeviceInfo::Profile)),
            this, SLOT(currentProfileChanged(QSystemDeviceInfo::Profile)));
    QSystemDeviceInfo::Profile profile = m_systemDeviceInfo->currentProfile();

    if (profile == QSystemDeviceInfo::SilentProfile) {
        m_silent = true;
    }
#endif
}


/*!
  Destructor.
*/
GameEngine::~GameEngine()
{
#ifdef Q_OS_SYMBIAN
    delete iVibrate;
#endif
}


/*!
  Sets the game QML object.
*/
void GameEngine::setGameQml(QObject *obj)
{
    m_gameQml = static_cast<QDeclarativeItem*>(obj);
}


/*!
  Sets the game level to \a level.
*/
void GameEngine::setGameLevel(LevelPluginInterface *level)
{
    // Set game level
    m_gameLevel = level;

    if (m_gameLevel) {
        // Set used sounds from the level into sound engine
        m_soundEngine->enableSounds(m_gameLevel->levelSounds());

        // Invoke QML to take new level in use
        QMetaObject::invokeMethod(m_gameQml, "levelReadyForCreation",
                                  Qt::AutoConnection);
        m_doEnemyMissile = m_gameLevel->enemyFireSpeed().toInt() / TimerSpeed;
    }
}


/*!
  Constructs a plugin metadata instance based on \a pluginInterface and appends
  it into the list of plugins.
*/
void GameEngine::addPlugin(LevelPluginInterface *pluginInterface)
{
    PluginData *data = new PluginData();
    data->setDisplayname(pluginInterface->displayname().toString());
    data->setThumbnailPath(pluginInterface->pathToLevelThumbnailPic().toString());
    data->setIapid(pluginInterface->iapId().toString());
    data->setFilename("");
    m_pluginList.append(data);
}


/*!
  Calls foreground() function in QML code.
  \see Game.qml
*/
void GameEngine::foreground()
{
    QMetaObject::invokeMethod(m_gameQml, "foreground", Qt::AutoConnection);
}


/*!
  Utility method returning a random value within the scope of \a low and
  \a high.
*/
QVariant GameEngine::randInt(QVariant low, QVariant high)
{
    // Random number between low and high
    return qrand() % ((high.toInt() + 1) - low.toInt()) + low.toInt();
}


/*!
  Returns the list of plugins as QVariant.
*/
QVariant GameEngine::pluginList()
{
    return QVariant::fromValue(m_pluginList);
}


/*!
  Plays the game start sound.
*/
void GameEngine::gameStartSound()
{
    if (!m_silent) {
        m_soundEngine->gameStartSound();
    }
}


// For resolving the platform in QML code:

QVariant GameEngine::isHarmattan()
{
#ifdef Q_WS_MAEMO_6
    return QVariant(true);
#else
    return QVariant(false);
#endif
}

QVariant GameEngine::isSymbian()
{
#ifdef Q_OS_SYMBIAN
    return QVariant(true);
#else
    return QVariant(false);
#endif
}

QVariant GameEngine::isMaemo()
{
#ifdef Q_WS_MAEMO_5
    return QVariant(true);
#else
    return QVariant(false);
#endif
}

QVariant GameEngine::isWindows()
{
#ifdef Q_OS_WIN
    return QVariant(true);
#else
    return QVariant(false);
#endif
}


/*!
  Resets the QML objects.
*/
void GameEngine::findQmlObjects()
{
    if (!m_gameQml) {
        qDebug() << "GameEngine::findQmlObjects(): No QML set!";
        return;
    }

    // Find Missile objects
    m_missileList.clear();
    m_enemyMissileList.clear();
    findMissiles(m_gameQml);

    // Set QMLs
    setLevelQml(m_gameQml->findChild<QObject*>(LevelObjectName));
    setMyShipQml(m_gameQml->findChild<QObject*>(PlayerShipObjectName));
    setEnemiesGridQml(m_gameQml->findChild<QObject*>(EnemyGridObjectName));

    // Find Enemy objects
    m_enemyList.clear();
    findEnemies(m_levelQml);
}


/*!
  Clears the QML objects.
*/
void GameEngine::clearQmlObjects()
{
    m_missileList.clear();
    m_enemyMissileList.clear();
    m_enemyList.clear();
    m_levelQml = 0;
    m_enemiesGridQml = 0;
    m_myShipQml = 0;
}


/*!
  Clears the plugin list.
*/
void GameEngine::clearPluginList()
{
    m_pluginList.clear();
}


/*!
  Starts/stops the engine timer depending on the value of \a enable.
*/
void GameEngine::enableEngineTimer(QVariant enable)
{
    if (m_gameLevel) {
        if (m_timerId == 0 && enable.toBool()) {
            m_timerId = QObject::startTimer(TimerSpeed);
        }
        else if (m_timerId != 0 && !enable.toBool()) {
            QObject::killTimer(m_timerId);
            m_timerId = 0;
        }
    }
}


/*!
  Mutes/unmutes sound depending on the value of \a enable.
*/
void GameEngine::enableSounds(QVariant enable)
{
    m_silent = !enable.toBool();

    if (m_silent) {
        m_soundEngine->enableSounds(false);
    }
    else {
        m_soundEngine->enableSounds(true);
    }
}


/*!
  Calls pauseGame() function in QML code.
  \see Game.qml
*/
void GameEngine::pauseGame()
{
    QMetaObject::invokeMethod(m_gameQml, "pauseGame", Qt::AutoConnection);
}


/*!
  Pauses/resumes the level depending on the value of \a doPause.
*/
void GameEngine::pauseLevel(QVariant doPause)
{
    bool enableTimer = !doPause.toBool();
    enableEngineTimer(QVariant(enableTimer));
    QMetaObject::invokeMethod(m_levelQml, "pause", Qt::AutoConnection,
                              Q_ARG(QVariant, doPause));
}


/*!
  Plays sound with \a index.
*/
void GameEngine::playSound(QVariant index)
{
    if (!m_silent) {
        int i = index.toInt();
        m_soundEngine->playSound(i);
    }
}


/*!
  Plays sound with \a index \a count times.
*/
void GameEngine::playSounds(QVariant index, QVariant count)
{
    if (!m_silent) {
        m_soundEngine->playSounds(index.toInt(), count.toInt());
    }
}


/*!
  Plays internal sound with \a index.
*/
void GameEngine::playInternalSound(QVariant index)
{
    if (!m_silent) {
        m_soundEngine->playInternalSound(index.toInt());
    }
}


/*!
  Plays internal sounds with \a index \a count times.
*/
void GameEngine::playInternalSounds(QVariant index, QVariant count)
{
    if (!m_silent) {
        m_soundEngine->playInternalSounds(index.toInt(), count.toInt());
    }
}


/*!
  Provides haptics feedback.
*/
void GameEngine::vibra()
{
#ifdef Q_OS_SYMBIAN
    if ( iVibrate )
        {
        TRAPD( err, iVibrate->StartVibraL( 4000, KHWRMVibraMaxIntensity ) );
        }
#endif
}


/*!
  Provides fast haptics feedback.
*/
void GameEngine::fastVibra()
{
#ifdef Q_OS_SYMBIAN
    if ( iVibrate )
        {
        TRAPD( err, iVibrate->StartVibraL( 100, KHWRMVibraMaxIntensity ) );
        }
#endif
}


/*!
  Opens \a link with external application (e.g. a web page with a browser).
*/
void GameEngine::openLink(QVariant link)
{
    QDesktopServices::openUrl(QUrl(link.toString()));
}


/*!
  Executes timed game logic.

  From QObject.
  \see http://doc.qt.nokia.com/latest/qobject.html#timerEvent
*/
void GameEngine::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timerId) {
        // Do hit test
        doHitTest();

        m_doEnemyMissile--;

        if (m_gameLevel && m_doEnemyMissile < 0) {
            m_doEnemyMissile = m_gameLevel->enemyFireSpeed().toInt() / TimerSpeed;
            doEnemyMissile(); // Do enemy missile launch
        }
    }
}


/*!
  Locates missile items recursively from the QML context provided by
  \a rootObject and stores pointers to them into the internal lists.
*/
void GameEngine::findMissiles(QObject *rootObject)
{
    if (!rootObject) {
        qDebug() << "GameEngine::findMissiles(): Invalid argument (NULL)!";
        return;
    }

    QObjectList list = rootObject->children();
    QObject *item;

    foreach (item, list) {
        if (item->children().count() > 0) {
            findMissiles(item);
        }
        else {
            if (!rootObject->objectName().compare(MissileObjectName)) {
                // The item is a player's missile.
                QDeclarativeItem *missile =
                        static_cast<QDeclarativeItem*>(rootObject);
                m_missileList.append(missile);
            }
            else if (!rootObject->objectName().compare(EnemyMissileObjectName)) {
                // The item is an enemy missile.
                QDeclarativeItem *enemyMissile =
                        static_cast<QDeclarativeItem*>(rootObject);
                m_enemyMissileList.append(enemyMissile);
            }
        }
    }
}


/*!
  Locates enemy items recursively from the QML context provided by
  \a rootObject and stores pointers to them into the internal list.
*/
void GameEngine::findEnemies(QObject *rootObject)
{
    if (!rootObject) {
        //qDebug() << "GameEngine::findEnemies(): Invalid argument (NULL)!";
        return;
    }

    QObjectList list = rootObject->children();
    QObject *item;

    foreach (item, list) {
        if (item->children().count() > 0
                && item->objectName().compare(EnemyObjectName)) {
            // Children exist and this item is NOT an enemy. Go deeper.
            findEnemies(item);
        }
        else if (!item->objectName().compare(EnemyObjectName)) {
            // This item is an enemy.
            QDeclarativeItem *enemy = static_cast<QDeclarativeItem*>(item);
            m_enemyList.append(enemy);
        }
    }
}


/*!
  Sets the level to \a obj (which is implemented with QML code).
*/
void GameEngine::setLevelQml(QObject *obj)
{
    m_levelQml = obj;
}


/*!
  Sets the player's ship as \a obj.
*/
void GameEngine::setMyShipQml(QObject *obj)
{
    m_myShipQml = static_cast<QDeclarativeItem*>(obj);
}


/*!
  Sets the enemy grid as \a obj.
*/
void GameEngine::setEnemiesGridQml(QObject *obj)
{
    m_enemiesGridQml = static_cast<QDeclarativeItem*>(obj);
}


/*!
  Calls gameOver() function with argument \a youWin in QML code.
  \see Game.qml
*/
void GameEngine::gameOver(bool youWin)
{
    QMetaObject::invokeMethod(m_gameQml, "gameOver", Qt::AutoConnection,
                              Q_ARG(QVariant, youWin));
}


/*!
  Resolves the first and the last index of visible enemies and inserts the
  values into \a start and \a end.
*/
void GameEngine::selectVisibleEnemy(int &start, int &end)
{
    QDeclarativeItem *enemy(0);

    for (int i = 0; i < m_enemyList.count(); i++) {
        enemy = m_enemyList[i];

        if (enemy->opacity() == 1) {
            start = i;
            break;
        }
    }

    for (int e = m_enemyList.count() - 1; e > 0; e--) {
        enemy = m_enemyList[e];

        if (enemy->opacity() == 1) {
            end = e;
            break;
        }
    }
}


/*!
  Randomly selects an enemy vessel from the visible ones and makes it to launch
  a missile.
*/
void GameEngine::doEnemyMissile()
{
    QMutexLocker locker(&m_enemyListMutex);
    QDeclarativeItem *missile(0);
    QDeclarativeItem *enemy(0);

    // Find a free missile
    foreach (missile, m_enemyMissileList) {
        if (missile->opacity() == 0) {
            // Select the enemy to fire randomly.
            int start(0);
            int end(0);
            selectVisibleEnemy(start, end);
            int whoWillFire = randInt(QVariant(start), QVariant(end)).toInt();

            if (m_enemyList.count() < whoWillFire + 1) {
                break;
            }

            enemy = m_enemyList.at(whoWillFire);

            if (enemy && enemy->opacity() == 1) {
                QPointF enemyPosition = enemy->pos();

                if (m_enemiesGridQml) {
                    enemyPosition += m_enemiesGridQml->pos();
                }

                int height =
                        m_gameQml->boundingRect().height() >
                        m_gameQml->boundingRect().width() ?
                            m_gameQml->boundingRect().height() :
                            m_gameQml->boundingRect().width();

                QMetaObject::invokeMethod(
                    m_gameQml, "fireEnemyMissile", Qt::AutoConnection,
                    Q_ARG(QVariant, enemyPosition.x() + enemy->boundingRect().width() / 4),
                    Q_ARG(QVariant, enemyPosition.y() + enemy->boundingRect().height()),
                    Q_ARG(QVariant, height));
            }

            break;
        }
    }
}


/*!
  Checks if the player or an enemy was destroyed (collision or hit by a
  missile).
*/
void GameEngine::doHitTest()
{
    if (!m_enemyList.count()) {
        // No enemies! The player has won!
        enableEngineTimer(QVariant(false));
        gameOver(true);
        return;
    }

    if (!m_myShipQml) {
        // No player's ship!
        return;
    }

    QDeclarativeItem *missile(0);
    QDeclarativeItem *enemy(0);

    // Check for collision
    if (m_myShipQml->opacity() == 1) {
        for (int e = 0; e < m_enemyList.count(); e++) {
            enemy = m_enemyList[e];

            if (enemy->opacity() == 0) {
                // Not a visible enemy.
                break;
            }

            QPointF enemyPos = enemy->pos();

            if (m_enemiesGridQml) {
                enemyPos += m_enemiesGridQml->pos();
            }

            QRectF enemyRect(enemyPos,
                             QSize(enemy->boundingRect().width(),
                                   enemy->boundingRect().height()));

            // Check for collision or if an enemy has gotten too deep
            if (enemyRect.contains(m_myShipQml->pos())
                    || (enemyRect.bottomLeft().y() >
                            m_myShipQml->pos().y()
                            + m_myShipQml->pos().y() * 0.1)) {
                // Enemy has collided with the player or gotten too deep!
                // Thus, the player loses!
                enableEngineTimer(QVariant(false));

                // Collision explosion
                QPointF playerPos =  m_myShipQml->pos();
                playSound(1);
                QMetaObject::invokeMethod(m_levelQml, "explode", Qt::AutoConnection,
                        Q_ARG(QVariant, playerPos.x() + m_myShipQml->boundingRect().width() / 2),
                        Q_ARG(QVariant, playerPos.y() + m_myShipQml->boundingRect().height()));
                m_myShipQml->setOpacity(0);

                gameOver(false); // "You lose"
                return;
            }
        }
    }

    // Check if the player's missiles have hit the enemies
    foreach (missile, m_missileList) {
        if (missile->opacity() == 1) {
            for (int e = 0; e < m_enemyList.count(); e++) {
                enemy = m_enemyList[e];

                if (enemy->opacity() < 1) {
                    // Not a visible enemy.
                    break;
                }

                QPointF missilePos = missile->pos();
                missilePos.setX(missilePos.rx()
                                + missile->boundingRect().width() / 2);

                QPointF enemyPos = enemy->pos();

                if (m_enemiesGridQml) {
                    enemyPos += m_enemiesGridQml->pos();
                }

                QRectF enemyRect(enemyPos,
                                 QSize(enemy->boundingRect().width(),
                                       enemy->boundingRect().height()));

                if (enemyRect.contains(missilePos)) {
                    // It's a hit!
                    playSound(0);
                    QMetaObject::invokeMethod(m_levelQml, "explode", Qt::AutoConnection,
                            Q_ARG(QVariant, enemyPos.x() + enemy->boundingRect().width() / 2),
                            Q_ARG(QVariant, enemyPos.y() + enemy->boundingRect().height()));
                    missile->setOpacity(0);
                    //fastVibra();

                    if (m_enemiesGridQml) {
                        // Set transparent placeholder for enemy when using GridView
                        enemy->setProperty("source",
                                QVariant("file:/" + m_gameLevel->pathToTransparentEnemyPic().toString()));
                    }
                    else {
                        // Hide enemy after the explosion
                        enemy->setOpacity(0);
                    }

                    // Remove the enemy from the list
                    m_enemyList.removeAt(e);
                    e--;
                }
            }
        }
    }

    // Check if an enemy missile has hit the player
    if (m_myShipQml->opacity() == 1) {
        foreach (missile, m_enemyMissileList) {
            if (missile->opacity() == 1) {
                // Get the missile position.
                QPointF missilePos = missile->pos();
                missilePos.setX(missilePos.rx()
                                + missile->boundingRect().width() / 2);

                QPointF playerPos = m_myShipQml->pos();
                QRectF playerRect(playerPos,
                                  QSize(m_myShipQml->boundingRect().width(),
                                        m_myShipQml->boundingRect().height()));

                if (playerRect.contains(missilePos)) {
                    // It's a hit!
                    playSound(1);
                    QMetaObject::invokeMethod(m_levelQml, "explode", Qt::AutoConnection,
                            Q_ARG(QVariant, playerPos.x() + m_myShipQml->boundingRect().width() / 2),
                            Q_ARG(QVariant, playerPos.y() + m_myShipQml->boundingRect().height()));
                    missile->setOpacity(0);
                    m_myShipQml->setOpacity(0);
                    break;
                }
            }
        }
    }
    else {
        // The player was killed (opacity == 0)! Game over!
        enableEngineTimer(QVariant(false));
        gameOver(false);
    }
}


#if defined(Q_OS_SYMBIAN) || defined(Q_WS_MAEMO_5) || defined (Q_WS_MAEMO_6)
/*!
  Handles the profile changes. Mutes the sound if necessary.
*/
void GameEngine::currentProfileChanged(QSystemDeviceInfo::Profile profile)
{
    if (profile == QSystemDeviceInfo::SilentProfile) {
        enableSounds(QVariant(false));
    }
    else {
        enableSounds(QVariant(true));
    }
}
#endif
