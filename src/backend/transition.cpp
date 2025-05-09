/**
 * @file transition.cpp
 * @brief Implements the Transition class and it's methods, which represent a transition between states in the FSM.
 *
 * @author Lukas Pseja (xpsejal00)
 * @date 03-04-2025
 */

#include "transition.hpp"

Transition::Transition(State *from, State *to, QString event, QString condition, int delay,
                       QString new_delay_variable_name)
    : from(from),
      to(to),
      event(event),
      condition(condition),
      delay(delay),
      delay_variable_name(new_delay_variable_name) {
    if (delay > 0) {
        is_delayed_transition = true;
    }
}

State *Transition::getFrom() { return from; }

State *Transition::getTo() { return to; }

QString Transition::getEvent() { return event; }

QString Transition::getCondition() { return condition; }

int Transition::getDelay() { return delay; }

QString Transition::getDelayVariableName() { return delay_variable_name; }

bool Transition::isDelayedTransition() { return is_delayed_transition; }

void Transition::setFrom(State *new_from) { from = new_from; }

void Transition::setTo(State *new_to) { to = new_to; }

void Transition::setEvent(QString new_event) { event = new_event; }

void Transition::setCondition(QString new_condition) { condition = new_condition; }

void Transition::setDelay(int new_delay) {
    delay = new_delay;
    if (delay > 0) {
        is_delayed_transition = true;
    }
}

void Transition::setDelayVariableName(QString new_delay_variable_name) {
    delay_variable_name = new_delay_variable_name;
}

void Transition::setDelayedTransition(bool new_is_delayed_transition) {
    is_delayed_transition = new_is_delayed_transition;
}
