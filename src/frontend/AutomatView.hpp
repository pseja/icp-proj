#ifndef AUTOMATVIEW_HPP
#define AUTOMATVIEW_HPP

#include "src/frontend/StateItem.hpp"
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
  explicit AutomatView(QWidget *parent = nullptr);

signals:
  void stateSelected(StateItem *state);
  void addState(StateItem *state);

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;

private:
  StateItem *transitionStart = nullptr;
  QGraphicsLineItem *templine = nullptr;
//  QGraphicsScene *scene;
};

#endif