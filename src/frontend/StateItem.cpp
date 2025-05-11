/**
 * @file StateItem.cpp
 * @brief Implements the StateItem class and its methods, which represent
 * a state in the FSM editor.
 *
 * @author Vaclav Sovak (xsovakv00)
 * @date 11-05-2025
 */

#include "StateItem.hpp"


StateItem::StateItem(const QString &name, const QString &code, QGraphicsItem *parent)
    : QObject(), QGraphicsEllipseItem(-40, -40, 80, 80, parent) {
  //object has to be movable and selectable
  setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
  setBrush(Qt::cyan); //we chose cyan color for the state

  //create a new state object, which is part of the graphical item
  state = new State(name);
  state->setCode(code); //setting code segment
  textItem = new QGraphicsTextItem(this); //some label
  textItem->setDefaultTextColor(Qt::black);

  //setting the name of the state
  textItem->setPlainText(state->getName());
  QRectF ell = boundingRect();
  QRectF text = textItem->boundingRect();
  qreal x = ell.center().x() - text.width() / 2; //centering the text
  qreal y = ell.center().y() - text.height() / 2;
  textItem->setPos(x, y);

}

//overriding menu method to display the context menu
//when right click is pressed
void StateItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
    QMenu menu;
    QAction *deleteAction = menu.addAction("Delete State");

    QAction *selectedAction = menu.exec(event->screenPos());

    if (selectedAction == deleteAction) {
      qDebug() << "State delete!";
      emit stateDeleted(this); //sending signal to delete the state
    }
}

//updating the state with provided information a creating glow for initial state
void StateItem::updateState(const QString &newName, const QString &newCode, bool initial) {
  state->setName(newName);
  textItem->setPlainText(newName);
  state->setCode(newCode);
  state->setInitial(initial);

  if (initial) {
    setPen(QPen(QColor(160, 32, 240), 5)); // purple color for initial state
  } else {
    setPen(QPen(Qt::black, 2));
  }

  //updating name of the state, centering it
  textItem->setPlainText(state->getName());
  QRectF ell = boundingRect();
  QRectF text = textItem->boundingRect();
  qreal x = ell.center().x() - text.width() / 2;
  qreal y = ell.center().y() - text.height() / 2;
  textItem->setPos(x, y);
}

//overriding mouse move event to send signal when the state is moved
void StateItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsEllipseItem::mouseMoveEvent(event);
  emit positionChanged(); //emiting signal when the state is moved, to recalculate text
  //qDebug() << "debugging -> draging onto: " << event->pos();
}