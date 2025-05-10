#ifndef TRANSITIONITEM_HPP
#define TRANSITIONITEM_HPP

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <qglobal.h>
#include <qgraphicsitem.h>
#include <qobjectdefs.h>
#include <QObject>
#include <qpen.h>
#include "backend/transition.hpp"
#include "StateItem.hpp"
#include <QTimer>

class TransitionItem : public QObject, public QGraphicsPathItem {
  Q_OBJECT
  
public:
    explicit TransitionItem(StateItem *fromState, StateItem *toState, QGraphicsItem *parent = nullptr, int offset = 0);
    Transition *transition;
    void updatePosition();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    //void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    //void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    StateItem* getFrom();
    StateItem *getTo();
    QString getLabel() const { return label->toPlainText(); }
    void setLabel(const QString &text) { label->setPlainText(text); }
    
    void startBlinking() {
      if (!blinking) {
        blink = new QTimer(this);
        connect(blink, &QTimer::timeout, this, &TransitionItem::setBlinking);
        blinking = false;
      }
      blink->start(300);
    }
    void stopBlinking() {
      if (blink) blink->stop();
      setPen(QPen(Qt::black, 2));
    }

protected:
//  void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

signals:
//  void stateDeleted(StateItem *state);

private:
  StateItem *fromState;
  StateItem *toState;
  QGraphicsTextItem *label;
  int offsetIndex;
  QTimer *blink = nullptr;
  bool blinking = false;
  void setBlinking() {
    blinking = !blinking;
    setPen(QPen(blinking ? Qt::green : Qt::black, 2));
  }
};

#endif // STATEITEM_HPP