#ifndef AUTOMATVIEW_HPP
#define AUTOMATVIEW_HPP

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <qglobal.h>
#include <qgraphicsitem.h>
#include <qgraphicssceneevent.h>
#include <qgraphicsview.h>
#include <qobjectdefs.h>

class StateItem : public QGraphicsEllipseItem {
public:
  explicit StateItem(qreal x, qreal y);

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

class AutomatView : public QGraphicsView {
  Q_OBJECT

public:
  explicit AutomatView(QWidget *parent = nullptr);
  void addState(qreal x, qreal y);

private:
  QGraphicsScene *scene;
};

#endif