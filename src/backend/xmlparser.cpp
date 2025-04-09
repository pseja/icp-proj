/**
 * @file xmlparser.cpp
 * @brief Implements the XMLParser class for parsing XML files into FSM objects and exporting FSM objects to XML.
 *
 * This file contains the implementation of the XMLParser class, including methods for reading an XML file
 * and converting it into a finite state machine (FSM) object, as well as exporting an FSM object into an XML file.
 *
 * The XMLParser class handles parsing states, transitions, variables, inputs, and outputs from the XML format
 * and populates the FSM object accordingly.
 *
 * @author Lukas Pseja (xpsejal00)
 * @date 04-04-2025
 */

#include "xmlparser.hpp"

bool XMLParser::XMLtoFSM(const QString &file_path, FSM &state_machine)
{
    QString xml_content;
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qCritical() << "Couldn't open file" << file_path;
        return false;
    }

    QDomDocument document;
    if (!document.setContent(&file))
    {
        qCritical() << "Couldn't set content of file" << file_path;
        file.close();
        return false;
    }

    file.close();

    qInfo() << "Parsing XML file: " << file_path;

    // Get the root element
    QDomElement root = document.documentElement();
    if (root.tagName() != "automaton")
    {
        qCritical() << "Invalid XML format: Root element is not <automaton>";
        return false;
    }

    // Check for required attributes
    if (!root.hasAttribute("name"))
    {
        qCritical() << "No name attribute found in XML";
        return false;
    }

    // Fetch the FSM name and set it
    QString fsm_name = root.attribute("name");
    state_machine.setName(fsm_name);
    qInfo() << "FSM name:" << state_machine.getName();

    // Fetch optional comment node
    QDomElement comment_node = root.firstChildElement("comment");
    if (comment_node.isNull())
    {
        qWarning() << "No comment found in XML";
    }
    else
    {
        // Fetch the comment text and set it
        QString comment = comment_node.text();
        state_machine.setComment(comment);
        qInfo() << "Comment:" << state_machine.getComment();
    }

    // Fetch the inputs and outputs
    QDomElement inputs_node = root.firstChildElement("inputs");
    if (inputs_node.isNull())
    {
        qCritical() << "No <inputs> element found in XML";
        return false;
    }

    // Fetch the inputs
    QDomElement input_node = inputs_node.firstChildElement("input");
    qInfo() << "Inputs:";
    while (!input_node.isNull())
    {
        if (!input_node.hasAttribute("name"))
        {
            qCritical() << "Invalid XML: <input> element has no \"name\" attribute";
            return false;
        }

        QString input_name = input_node.attribute("name");
        state_machine.addInput(input_name);

        qInfo() << input_name;

        input_node = input_node.nextSiblingElement("input");
    }

    // Fetch the outputs
    QDomElement outputs_node = root.firstChildElement("outputs");
    if (outputs_node.isNull())
    {
        qCritical() << "No <outputs> element found in XML";
        return false;
    }

    QDomElement output_node = outputs_node.firstChildElement("output");
    qInfo() << "Outputs:";
    while (!output_node.isNull())
    {
        if (!output_node.hasAttribute("name"))
        {
            qCritical() << "Invalid XML: <output> element has no \"name\" attribute";
            return false;
        }

        QString output_name = output_node.attribute("name");
        state_machine.addOutput(output_name);
        qInfo() << output_name;

        output_node = output_node.nextSiblingElement("output");
    }

    QDomElement variables_node = root.firstChildElement("variables");
    if (variables_node.isNull())
    {
        qCritical() << "No <variables> element found in XML";
        return false;
    }

    QDomElement variable_node = variables_node.firstChildElement("variable");
    if (variable_node.isNull())
    {
        qCritical() << "No <variable> element found in XML";
        return false;
    }

    qInfo() << "Variables:";
    while (!variable_node.isNull())
    {
        if (!variable_node.hasAttribute("name") || !variable_node.hasAttribute("type") ||
            !variable_node.hasAttribute("value"))
        {
            qCritical() << "Invalid XML: <variable> element is missing required attributes";
            return false;
        }

        QString variable_name = variable_node.attribute("name");
        QString variable_type = variable_node.attribute("type");
        QString variable_value = variable_node.attribute("value");

        state_machine.addVariable(variable_type, variable_name, variable_value);

        Variable *variable = state_machine.getVariable(variable_name);

        qInfo() << variable->getType() << variable->getName() << variable->getValue().toString();

        variable_node = variable_node.nextSiblingElement("variable");
    }

    QDomElement states_node = root.firstChildElement("states");
    if (states_node.isNull())
    {
        qCritical() << "Invalid XML: No <states> element found";
        return false;
    }

    QDomElement state_node = states_node.firstChildElement("state");
    while (!state_node.isNull())
    {
        QString state_name = state_node.attribute("name");
        bool is_initial = state_node.attribute("initial").toLower() == "true";

        State *state = new State(state_name);
        state->setName(state_name);
        state_machine.addState(state, state_name);

        qInfo() << "State" << state_name << "created";

        if (is_initial)
        {
            if (state_machine.getInitialState() != nullptr)
            {
                qCritical() << "Invalid XML: Multiple initial states found";
                return false;
            }

            state_machine.setInitialState(state);
            state->setInitial(true);

            qInfo() << "Initial state set to" << state_machine.getInitialState()->getName();
        }

        QDomElement code_node = state_node.firstChildElement("code");
        QString code = code_node.isNull() ? "" : code_node.text();
        state->setCode(code);
        qInfo() << "Code" << state->getCode() << "set for state" << state_name;

        state_node = state_node.nextSiblingElement("state");
    }

    qInfo() << "Parsed" << state_machine.getStates().size() << "states";

    QDomElement transitions_node = root.firstChildElement("transitions");
    if (transitions_node.isNull())
    {
        qCritical() << "Invalid XML: No <transitions> element found";
        return false;
    }

    QDomElement transition_node = transitions_node.firstChildElement("transition");
    while (!transition_node.isNull())
    {
        QString from_state_name = transition_node.attribute("from");
        QString to_state_name = transition_node.attribute("to");

        State *from_state = state_machine.getState(from_state_name);
        State *to_state = state_machine.getState(to_state_name);

        if (!state_machine.getStates().contains(from_state_name) || !state_machine.getStates().contains(to_state_name))
        {
            qCritical() << "Invalid transition: Transition from" << from_state_name << "to" << to_state_name
                        << "cannot be created because one or both of the states don't exist";
        }

        QDomElement condition_node = transition_node.firstChildElement("condition");
        QDomElement delay_node = transition_node.firstChildElement("delay");

        QString condition_event;
        QString delay_variable_name;
        Variable *delay_variable = nullptr;

        if (!condition_node.isNull())
        {
            condition_event = condition_node.attribute("event");
            qInfo() << "Condition event" << condition_event << "for transition from" << from_state_name << "to"
                    << to_state_name;
        }

        if (!delay_node.isNull())
        {
            delay_variable_name = delay_node.text();
            delay_variable = state_machine.getVariable(delay_variable_name);
            if (delay_variable == nullptr)
            {
                qCritical() << "Invalid XML: Delay variable" << delay_variable_name << "not found";
                return false;
            }
            qInfo() << "Delay variable" << delay_variable->getName() << "for transition from" << from_state_name << "to"
                    << to_state_name << "with value" << delay_variable->getValue().toString();
        }

        if (condition_event.isEmpty() && delay_variable == nullptr)
        {
            qCritical() << "Invalid XML: No valid <condition> or <delay> for transition from" << from_state_name << "to"
                        << to_state_name;
            continue;
        }

        // TODO: Handle the case where delay is float/double etc.
        int delay_value = delay_variable ? delay_variable->getValue().toInt() : -1;
        qInfo() << "Delay value:" << delay_value;
        state_machine.addTransition(from_state, to_state, condition_event, delay_value);

        qInfo() << "Transition" << (condition_event.isEmpty() ? delay_variable_name : condition_event) << "from"
                << from_state_name << "to" << to_state_name << "created";

        transition_node = transition_node.nextSiblingElement("transition");
    }

    qInfo() << "Parsed" << state_machine.getTransitions().size() << "transitions";

    return true;
}
