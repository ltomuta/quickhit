/**
 * Copyright (c) 2011 Nokia Corporation.
 */

#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QDeclarativeItem>
#include <QList>
#include <QMutex>
#include <QObject>

#ifdef Q_OS_SYMBIAN
    #include <hwrmvibra.h>
#endif

// Qt Mobility
#if defined (Q_OS_SYMBIAN) || defined(Q_WS_MAEMO_5) || defined (Q_WS_MAEMO_6)
    #include <QSystemDeviceInfo>
    QTM_USE_NAMESPACE
#endif

class InvSounds;
class LevelPluginInterface;


class GameEngine : public QObject
{
    Q_OBJECT

public:
    explicit GameEngine(QObject *parent = 0);
    ~GameEngine();

public:
    void setGameQml(QObject *obj);
    void setGameLevel(LevelPluginInterface *level);
    void addPlugin(LevelPluginInterface *pluginInterface);
    void foreground();

    // Accessible from QML code
    Q_INVOKABLE QVariant randInt(QVariant low, QVariant high);
    Q_INVOKABLE QVariant pluginList();
    Q_INVOKABLE void gameStartSound();
    Q_INVOKABLE QVariant isHarmattan();
    Q_INVOKABLE QVariant isSymbian();
    Q_INVOKABLE QVariant isMaemo();
    Q_INVOKABLE QVariant isWindows();

public slots:
    void findQmlObjects();
    void clearQmlObjects();
    void clearPluginList();
    void enableEngineTimer(QVariant enable);
    void enableSounds(QVariant enable);
    void pauseGame();
    void pauseLevel(QVariant doPause);
    void playSound(QVariant index);
    void playSounds(QVariant index, QVariant count);
    void playInternalSound(QVariant index);
    void playInternalSounds(QVariant index, QVariant count);
    void vibra();
    void fastVibra();
    void openLink(QVariant link);

protected: // From QObject
    void timerEvent(QTimerEvent *event);

private:
    void findMissiles(QObject *rootObject);
    void findEnemies(QObject *rootObject);
    void setLevelQml(QObject *obj);
    void setMyShipQml(QObject *obj);
    void setEnemiesGridQml(QObject *obj);
    void gameOver(bool youWin);
    void selectVisibleEnemy(int &start, int &end);
    void doHitTest();
    void doEnemyMissile();

private slots:
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_MAEMO_5) || defined (Q_WS_MAEMO_6)
    void currentProfileChanged(QSystemDeviceInfo::Profile profile);
#endif

private: // Data
    QObject *m_levelQml;
    QDeclarativeItem *m_gameQml;
    QDeclarativeItem *m_myShipQml;
    QDeclarativeItem *m_enemiesGridQml;
    LevelPluginInterface *m_gameLevel;
    InvSounds *m_soundEngine;
    QList<QDeclarativeItem*> m_enemyList;
    QList<QDeclarativeItem*> m_missileList;
    QList<QDeclarativeItem*> m_enemyMissileList;
    QList<QObject*> m_pluginList;
    QMutex m_enemyListMutex;
    bool m_enableEnemyMissiles;
    int m_doEnemyMissile;
    int m_timerId;
    bool m_silent;

#if defined Q_OS_SYMBIAN || defined Q_WS_MAEMO_5 || defined (Q_WS_MAEMO_6)
    QSystemDeviceInfo *m_systemDeviceInfo;
#endif

#ifdef Q_OS_SYMBIAN
    CHWRMVibra* iVibrate;
#endif
};

#endif // GAMEENGINE_H
