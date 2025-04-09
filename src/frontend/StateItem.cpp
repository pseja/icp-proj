#include "StateItem.hpp"
#include <QGraphicsScene>
#include <QDebug>
#include <qglobal.h>
#include <qgraphicsitem.h>
#include <qnamespace.h>
#include <qobject.h>
#include <QObject>

StateItem::StateItem(const QString &name, const QString &code, QGraphicsItem *parent)
    : QObject(), QGraphicsEllipseItem(-40, -40, 80, 80, parent) {
  setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
  setBrush(Qt::cyan);

  textItem = new QGraphicsTextItem(this);
  textItem->setDefaultTextColor(Qt::black);
  stateName = "State";
  textItem->setPlainText(stateName);
  

  QRectF bounds = boundingRect();
  textItem->setPos(bounds.center() - textItem->boundingRect().center());
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
        scene()->removeItem(this);
        delete this;
    }
}

void StateItem::setName(const QString &newName) { stateName = newName; textItem->setPlainText(newName);}
void StateItem::setCodeSegment(const QString &newCode) {
  codeSegment = newCode;
}

QString StateItem::getName() { return stateName; }
QString StateItem::getCodeSegment() {return codeSegment;}
