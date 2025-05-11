/**
 * @file StateItem.hpp
 * @brief Defines the StateItem class and it's methods which represent a state in the FSM editor.
 *
 * @author Vaclav Sovak (xsovakv00)
 * @date 11-05-2025
 */

#ifndef STATEITEM_HPP
#define STATEITEM_HPP

#include <qglobal.h>
#include <qgraphicsitem.h>
#include <qgraphicssceneevent.h>
#include <qobjectdefs.h>
#include <qvariant.h>

#include <QAction>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QObject>

#include "backend/state.hpp"
// #include "src/backend/state.hpp"

/**
 * @class StateItem
 * @brief A graphical representation of a state in the FSM.
 */
class StateItem : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT
   public:
    /**
     * @brief Constructs a StateItem with the given name and code.
     *
     * @param name The name of the state.
     * @param code The code associated with the state (optional).
     * @param parent The parent QGraphicsItem (optional).
     */
    explicit StateItem(const QString &name, const QString &code = "", QGraphicsItem *parent = nullptr);

    /**
     * @brief Pointer to the backend State class this item represents.
     */
    State *state;

    /**
     * @brief Updates the state properties.
     *
     * @param newName The new name for the state.
     * @param newCode The new code for the state.
     * @param initial Whether this is the initial state.
     */
    void updateState(const QString &newName, const QString &newCode, bool initial);

   protected:
    /**
     * @brief Handles context menu events for the state item.
     *
     * @param event The context menu event.
     */
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

    /**
     * @brief Handles mouse move events for the state item.
     *
     * @param event The mouse event.
     */
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

   signals:
    /**
     * @brief Signal emitted when the state is deleted.
     *
     * @param state Pointer to the deleted StateItem.
     */
    void stateDeleted(StateItem *state);

    /**
     * @brief Signal emitted when the state's position changes.
     */
    void positionChanged();

   private:
    /**
     * @brief Text item displaying the state's name.
     */
    QGraphicsTextItem *textItem;
};

#endif  // STATEITEM_HPP
