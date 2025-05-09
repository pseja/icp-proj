/**
 * @file variable.cpp
 * @brief Implements the Variable class and it's methods, which represent a variable in the FSM.
 *
 * @author Lukas Pseja (xpsejal00)
 * @date 03-04-2025
 */

#include "variable.hpp"

Variable::Variable(QString type, QString name, QVariant value) : type(type), name(name), value(value) {}

QString Variable::getName() { return name; }

QString Variable::getType() { return type; }

QVariant Variable::getValue() { return value; }

void Variable::setName(const QString &new_name) { name = new_name; }

void Variable::setType(const QString &new_type) { type = new_type; }

void Variable::setValue(const QVariant &new_value) { value = new_value; }
