#ifndef TRANSITIONITEM_HPP
#define TRANSITIONITEM_HPP

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <qglobal.h>
#include <qgraphicsitem.h>
#include <qobjectdefs.h>
#include <QObject>
#include "src/backend/transition.hpp"
#include "StateItem.hpp"

class TransitionItem : public QObject, public QGraphicsPathItem {
  Q_OBJECT
  
public:
    explicit TransitionItem(StateItem *fromState, StateItem *toState, QGraphicsItem *parent = nullptr, int offset = 0);
    Transition *transition;
    void updatePosition();
    StateItem* getFrom();
    StateItem *getTo();
    QString getLabel() const { return label->toPlainText(); }
    void setLabel(const QString &text) { label->setPlainText(text); }

protected:
//  void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

signals:
//  void stateDeleted(StateItem *state);

private:
  StateItem *fromState;
  StateItem *toState;
  QGraphicsTextItem *label;
  int offsetIndex;
};

#endif // STATEITEM_HPP