#include "StateItem.hpp"
//#include "src/backend/state.hpp"
#include <QGraphicsScene>
#include <QDebug>
#include <qglobal.h>
#include <qgraphicsitem.h>
#include <qgraphicssceneevent.h>
#include <qnamespace.h>
#include <qobject.h>
#include <QObject>

StateItem::StateItem(const QString &name, const QString &code, QGraphicsItem *parent)
    : QObject(), QGraphicsEllipseItem(-40, -40, 80, 80, parent) {
  setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
  setBrush(Qt::cyan);

  state = new State(name);
  state->setCode(code);
  textItem = new QGraphicsTextItem(this);
  textItem->setDefaultTextColor(Qt::black);
  //state->setName("State");
  //stateName = "State";
  textItem->setPlainText(state->getName());
  QRectF ell = boundingRect();
  QRectF text = textItem->boundingRect();
  qreal x = ell.center().x() - text.width() / 2;
  qreal y = ell.center().y() - text.height() / 2;
  textItem->setPos(x, y);

  //QRectF bounds = boundingRect();
  //textItem->setPos(bounds.center() - textItem->boundingRect().center());
}

void StateItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
    QMenu menu;
    QAction *renameAction = menu.addAction("Přejmenovat");
    QAction *deleteAction = menu.addAction("Smazat");

    QAction *selectedAction = menu.exec(event->screenPos());

    if (selectedAction == renameAction) {
        qDebug() << "Přejmenovat stav!";
    } else if (selectedAction == deleteAction) {
      qDebug() << "Mazání stavu!";
      emit stateDeleted(this);
        //scene()->removeItem(this);
        //delete this;
    }
}

void StateItem::updateState(const QString &newName, const QString &newCode, bool initial) {
  state->setName(newName);
  textItem->setPlainText(newName);
  state->setCode(newCode);
  state->setInitial(initial);

  textItem->setPlainText(state->getName());
  QRectF ell = boundingRect();
  QRectF text = textItem->boundingRect();
  qreal x = ell.center().x() - text.width() / 2;
  qreal y = ell.center().y() - text.height() / 2;
  textItem->setPos(x, y);
}
//QString StateItem::getName() { return stateName; }
//QString StateItem::getCodeSegment() {return codeSegment;}

void StateItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsEllipseItem::mouseMoveEvent(event);
  emit positionChanged();
  qDebug() << "debugging -> draging onto: " << event->pos();
}