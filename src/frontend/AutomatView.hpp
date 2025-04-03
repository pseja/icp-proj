#ifndef AUTOMATVIEW_HPP
#define AUTOMATVIEW_HPP

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
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

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;
//private:
//  QGraphicsScene *scene;
};

#endif