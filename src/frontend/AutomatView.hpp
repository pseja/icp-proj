/**
 * @file AutomatView.hpp
 * @brief Defines the AutomatView class and it's methods which provide a graphical view for the FSM editing.
 *
 * @author Vaclav Sovak (xsovakv00)
 * @date 11-05-2025
 */

#ifndef AUTOMATVIEW_HPP
#define AUTOMATVIEW_HPP

#include "StateItem.hpp"
#include "TransitionItem.hpp"
#include "backend/fsm.hpp"
// #include "src/frontend/mainwindow.hpp"
// #include "src/frontend/mainwindow.hpp"
// #include "src/frontend/mainwindow.hpp"
#include <qevent.h>
#include <qglobal.h>
#include <qgraphicsitem.h>
#include <qgraphicssceneevent.h>
#include <qgraphicsview.h>
#include <qobjectdefs.h>
#include <qwidget.h>
#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QMouseEvent>
#include <cstddef>
#include "StateItem.hpp"
#include <algorithm>
#include <asm-generic/errno.h>
#include <linux/limits.h>
#include <qdebug.h>
#include <qevent.h>
#include <qglobal.h>
#include <qgraphicsitem.h>
#include <qgraphicsscene.h>
#include <qgraphicssceneevent.h>
#include <qgraphicsview.h>
#include <qline.h>
#include <qlist.h>
#include <qnamespace.h>
#include <qobjectdefs.h>
#include <qpen.h>
#include <qpoint.h>
#include <typeinfo>
#include "TransitionItem.hpp"

/**
 * @class AutomatView
 * @brief A graphics view widget for displaying and interacting with FSMs.
 */
class AutomatView : public QGraphicsView {
    Q_OBJECT

   public:
    /**
     * @brief Constructs an AutomatView with the given FSM and parent widget.
     *
     * @param fsm Pointer to the FSM model this view will visualize and manipulate.
     * @param parent Optional parent widget.
     */
    explicit AutomatView(FSM *fsm, QWidget *parent = nullptr);

   signals:
    /**
     * @brief Signal emitted when a state is selected in the view.
     *
     * @param state Pointer to the selected StateItem.
     */
    void stateSelected(StateItem *state);

    /**
     * @brief Signal emitted when a transition is selected in the view.
     *
     * @param transition Pointer to the selected TransitionItem.
     */
    void transitionSelected(TransitionItem *transition);

    /**
     * @brief Signal emitted to add a new state to the FSM.
     *
     * @param state Pointer to the StateItem to be added.
     */
    void addState(StateItem *state);

    /**
     * @brief Signal emitted to add a new transition to the FSM.
     *
     * @param trans Pointer to the TransitionItem to be added.
     */
    void addTransition(TransitionItem *trans);

    /**
     * @brief Signal emitted to display FSM information.
     */
    void showFSMInfo();

   private slots:
    /**
     * @brief Handles selection changes in the view.
     */
    void onSelection();

   protected:
    /**
     * @brief Handles mouse press events.
     *
     * @param event The mouse event.
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief Handles mouse double-click events.
     *
     * @param event The mouse event.
     */
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    /**
     * @brief Handles mouse move events.
     *
     * @param event The mouse event.
     */
    void mouseMoveEvent(QMouseEvent *event) override;

    /**
     * @brief Handles mouse release events.
     *
     * @param event The mouse event.
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

   private:
    /**
     * @brief Pointer to the StateItem where a transition begins during creation.
     */
    StateItem *transitionStart = nullptr;

    /**
     * @brief Temporary line shown during transition creation.
     */
    QGraphicsLineItem *templine = nullptr;

    /**
     * @brief Pointer to the visualized FSM.
     */
    FSM *fsm;

    /**
     * @brief Counter for generating unique state names.
     */
    int stateCounter = 1;
    //  QGraphicsScene *scene;
};

#endif