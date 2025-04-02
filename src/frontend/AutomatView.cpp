#include "AutomatView.hpp"
#include "StateItem.hpp"
#include <qevent.h>
#include <qglobal.h>
#include <qgraphicsitem.h>
#include <qgraphicsscene.h>
#include <qgraphicssceneevent.h>
#include <qnamespace.h>
#include <qpoint.h>

// implementace automat platna
AutomatView::AutomatView(QWidget *parent) : QGraphicsView(parent) {
  scene = new QGraphicsScene(this);
  setMinimumSize(400, 300); // Nastav minimální velikost
  setScene(scene);
  setRenderHint(QPainter::Antialiasing);
}

void AutomatView::mousePressEvent(QMouseEvent *event) {

}

void AutomatView::mouseDoubleClickEvent(QMouseEvent *event){
  QPointF scenePos = mapToScene(event->pos());
  qDebug() << "Kliknuto na souřadnice:" << scenePos;

  StateItem *state = new StateItem(100, 200, nullptr);
  state->setBrush(Qt::cyan);
  state->setPos(scenePos);

  scene->addItem(state);
}