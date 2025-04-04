#include "AutomatView.hpp"
#include "StateItem.hpp"
#include <asm-generic/errno.h>
#include <qdebug.h>
#include <qevent.h>
#include <qglobal.h>
#include <qgraphicsitem.h>
#include <qgraphicsscene.h>
#include <qgraphicssceneevent.h>
#include <qlist.h>
#include <qnamespace.h>
#include <qpoint.h>
#include <typeinfo>

// implementace automat platna
AutomatView::AutomatView(QWidget *parent) : QGraphicsView(parent) {
  setMinimumSize(400, 300); // Nastav minimální velikost
  setScene(new QGraphicsScene(this));
  setRenderHint(QPainter::Antialiasing);
}

void AutomatView::mousePressEvent(QMouseEvent *event) {
  QPointF scenePos = mapToScene(event->pos());
  QList<QGraphicsItem *> clicked = scene()->items(scenePos);

  for (QGraphicsItem *item : clicked) {
    if (typeid(*item) == typeid(StateItem)) {
      if (item->isSelected()) {
        item->setSelected(false);
      }
      else {
        item->setSelected(true);
        StateItem *state = dynamic_cast<StateItem *>(item);
        emit stateSelected(state);
      }
      return;
    }
  }
}

void AutomatView::mouseDoubleClickEvent(QMouseEvent *event){
  QPointF scenePos = mapToScene(event->pos());
  qDebug() << "Kliknuto na souřadnice:" << scenePos;

  // Vytvoříme dočasný state jen pro kontrolu kolizí
  StateItem tempState(nullptr);
  tempState.setPos(scenePos);

  // Zjistíme, jestli by se překrýval s jinými objekty
  for (QGraphicsItem *item : scene()->items()) {
    if (typeid(*item) == typeid(StateItem) && item->collidesWithItem(&tempState)) {
      return;
    }
  }
  
  StateItem *state = new StateItem(nullptr);
  state->setBrush(Qt::cyan);
  state->setPos(scenePos);
  state->setFlags(QGraphicsItem::ItemIsSelectable |
                  QGraphicsItem::ItemIsMovable);
  scene()->addItem(state);
  //state->setSelected(true);
}