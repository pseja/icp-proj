#ifndef STATEITEM_HPP
#define STATEITEM_HPP

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <qglobal.h>

class StateItem : public QGraphicsEllipseItem {
public:
    explicit StateItem(qreal x = 0, qreal y = 0, QGraphicsItem *parent = nullptr);

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
};

#endif // STATEITEM_HPP
