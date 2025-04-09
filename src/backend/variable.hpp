/**
 * @file variable.hpp
 * @brief Defines the Variable class, which represents a variable in the state machine.
 *
 * This file contains the declaration of the Variable class, which provides functionality
 * for managing variable-specific attributes such as type, name, and value.
 *
 * @author Lukas Pseja (xpsejal00)
 * @date 03-04-2025
 */

#pragma once

#include <QString>
#include <QVariant>

/**
 * @class Variable
 * @brief Represents a variable in the state machine.
 *
 * The Variable class provides attributes and methods to manage variable-specific
 * properties such as the variable's type, name, and value.
 */
class Variable : public QObject
{
    Q_OBJECT

  private:
    QString name;   ///< The name of the variable.
    QString type;   ///< The type of the variable (e.g., "int", "float", "QString").
    QVariant value; ///< The value of the variable.

  public:
    /**
     * @brief Default constructor for the Variable class.
     */
    Variable() = default;

    /**
     * @brief Constructs a Variable object with the given type, name, and value.
     * @param type The type of the variable.
     * @param name The name of the variable.
     * @param value The value of the variable.
     */
    Variable(QString type, QString name, QVariant value);

    /**
     * @brief Gets the name of the variable.
     * @return The name of the variable.
     */
    QString getName();

    /**
     * @brief Gets the type of the variable.
     * @return The type of the variable.
     */
    QString getType();

    /**
     * @brief Gets the value of the variable.
     * @return The value of the variable.
     */
    QVariant getValue();

    /**
     * @brief Sets the name of the variable.
     * @param new_name The new name of the variable.
     */
    void setName(const QString &new_name);

    /**
     * @brief Sets the type of the variable.
     * @param new_type The new type of the variable.
     */
    void setType(const QString &new_type);

    /**
     * @brief Sets the value of the variable.
     * @param new_value The new value of the variable.
     */
    void setValue(const QVariant &new_value);
};