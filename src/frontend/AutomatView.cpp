/**
 * @file AutomatView.cpp
 * @brief Implements the AutomatView class and its methods, which provide
 *  a graphical view for the FSM editing.
 *
 * @author Vaclav Sovak (xsovakv00)
 * @date 11-05-2025
 */

#include "AutomatView.hpp"
#include "mainwindow.hpp"


AutomatView::AutomatView(FSM *fsm, QWidget *parent) : QGraphicsView(parent) {
  //minumum size for automat view and init of scene
  setMinimumSize(400, 300); 
  setScene(new QGraphicsScene(this));
  setRenderHint(QPainter::Antialiasing);

  //setting options for future overrides
  setDragMode(QGraphicsView::NoDrag);
  scene()->setItemIndexMethod(QGraphicsScene::NoIndex);
  scene()->setSceneRect(rect());
  scene()->installEventFilter(this);
  
  //connecting signal to handle whatever is selected
  connect(scene(), &QGraphicsScene::selectionChanged, this, &AutomatView::onSelection);
}

//simple event for dis-selecting items and showing info about FSM
void AutomatView::onSelection() {
  QList<QGraphicsItem *> selectedItems = scene()->selectedItems();
  if (selectedItems.isEmpty()) {
    emit showFSMInfo();
  }
}

//overriding mouse press event to handle clicks on states and transitions
void AutomatView::mousePressEvent(QMouseEvent *event) {
  QGraphicsView::mousePressEvent(event);

  //deciding if the click should be registered as an end of transition
  if (transitionStart && templine) {
    QPointF scenePos = mapToScene(event->pos());
    QList<QGraphicsItem *> clicked = scene()->items(scenePos);

    //going through all clicked items
    for (QGraphicsItem *item : clicked) {
      StateItem *endstate = dynamic_cast<StateItem *>(item);
      if (endstate) {
        int count = 0;
        for (QGraphicsItem *other : scene()->items()) {
          TransitionItem *ti = dynamic_cast<TransitionItem *>(other);
          if (ti && ti->getFrom() == transitionStart && ti->getTo() == endstate) {
            ++count;
          }
        }
        qDebug() << "creating transition";
        //create transition
        TransitionItem *transition =
            new TransitionItem(transitionStart, endstate, nullptr, count);

        qDebug() << "transition created";
        scene()->addItem(transition); // add transition to scene
        emit addTransition(transition); // emit signal to add transition to FSM
        break;
      }
    }
    //remove temporary line
    scene()->removeItem(templine);
    delete templine;
    templine = nullptr;
    transitionStart = nullptr;
  }

  QList<QGraphicsItem *> clicked = scene()->selectedItems();
  //selection of state or transition
  for (QGraphicsItem *item : clicked) {
    if (typeid(*item) == typeid(StateItem)) {
        //scene()->clearSelection();
      
        //item->setSelected(true);
        StateItem *state = dynamic_cast<StateItem *>(item);
        emit stateSelected(state);
        break;
    } else if (typeid(*item) == typeid(TransitionItem)) {
      TransitionItem *transition = dynamic_cast<TransitionItem *>(item);
      emit transitionSelected(transition);
      qDebug() << "Transition clicked!";
      qDebug() << "Transition from" << transition->getFrom()->state->getName()
               << "to" << transition->getTo()->state->getName();
      qDebug() << "Event:" << transition->transition->getEvent();
      qDebug() << "Condition:" << transition->transition->getCondition();
      qDebug() << "Delay:" << transition->transition->getDelay();
      qDebug() << "Delay Variable:" << transition->transition->getDelayVariableName();
      qDebug() << "Transition clicked END!";
    }
  }
}

void AutomatView::mouseDoubleClickEvent(QMouseEvent *event) {

  QPointF scenePos = mapToScene(event->pos());

  //begginging of transition line drawing
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
  

  //control of overlapping states for creating purposes
  for (QGraphicsItem *item : scene()->items()) {
    if (typeid(*item) == typeid(StateItem)) {
        if (item->boundingRect().translated(item->pos()).intersects(
          QRectF(scenePos.x() - 40, scenePos.y() - 40, 80, 80))) {
          return;
        }
    }
  }

  qDebug() << "clicked";
  StateItem tempState(nullptr);
  tempState.setPos(scenePos);
  qDebug() << "added state to fsm";
  //fsm->addState(tempState->state, "State");
  
  //is is overlapping with any other state
  for (QGraphicsItem *item : scene()->items()) {
    if (typeid(*item) == typeid(StateItem) && item->collidesWithItem(&tempState)) {
      return;
    }
  }

  //creating new state with unique name
  QString stateName = QString("State%1").arg(stateCounter++);
  StateItem *state = new StateItem(stateName);
  state->setBrush(Qt::cyan);
  state->setPos(scenePos);
  state->setFlags(QGraphicsItem::ItemIsSelectable |
                  QGraphicsItem::ItemIsMovable);
  scene()->addItem(state);
  emit addState(state);


  //couldnt find a better place to put it in, registering signal for deletion
  MainWindow *mainWin = qobject_cast<MainWindow *>(window());
  if (mainWin) {
    connect(state, &StateItem::stateDeleted, mainWin, &MainWindow::handleStateDeleted);
  }
  //state->setSelected(true);
}

//overriding mouse move event to handle line drawing
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
