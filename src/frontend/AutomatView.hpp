#ifndef AUTOMATVIEW_HPP
#define AUTOMATVIEW_HPP

#include "StateItem.hpp"
#include "TransitionItem.hpp"
#include "backend/fsm.hpp"
//#include "src/frontend/mainwindow.hpp"
//#include "src/frontend/mainwindow.hpp"
//#include "src/frontend/mainwindow.hpp"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <cstddef>
#include <qevent.h>
#include <qglobal.h>
#include <qgraphicsitem.h>
#include <qgraphicssceneevent.h>
#include <qgraphicsview.h>
#include <qobjectdefs.h>
#include <qwidget.h>
#include <QMouseEvent>
#include <QDebug>

class AutomatView : public QGraphicsView {
  Q_OBJECT

public:
  explicit AutomatView(FSM *fsm, QWidget *parent = nullptr);
  void setLocked(bool locked) { this->locked = locked; }
  bool getLocked() { return locked; }
  bool eventFilter(QObject *obj, QEvent *event) override;
signals:
  void stateSelected(StateItem *state);
  void transitionSelected(TransitionItem *transition);
  void addState(StateItem *state);
  void addTransition(TransitionItem *trans);
  void showFSMInfo();
  

private slots:
  void onSelection();

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;

private:
  StateItem *transitionStart = nullptr;
  QGraphicsLineItem *templine = nullptr;
  FSM *fsm;
  int stateCounter = 1;
  bool locked = false;
//  QGraphicsScene *scene;
};

#endif