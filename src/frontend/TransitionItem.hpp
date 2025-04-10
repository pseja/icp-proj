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
#include "src/backend/transition.hpp"
#include "StateItem.hpp"

class TransitionItem : public QObject, public QGraphicsLineItem {
  Q_OBJECT
  
public:
    explicit TransitionItem(StateItem *fromState, StateItem *toState, QGraphicsItem *parent = nullptr);
    Transition *transition;
    void updatePosition();

protected:
//  void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

signals:
//  void stateDeleted(StateItem *state);

private:
  StateItem *fromState;
  StateItem *toState;
  QGraphicsTextItem *label;
};

#endif // STATEITEM_HPP