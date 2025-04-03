#include "StateItem.hpp"
#include <QGraphicsScene>
#include <QDebug>
#include <qglobal.h>

StateItem::StateItem(QGraphicsItem *parent)
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