#ifndef STATEITEM_HPP
#define STATEITEM_HPP

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <qglobal.h>
#include <qgraphicsitem.h>
#include <qgraphicssceneevent.h>
#include <qobjectdefs.h>
#include <QObject>
#include <qvariant.h>
#include "src/backend/state.hpp"
#include "src/backend/state.hpp"

class StateItem : public QObject, public QGraphicsEllipseItem {
  Q_OBJECT
public:
    explicit StateItem(const QString &name, const QString &code = "", QGraphicsItem *parent = nullptr);
    State* state;
    //void setName(const QString &newName);
    //void setCodeSegment(const QString &newCode);
    void updateState(const QString &newName, const QString &newCode,
                     bool initial);
    //QString getCodeSegment();
    //QString getName();
      
protected:
  void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  //QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

signals:
  void stateDeleted(StateItem *state);
  void positionChanged();

private:
  QGraphicsTextItem *textItem;
};

#endif // STATEITEM_HPP
