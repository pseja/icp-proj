#include "TransitionItem.hpp"
#include "src/frontend/StateItem.hpp"
#include <QPen>
#include <QPainter>
#include <qline.h>
#include <qdebug.h>
#include <qpoint.h>

TransitionItem::TransitionItem(StateItem *startState, StateItem *endState, QGraphicsItem *parent, int offsetIndex)
    : QObject(), QGraphicsPathItem(parent), offsetIndex(offsetIndex) {

    //initing transition from backend class <- pseja's class
    transition = new Transition(startState->state, endState->state, "Transition", "", -1, "");

    //setting color and axis position of transition line
    setPen(QPen(Qt::black, 2));
    setZValue(-1);


    QPointF p1 = startState->pos();
    QPointF p2 = endState->pos();

    QLineF line(p1, p2);
    QPointF normal(-line.dy(), line.dx());
    if (line.length() != 0) {
        normal /= line.length();
    }

    double offset = 80.0 * offsetIndex;
    QPointF mid = (p1 + p2) / 2 + normal * offset;

    QPainterPath path(p1);
    path.quadTo(mid, p2);
    setPath(path);


    //asigning state to transition, will be used for search, then labeling the transition
    fromState = startState;
    toState = endState;
    label = new QGraphicsTextItem("Transition", this);
    label->setDefaultTextColor(Qt::darkRed);
    label->setPos(mid);
    
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
    /*
    //we recalculate and put the line according to new position
    setLine(QLineF(fromState->pos(), toState->pos()));
    //redraw label if one exists
    if (label) {
      label->setPos((fromState->pos() + toState->pos()) / 2);
    }
    */
    QPointF p1 = fromState->pos();
    QPointF p2 = toState->pos();

    QLineF line(p1, p2);
    QPointF normal(-line.dy(), line.dx());
    if (line.length() != 0)
        normal /= line.length();

    double offset = 80.0 * offsetIndex; // offsetIndex musí být member proměnná!
    QPointF mid = (p1 + p2) / 2 + normal * offset;

    QPainterPath path(p1);
    path.quadTo(mid, p2);
    setPath(path);

    if (label) {
        label->setPos(mid);
    }
  }
}

StateItem* TransitionItem::getFrom() {
  return fromState;
}

StateItem* TransitionItem::getTo() {
  return toState;
}
