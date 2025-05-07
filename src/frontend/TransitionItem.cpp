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
    setZValue(-10);
    fromState = startState;
    toState = endState;

    if (fromState == toState) {
      QPointF p = fromState->pos();
      double radius = 30.0 + 20.0 * offsetIndex;
      QRectF ellipseRect(p.x() - radius, p.y() - 2*radius, 2*radius, 2*radius);
      QPainterPath path;
      path.arcMoveTo(ellipseRect, 225);
      path.arcTo(ellipseRect, 225, -270);
      setPath(path);

      label = new QGraphicsTextItem(QString("Transition %1").arg(offsetIndex), this);
      label->setDefaultTextColor(Qt::darkRed);
      label->setPos(p.x(), p.y() - 2 * radius - 20);
    } else {
    QPointF p1 = startState->pos();
    QPointF p2 = endState->pos();

    QLineF line(p1, p2);
    QPointF normal(-line.dy(), line.dx());
    if (line.length() != 0) {
        normal /= line.length();
    }

    double offset = 40.0 * (offsetIndex - 0.5);
    QPointF mid = (p1 + p2) / 2 + normal * offset;

    QPainterPath path(p1);
    path.quadTo(mid, p2);
    setPath(path);


    //asigning state to transition, will be used for search, then labeling the transition

    label = new QGraphicsTextItem(QString("Transition %1").arg(offsetIndex), this);
    label->setDefaultTextColor(Qt::darkRed);
    label->setPos(mid);
  }    
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

    if (fromState == toState) {
      // Smyčka: vykresli oblouk nad stavem, každý další offsetIndex posune smyčku výš
      double radius = 30.0 + 20.0 * offsetIndex;
      QRectF ellipseRect(p1.x() - radius, p1.y() - 2*radius, 2*radius, 2*radius);
      QPainterPath path;
      path.arcMoveTo(ellipseRect, 225);
      path.arcTo(ellipseRect, 225, -270);
      setPath(path);

      if (label) {
        label->setPos(p1.x(), p1.y() - 2*radius - 20);
      }
      return;
    }


    QLineF line(p1, p2);
    QPointF normal(-line.dy(), line.dx());
    if (line.length() != 0)
        normal /= line.length();

    double offset = 40.0 * (offsetIndex - 0.5); // offsetIndex musí být member proměnná!
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
