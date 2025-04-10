#include "AutomatView.hpp"
#include "StateItem.hpp"
#include <algorithm>
#include <asm-generic/errno.h>
#include <qdebug.h>
#include <qevent.h>
#include <qglobal.h>
#include <qgraphicsitem.h>
#include <qgraphicsscene.h>
#include <qgraphicssceneevent.h>
#include <qgraphicsview.h>
#include <qlist.h>
#include <qnamespace.h>
#include <qpoint.h>
#include <typeinfo>
#include "mainwindow.hpp"

// implementace automat platna
AutomatView::AutomatView(QWidget *parent) : QGraphicsView(parent) {
  setMinimumSize(400, 300); // Nastav minimální velikost
  setScene(new QGraphicsScene(this));
  setRenderHint(QPainter::Antialiasing);
  setDragMode(QGraphicsView::RubberBandDrag);
}


void AutomatView::mousePressEvent(QMouseEvent *event) {
  QGraphicsView::mousePressEvent(event);
  
  //QPointF scenePos = mapToScene(event->pos());
  QList<QGraphicsItem *> clicked = scene()->selectedItems();

  for (QGraphicsItem *item : clicked) {
    if (typeid(*item) == typeid(StateItem)) {
        //scene()->clearSelection();
      
        //item->setSelected(true);
        StateItem *state = dynamic_cast<StateItem *>(item);
        emit stateSelected(state);
        break;
    }
  }
}


void AutomatView::mouseDoubleClickEvent(QMouseEvent *event){
  QPointF scenePos = mapToScene(event->pos());
  qDebug() << "Kliknuto na souřadnice:" << scenePos;
  qDebug() << "got possion\n";
  // Vytvoříme dočasný state jen pro kontrolu kolizí
  StateItem tempState(nullptr);
  tempState.setPos(scenePos);
  
  // Zjistíme, jestli by se překrýval s jinými objekty
  for (QGraphicsItem *item : scene()->items()) {
    if (typeid(*item) == typeid(StateItem) && item->collidesWithItem(&tempState)) {
      return;
    }
  }
  
  qDebug() << "if it doesnt collide\n";
  StateItem *state = new StateItem(nullptr);
  state->setBrush(Qt::cyan);
  state->setPos(scenePos);
  state->setFlags(QGraphicsItem::ItemIsSelectable |
                  QGraphicsItem::ItemIsMovable);
  scene()->addItem(state);
  emit addState(state);


  //couldnt find a better place to put it in
  MainWindow *mainWin = qobject_cast<MainWindow *>(window());
  if (mainWin) {
    connect(state, &StateItem::stateDeleted, mainWin, &MainWindow::handleStateDeleted);
  }
  //state->setSelected(true);
}

void AutomatView::mouseMoveEvent(QMouseEvent *event) {
  QGraphicsView::mouseMoveEvent(event);
}

void AutomatView::mouseReleaseEvent(QMouseEvent *event) {
  QGraphicsView::mouseReleaseEvent(event);
}

