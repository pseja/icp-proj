#include "AutomatView.hpp"
#include "StateItem.hpp"
#include <algorithm>
#include <asm-generic/errno.h>
#include <linux/limits.h>
#include <qdebug.h>
#include <qevent.h>
#include <qglobal.h>
#include <qgraphicsitem.h>
#include <qgraphicsscene.h>
#include <qgraphicssceneevent.h>
#include <qgraphicsview.h>
#include <qline.h>
#include <qlist.h>
#include <qnamespace.h>
#include <qpen.h>
#include <qpoint.h>
#include <typeinfo>
#include "mainwindow.hpp"
#include "TransitionItem.hpp"

// implementace automat platna
AutomatView::AutomatView(QWidget *parent) : QGraphicsView(parent) {
  setMinimumSize(400, 300); // Nastav minimální velikost
  setScene(new QGraphicsScene(this));
  setRenderHint(QPainter::Antialiasing);
  setDragMode(QGraphicsView::RubberBandDrag);
  scene()->setItemIndexMethod(QGraphicsScene::NoIndex);
  scene()->setSceneRect(rect()); // důležité: nastav scény velikost podle view
  scene()->installEventFilter(this);  // pro ladění pohybů, pokud chceš
  //setAcceptHoverEvents(true);
}


void AutomatView::mousePressEvent(QMouseEvent *event) {
  QGraphicsView::mousePressEvent(event);

  if (transitionStart && templine) {
    QPointF scenePos = mapToScene(event->pos());
    QList<QGraphicsItem *> clicked = scene()->items(scenePos);

    for (QGraphicsItem *item : clicked) {
      StateItem *endstate = dynamic_cast<StateItem *>(item);
      if (endstate && endstate != transitionStart) {
        qDebug() << "creating transition";
        TransitionItem *transition = new TransitionItem(transitionStart, endstate, nullptr);
        qDebug() << "transition created";
        scene()->addItem(transition);
        break;
      }
    }
    scene()->removeItem(templine);
    delete templine;
    templine = nullptr;
    transitionStart = nullptr;
  }

  
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

void AutomatView::mouseDoubleClickEvent(QMouseEvent *event) {

  //auto ellipse = scene()->addEllipse(-20, -20, 40, 40, QPen(Qt::red), QBrush(Qt::yellow));
  //ellipse->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

  
  QPointF scenePos = mapToScene(event->pos());
  qDebug() << "Kliknuto na souřadnice:" << scenePos;
  qDebug() << "got possion\n";
  // Vytvoříme dočasný state jen pro kontrolu kolizí

  QList<QGraphicsItem *> clicked = scene()->selectedItems();
  for (QGraphicsItem *item : clicked) {
    if (typeid(*item) == typeid(StateItem)) {
      StateItem *state = dynamic_cast<StateItem *>(item);
      transitionStart = state;

      templine = scene()->addLine(QLineF(scenePos, scenePos),
                                  QPen(Qt::black, 2, Qt::DashLine));
      return;
    }

  }

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
  if (templine) {
    QPointF scenePos = mapToScene(event->pos());
    QLineF newLine(templine->line().p1(), scenePos);
    templine->setLine(newLine);
  }
  
  QGraphicsView::mouseMoveEvent(event);
}

void AutomatView::mouseReleaseEvent(QMouseEvent *event) {
  QGraphicsView::mouseReleaseEvent(event);
}

