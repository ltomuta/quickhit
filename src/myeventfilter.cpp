/**
 * Copyright (c) 2011-2014 Microsoft Mobile.
 */

#include "myeventfilter.h"
#include <QDebug>


/*!
  \class MyEventFilter
  \brief Custom event filter.
*/


/*!
  Constructor.
*/
MyEventFilter::MyEventFilter(QObject *parent) :
    QObject(parent)
{
}


/*!
  Destructor.
*/
MyEventFilter::~MyEventFilter()
{
}


/*!
  Catches events. If \a event is of type QEvent::ActivationChange, the method
  will emit activationChangeFiltered() event. Otherwise the event is
  propagated.
*/
bool MyEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    // http://doc.trolltech.com/4.7/qevent.html#QEvent
    // QEvent::ActivationChange : The application has been made available to the user.

    if (event->type() == QEvent::ActivationChange) {
        emit activationChangeFiltered();
        return false;
    }

    // Let the event propagate i.e. standard event processing.
    return QObject::eventFilter(obj, event);
}
