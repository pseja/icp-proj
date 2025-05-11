/**
 * @file TransitionItem.hpp
 * @brief Defines the TransitionItem class and it's methods which represent a transition in the FSM graphical editor.
 *
 * @author Vaclav Sovak (xsovakv00)
 * @date 11-05-2025
 */

#ifndef TRANSITIONITEM_HPP
#define TRANSITIONITEM_HPP

#include <qglobal.h>
#include <qgraphicsitem.h>
#include <qobjectdefs.h>
#include <qpen.h>

#include <QAction>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QObject>
#include <QTimer>

#include "StateItem.hpp"
#include "backend/transition.hpp"

/**
 * @class TransitionItem
 * @brief A graphical representation of a transition between states in the FSM diagram.
 */
class TransitionItem : public QObject, public QGraphicsPathItem {
    Q_OBJECT

   public:
    /**
     * @brief Constructs a TransitionItem between two states.
     *
     * @param fromState The source state of the transition.
     * @param toState The target state of the transition.
     * @param parent The parent QGraphicsItem (optional).
     * @param offset Offset value for curved transitions between the same states (optional).
     */
    explicit TransitionItem(StateItem *fromState, StateItem *toState, QGraphicsItem *parent = nullptr, int offset = 0);

    /**
     * @brief Pointer to the backend Transition class this item represents.
     */
    Transition *transition;

    /**
     * @brief Updates the transition's position based on the current state positions.
     */
    void updatePosition();

    /**
     * @brief Custom paint method for drawing the transition.
     *
     * @param painter The painter to use for drawing.
     * @param option Style options for drawing.
     * @param widget The widget being painted on.
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    /**
     * @brief Gets the source state of the transition.
     *
     * @return Pointer to the source StateItem.
     */
    StateItem *getFrom();

    /**
     * @brief Gets the target state of the transition.
     *
     * @return Pointer to the target StateItem.
     */
    StateItem *getTo();

    /**
     * @brief Gets the label text of the transition.
     *
     * @return The label text as QString.
     */
    QString getLabel() const { return label->toPlainText(); }

    /**
     * @brief Sets the label text of the transition.
     *
     * @param text The new label text.
     */
    void setLabel(const QString &text) { label->setPlainText(text); }


    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

    /**
     * @brief Starts the transition blinking effect alternating between green and black.
     */
    void startBlinking() {
        if (!blinking) {
            blink = new QTimer(this);
            connect(blink, &QTimer::timeout, this, &TransitionItem::setBlinking);
            blinking = false;
        }
        blink->start(300);
    }

    /**
     * @brief Stops the transition blinking effect.
     */
    void stopBlinking() {
        if (blink) blink->stop();
        setPen(QPen(Qt::black, 2));
    }
    
  signals:
    void transitionDeleted(TransitionItem *transition);

   private:
    /**
     * @brief The source state of the transition.
     */
    StateItem *fromState;

    /**
     * @brief The target state of the transition.
     */
    StateItem *toState;

    /**
     * @brief Text item displaying the transition's label.
     */
    QGraphicsTextItem *label;

    /**
     * @brief Offset index used to position curved transitions.
     */
    int offsetIndex;

    /**
     * @brief Timer used for the blinking effect.
     */
    QTimer *blink = nullptr;

    /**
     * @brief Current blinking state.
     */
    bool blinking = false;

    /**
     * @brief Toggles the blinking effect.
     */
    void setBlinking() {
        blinking = !blinking;
        setPen(QPen(blinking ? Qt::green : Qt::black, 2));
    }
};

#endif  // TRANSITIONITEM_HPP