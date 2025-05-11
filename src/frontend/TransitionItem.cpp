#include "TransitionItem.hpp"
#include "StateItem.hpp"
#include "backend/transition.hpp"
#include <QPen>
#include <QPainter>
#include <qgraphicsscene.h>
#include <qline.h>
#include <qdebug.h>
#include <qpoint.h>
#include <qmath.h>

TransitionItem::TransitionItem(StateItem *startState, StateItem *endState, QGraphicsItem *parent, int offsetIndex)
    : QObject(), QGraphicsPathItem(parent), offsetIndex(offsetIndex) {

    //initing transition from backend class <- pseja's class
    transition = new Transition(startState->state, endState->state, "", "", -1, "");

    //setting color and axis position of transition line
    setPen(QPen(Qt::black, 2));
    setZValue(-10);
    setFlags(QGraphicsItem::ItemIsSelectable);
    //setAcceptHoverEvents(true);
    fromState = startState;
    toState = endState;
    QPointF p1 = startState->pos();
    QPointF p2 = endState->pos();

    if (fromState == toState) {
        QPointF p = fromState->pos();
        double baseAngle = 270.0;
        double angleStep = 30.0;
        double radius = 25.0 + 15.0 * offsetIndex;
        int maxTries = 12;
        double angle = baseAngle;
        QRectF ellipseRect;
        QPainterPath path;

        for (int i = 0; i < maxTries; ++i) {
            double rad = angle * M_PI / 180.0;
            double dx = cos(rad) * radius;
            double dy = sin(rad) * radius;
            QPointF center = p + QPointF(dx, dy);

            ellipseRect = QRectF(center.x() - radius, center.y() - radius, 2*radius, 2*radius);
            path = QPainterPath();
            path.arcMoveTo(ellipseRect, angle);
            path.arcTo(ellipseRect, angle, -360);

            
            bool collision = false;
            if (scene()) {
                for (QGraphicsItem* item : scene()->items(path.boundingRect())) {
                  if (item != this) {
                      auto other = dynamic_cast<TransitionItem*>(item);
                      if (other && other->path().intersects(path)) {
                          collision = true;
                          break;
                      }
                  }
                }
            }
            if (!collision) break;
            angle += angleStep;
        }

        setPath(path);
        label = new QGraphicsTextItem(transition->getCondition(), this);
        label->setDefaultTextColor(Qt::darkRed);
        label->setPos(ellipseRect.center().x(), ellipseRect.top() - 20);
        setZValue(-10 - offsetIndex);
    } else {


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

    label = new QGraphicsTextItem(transition->getCondition(), this);
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
            QPointF p = fromState->pos();
            double baseAngle = 270.0;
            double angleStep = 30.0;
            double radius = 25.0 + 15.0 * offsetIndex;
            int maxTries = 12;
            double angle = baseAngle;
            QRectF ellipseRect;
            QPainterPath path;

            for (int i = 0; i < maxTries; ++i) {
                double rad = angle * M_PI / 180.0;
                double dx = cos(rad) * radius;
                double dy = sin(rad) * radius;
                QPointF center = p + QPointF(dx, dy);

                ellipseRect = QRectF(center.x() - radius, center.y() - radius, 2*radius, 2*radius);
                path = QPainterPath();
                path.arcMoveTo(ellipseRect, angle);
                path.arcTo(ellipseRect, angle, -360);

                bool collision = false;
                if (scene()) {
                    for (QGraphicsItem* item : scene()->items(path.boundingRect())) {
                      if (item != this) {
                          auto other = dynamic_cast<TransitionItem*>(item);
                          if (other && other->path().intersects(path)) {
                              collision = true;
                              break;
                          }
                      }
                    }
                }
                if (!collision) break;
                angle += angleStep;
            }

            setPath(path);
            if (label) label->setPos(ellipseRect.center().x(), ellipseRect.top() - 20);
            setZValue(-10 - offsetIndex);
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

void TransitionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  QPen pen = this->pen();
  if (isSelected()) {
    pen.setColor(Qt::red);
    pen.setWidth(5);
  }
  painter->setPen(pen);
  painter->drawPath(path());

  if (fromState != toState) {
    QPainterPath p = path();
    qreal totalLength = p.length();
    qreal arrowDistance = 40.0;
    qreal percent = 1.0 - (arrowDistance / totalLength);
    if (percent < 0.0) percent = 0.0;

    QPointF arrowPos = p.pointAtPercent(percent);
    QPointF beforeArrow = p.pointAtPercent(percent - 0.01);

    QLineF line(beforeArrow, arrowPos);
    double angle = std::atan2(arrowPos.y() - beforeArrow.y(), arrowPos.x() - beforeArrow.x());

    double arrowSize = 18.0;
    QPointF arrowP1 = arrowPos + QPointF(std::cos(angle + M_PI / 7) * -arrowSize,
                                          std::sin(angle + M_PI / 7) * -arrowSize);
    QPointF arrowP2 = arrowPos + QPointF(std::cos(angle - M_PI / 7) * -arrowSize,
                                          std::sin(angle - M_PI / 7) * -arrowSize);

    QPolygonF arrowHead;
    arrowHead << arrowPos << arrowP1 << arrowP2;

    painter->setBrush(Qt::darkMagenta);
    painter->drawPolygon(arrowHead);
  }
}

void TransitionItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
    QMenu menu;
    QAction *deleteAction = menu.addAction("Delete Transition");

    QAction *selectedAction = menu.exec(event->screenPos());

    if (selectedAction == deleteAction) {
        emit transitionDeleted(this);
    }
}

/*
void TransitionItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    setZValue(10);
    QGraphicsPathItem::hoverEnterEvent(event);
}

void TransitionItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    setZValue(-20); // nebo původní hodnota
    QGraphicsPathItem::hoverLeaveEvent(event);
}
*/