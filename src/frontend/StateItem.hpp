#ifndef STATEITEM_HPP
#define STATEITEM_HPP

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <qglobal.h>
#include <qgraphicsitem.h>
#include <qobjectdefs.h>
#include <QObject>
#include "src/backend/state.hpp"

class StateItem : public QObject, public QGraphicsEllipseItem {
  Q_OBJECT
  
public:
    explicit StateItem(const QString &name, const QString &code = "", QGraphicsItem *parent = nullptr);
    State* state;
    //void setName(const QString &newName);
    //void setCodeSegment(const QString &newCode);
    void updateState(const QString &newName, const QString &newCode, bool initial);
    //QString getCodeSegment();
    //QString getName();

protected:
  void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

signals:
  void stateDeleted(StateItem *state);

private:
  QGraphicsTextItem *textItem;
};

#endif // STATEITEM_HPP
