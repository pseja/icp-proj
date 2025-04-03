#ifndef STATEITEM_HPP
#define STATEITEM_HPP

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <qglobal.h>
#include <qgraphicsitem.h>

class StateItem : public QGraphicsEllipseItem {
public:
    explicit StateItem(const QString &name, const QString &code = "", QGraphicsItem *parent = nullptr);

    void setName(const QString &newName);
    void setCodeSegment(const QString &newCode);
    QString getCodeSegment() const;

protected:
  void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

private:
  QGraphicsTextItem *textItem;
  QString stateName;
  QString codeSegment;
};

#endif // STATEITEM_HPP
