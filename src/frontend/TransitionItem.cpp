#include "TransitionItem.hpp"
#include <QPen>
#include <QPainter>
#include <qline.h>
#include <qdebug.h>

TransitionItem::TransitionItem(StateItem *startState, StateItem *endState, QGraphicsItem *parent)
    : QObject(), QGraphicsLineItem(parent) {

    setPen(QPen(Qt::black, 2));
    setZValue(-1); // aby šipka byla pod stavy
    fromState = startState;
    toState = endState;
    qDebug() << "updating possition";
    // Linka mezi pozicemi stavů
    updatePosition();
    qDebug() << "possition updated";
    // Volitelné: zobrazit název/kód přechodu
    label = new QGraphicsTextItem("Transition");
    label->setDefaultTextColor(Qt::darkRed);
    label->setPos((fromState->pos() + toState->pos()) / 2); // uprostřed čáry
}

void TransitionItem::updatePosition() {
  if (fromState && toState) {
    setLine(QLineF(fromState->pos(), toState->pos()));
    qDebug() << "line set";
    if (label) {
      label->setPos((fromState->pos() + toState->pos()) / 2);
      qDebug() << "label set";
    }
    qDebug() << "label not set";
  }
}

