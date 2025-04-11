#include "TransitionItem.hpp"
#include "src/frontend/StateItem.hpp"
#include <QPen>
#include <QPainter>
#include <qline.h>
#include <qdebug.h>

TransitionItem::TransitionItem(StateItem *startState, StateItem *endState, QGraphicsItem *parent)
    : QObject(), QGraphicsLineItem(parent) {

    //initing transition from backend class <- pseja's class
    transition = new Transition(startState->state, endState->state, "Transition", "", -1, "");

    //setting color and axis position of transition line
    setPen(QPen(Qt::black, 2));
    setZValue(-1);

    //asigning state to transition, will be used for search, then labeling the transition
    fromState = startState;
    toState = endState;
    label = new QGraphicsTextItem("Transition", this);
    label->setDefaultTextColor(Qt::darkRed);
    label->setPos((fromState->pos() + toState->pos()) / 2);
    
    //we call drawing(i.e. updateposition) for the first time
    updatePosition();
    qDebug() << "possition updated";
    
    //connection needed for redrawing position when one of the states moves
    connect(fromState, &StateItem::positionChanged, this,
            &TransitionItem::updatePosition);
    connect(toState, &StateItem::positionChanged, this,
            &TransitionItem::updatePosition);
}

void TransitionItem::updatePosition() {
  //we can change position only when pointer to state exists
  if (fromState && toState) {
    //we recalculate and put the line according to new position
    setLine(QLineF(fromState->pos(), toState->pos()));
    //redraw label if one exists
    if (label) {
      label->setPos((fromState->pos() + toState->pos()) / 2);
    }
  }
}

StateItem* TransitionItem::getFrom() {
  return fromState;
}

StateItem* TransitionItem::getTo() {
  return toState;
}
