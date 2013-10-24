/*
 * Copyright (c) 2011 Nokia Corporation.
 */

#ifndef KEYEVENTLISTENER_H
#define KEYEVENTLISTENER_H

#include <QObject>
#include <QKeyEvent>

class MyEventFilter : public QObject
{
    Q_OBJECT

public:
    MyEventFilter(QObject *parent = 0);
    ~MyEventFilter();

protected:
     bool eventFilter(QObject *obj, QEvent *event);

signals:
    void activationChangeFiltered();

};

#endif // KEYEVENTLISTENER_H
