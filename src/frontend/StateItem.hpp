#ifndef STATEITEM_HPP
#define STATEITEM_HPP

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <qglobal.h>

class StateItem : public QGraphicsEllipseItem {
public:
    explicit StateItem(QGraphicsItem *parent = nullptr);

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
};

#endif // STATEITEM_HPP
