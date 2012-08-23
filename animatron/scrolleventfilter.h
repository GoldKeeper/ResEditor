#ifndef SCROLLEVENTFILTER_H
#define SCROLLEVENTFILTER_H

#include <QObject>
#include <QEvent>
#include <QWheelEvent>
#include <QMessageBox>

class ScrollEventFilter : public QObject
{
    Q_OBJECT

public:
    ScrollEventFilter(QObject * parent):QObject(parent){}
signals:
    void scrolled(int);

protected:
    bool eventFilter(QObject *obj, QEvent *event)
    {
        if (event->type() == QEvent::Wheel)
        {
            QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);
            event->ignore();
            emit scrolled(wheelEvent->delta());
            return true;
        }
        else
        {
            // standard event processing
            return QObject::eventFilter(obj, event);
        }
    }
};


#endif // SCROLLEVENTFILTER_H
