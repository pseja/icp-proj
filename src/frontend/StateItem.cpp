#include "StateItem.hpp"
#include <QGraphicsScene>
#include <QDebug>
#include <qglobal.h>

StateItem::StateItem(const QString &name, const QString &code, QGraphicsItem *parent)
    : QGraphicsEllipseItem(-40, -40, 80, 80, parent) {
  setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
  
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
        scene()->removeItem(this);
        delete this;
    }
}

void StateItem::setName(const QString &newName) { stateName = newName; }
void StateItem::setCodeSegment(const QString &newCode) {
  codeSegment = newCode;
}

QString StateItem::getName() { return stateName; }
QString StateItem::getCodeSegment() {return codeSegment;}
