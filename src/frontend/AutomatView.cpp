#include "AutomatView.hpp"
#include <qglobal.h>
#include <qgraphicsitem.h>
#include <qgraphicsscene.h>
#include <qgraphicssceneevent.h>
#include <qnamespace.h>

// State item implementace
StateItem::StateItem(qreal x, qreal y) : QGraphicsEllipseItem(0, 0, 50, 50) {
  setPos(x, y);
  setBrush(Qt::cyan);
  setFlags(ItemIsMovable | ItemIsSelectable);
}

void StateItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  setBrush(Qt::yellow);
  QGraphicsEllipseItem::mousePressEvent(event);
}

void StateItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsEllipseItem::mouseMoveEvent(event);
}

// implementace automat platna
AutomatView::AutomatView(QWidget *parent) : QGraphicsView(parent) {
  scene = new QGraphicsScene(this);
  setMinimumSize(400, 300); // Nastav minimální velikost
  setScene(scene);
  setRenderHint(QPainter::Antialiasing);
}

void AutomatView::addState(qreal x, qreal y) {
  scene->addItem(new StateItem(x,y));
}