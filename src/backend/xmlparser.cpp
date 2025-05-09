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
#include <qchar.h>
#include <qglobal.h>
#include <qnumeric.h>

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

    qInfo() << "Parsing XML file:" << file_path;

    // Get the root element and check if it's <automaton>
    QDomElement root = document.documentElement();
    if (root.tagName() != "automaton")
    {
        qCritical() << "Invalid XML format: Root element is not <automaton>";
        return false;
    }

    // Check if the root element has the required "name" attribute and it's not empty
    if (!root.hasAttribute("name") || root.attribute("name").isEmpty())
    {
        qCritical()
            << "Invalid XML format: The automaton element is missing the required \"name\" attribute or it's empty";
        return false;
    }

    // Check if there are additional attributes besides "name"
    if (root.attributes().count() > 1)
    {
        qCritical() << "The root element has additional attributes besides \"name\"";
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

    // Fetch the inputs
    QDomElement inputs_node = root.firstChildElement("inputs");
    if (inputs_node.isNull())
    {
        qWarning() << "No <inputs> element found in XML";
    }
    else
    {
        QDomElement input_node = inputs_node.firstChildElement("input");
        qInfo() << "Inputs:";
        while (!input_node.isNull())
        {
            if (!input_node.hasAttribute("name") || input_node.attribute("name").isEmpty())
            {
                qCritical() << "Invalid XML format: <input> element has no \"name\" attribute or it's empty";
                return false;
            }

            // Check if there are additional attributes besides "name"
            if (input_node.attributes().count() > 1)
            {
                qCritical() << "Invalid XML format: The <input> element has additional attributes besides \"name\"";
                return false;
            }

            QString input_name = input_node.attribute("name");
            state_machine.addInput(input_name);

            qInfo() << input_name;

            input_node = input_node.nextSiblingElement("input");
        }
    }

    // Fetch the outputs
    QDomElement outputs_node = root.firstChildElement("outputs");
    if (outputs_node.isNull())
    {
        qWarning() << "No <outputs> element found in XML";
    }
    else
    {
        QDomElement output_node = outputs_node.firstChildElement("output");
        qInfo() << "Outputs:";
        while (!output_node.isNull())
        {
            if (!output_node.hasAttribute("name") || output_node.attribute("name").isEmpty())
            {
                qCritical() << "Invalid XML format: <output> element has no \"name\" attribute or it's empty";
                return false;
            }

            // Check if there are additional attributes besides "name"
            if (output_node.attributes().count() > 1)
            {
                qCritical() << "Invalid XML format: The <output> element has additional attributes besides \"name\"";
                return false;
            }

            QString output_name = output_node.attribute("name");
            state_machine.addOutput(output_name);
            qInfo() << output_name;

            output_node = output_node.nextSiblingElement("output");
        }
    }

    // Fetch the variables
    QDomElement variables_node = root.firstChildElement("variables");
    if (variables_node.isNull())
    {
        qWarning() << "No <variables> element found in XML";
    }
    else
    {
        QDomElement variable_node = variables_node.firstChildElement("variable");
        qInfo() << "Variables:";
        while (!variable_node.isNull())
        {
            if (!variable_node.hasAttribute("name") || variable_node.attribute("name").isEmpty() ||
                !variable_node.hasAttribute("type") || variable_node.attribute("type").isEmpty() ||
                !variable_node.hasAttribute("value") || variable_node.attribute("value").isEmpty())
            {
                qCritical()
                    << "Invalid XML format: <variable> element is missing required attributes or they are empty";
                return false;
            }

            if (variable_node.attributes().count() > 3)
            {
                qCritical() << "Invalid XML format: The <variable> element has additional attributes";
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
    }

    // Fetch the states
    QDomElement states_node = root.firstChildElement("states");
    if (states_node.isNull())
    {
        qWarning() << "No <states> element found in XML";
    }
    else
    {
        QDomElement state_node = states_node.firstChildElement("state");
        qInfo() << "States:";
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
    }

    // Fetch the transitions
    QDomElement transitions_node = root.firstChildElement("transitions");
    if (transitions_node.isNull())
    {
        qWarning() << "No <transitions> element found in XML";
    }
    else
    {
        QDomElement transition_node = transitions_node.firstChildElement("transition");
        while (!transition_node.isNull())
        {
            QString from_state_name = transition_node.attribute("from");
            QString to_state_name = transition_node.attribute("to");

            State *from_state = state_machine.getState(from_state_name);
            State *to_state = state_machine.getState(to_state_name);

            if (!state_machine.getStates().contains(from_state_name) ||
                !state_machine.getStates().contains(to_state_name))
            {
                qCritical() << "Invalid transition: Transition from" << from_state_name << "to" << to_state_name
                            << "cannot be created because one or both of the states don't exist";
            }

            QDomElement condition_node = transition_node.firstChildElement("condition");
            QDomElement delay_node = transition_node.firstChildElement("delay");

            QString condition_event;
            QString condition_code;
            QString delay_variable_name;
            Variable *delay_variable = nullptr;

            if (!condition_node.isNull())
            {
                condition_event = condition_node.attribute("event");
                condition_code = condition_node.text();
                qInfo() << "Condition event" << condition_event << "and code" << condition_code << "for transition from"
                        << from_state_name << "to" << to_state_name;
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
                qInfo() << "Delay variable" << delay_variable->getName() << "for transition from" << from_state_name
                        << "to" << to_state_name << "with value" << delay_variable->getValue().toString();
            }

            if (condition_event.isEmpty() && delay_variable == nullptr)
            {
                qCritical() << "Invalid XML: No valid <condition> or <delay> for transition from" << from_state_name
                            << "to" << to_state_name;
                continue;
            }

            int delay_value = delay_variable ? delay_variable->getValue().toInt() : -1;
            qInfo() << "Delay value:" << delay_value;
            state_machine.addTransition(from_state, to_state, condition_event, condition_code, delay_value,
                                        delay_variable_name);

            qInfo() << "Transition" << (condition_event.isEmpty() ? delay_variable_name : condition_event) << "from"
                    << from_state_name << "to" << to_state_name << "created";

            transition_node = transition_node.nextSiblingElement("transition");
        }

        qInfo() << "Parsed" << state_machine.getTransitions().size() << "transitions";
    }

    return true;
}

bool XMLParser::FSMtoXML(FSM &state_machine, const QString &file_path)
{
    qInfo() << "Starting to export FSM to XML file:" << file_path;
    QDomDocument document;

    qInfo() << "Creating root automaton node";
    QDomElement root = document.createElement("automaton");
    root.setAttribute("name", state_machine.getName());
    qInfo() << "Set FSM name to:" << state_machine.getName();
    document.appendChild(root);

    if (!state_machine.getComment().isEmpty())
    {
        qInfo() << "Creating comment node";
        QDomElement comment_node = document.createElement("comment");
        comment_node.appendChild(document.createTextNode(state_machine.getComment()));
        root.appendChild(comment_node);
        qInfo() << "Set comment to:" << state_machine.getComment();
    }

    QSet<QString> inputs = state_machine.getInputs();
    QDomElement inputs_node;
    if (!inputs.isEmpty())
    {
        qInfo() << "Creating inputs node";
        inputs_node = document.createElement("inputs");
    }

    for (const auto &input : inputs)
    {
        qInfo() << "Creating input node";
        QDomElement input_node = document.createElement("input");
        input_node.setAttribute("name", input);
        qInfo() << "Set input name to:" << input;
        inputs_node.appendChild(input_node);
    }
    root.appendChild(inputs_node);

    QSet<QString> outputs = state_machine.getOutputs();
    QDomElement outputs_node;
    if (!outputs.isEmpty())
    {
        qInfo() << "Creating outputs node";
        outputs_node = document.createElement("outputs");
    }

    for (const auto &output : outputs)
    {
        qInfo() << "Creating output node";
        QDomElement output_node = document.createElement("output");
        output_node.setAttribute("name", output);
        qInfo() << "Set output name to:" << output;
        outputs_node.appendChild(output_node);
    }
    root.appendChild(outputs_node);

    QMap<QString, Variable *> variables = state_machine.getVariables();
    QDomElement variables_node;
    if (!variables.isEmpty())
    {
        qInfo() << "Creating variables node";
        variables_node = document.createElement("variables");
    }

    for (const auto &variable : variables)
    {
        qInfo() << "Creating variable node";
        QDomElement variable_node = document.createElement("variable");
        variable_node.setAttribute("name", variable->getName());
        variable_node.setAttribute("type", variable->getType());
        variable_node.setAttribute("value", variable->getValue().toString());
        qInfo() << "Set variable name to:" << variable->getName() << "type to:" << variable->getType()
                << "and value to:" << variable->getValue().toString();
        variables_node.appendChild(variable_node);
    }
    root.appendChild(variables_node);

    QMap<QString, State *> states = state_machine.getStates();
    QDomElement states_node;
    if (!states.isEmpty())
    {
        qInfo() << "Creating states node";
        states_node = document.createElement("states");
    }

    for (const auto &state : states)
    {
        qInfo() << "Creating state node";
        QDomElement state_node = document.createElement("state");
        state_node.setAttribute("name", state->getName());
        qInfo() << "Set state name to:" << state->getName();
        if (state->isInitial())
        {
            state_node.setAttribute("initial", "true");
            qInfo() << "Set state" << state->getName() << "as initial";
        }

        if (!state->getCode().isEmpty())
        {
            qInfo() << "Creating code node";
            QDomElement code_node = document.createElement("code");
            code_node.appendChild(document.createTextNode(state->getCode()));
            qInfo() << "Set code to:" << state->getCode();
            state_node.appendChild(code_node);
        }
        states_node.appendChild(state_node);
    }
    root.appendChild(states_node);

    QMultiMap<QString, Transition *> transitions = state_machine.getTransitions();
    QDomElement transitions_node;

    if (!transitions.isEmpty())
    {
        qInfo() << "Creating transitions node";
        transitions_node = document.createElement("transitions");
    }

    for (auto it = transitions.begin(); it != transitions.end(); it++)
    {
        QString from_state_name = it.key();
        Transition *transition = it.value();

        qInfo() << "Creating transition node";
        QDomElement transition_node = document.createElement("transition");
        transition_node.setAttribute("from", transition->getFrom()->getName());
        transition_node.setAttribute("to", transition->getTo()->getName());
        qInfo() << "Set transition from" << transition->getFrom()->getName() << "to" << transition->getTo()->getName();

        if (!transition->getEvent().isEmpty())
        {
            qInfo() << "Creating condition node";
            QDomElement condition_node = document.createElement("condition");
            condition_node.setAttribute("event", transition->getEvent());
            qInfo() << "Set condition event to:" << transition->getEvent();
            condition_node.appendChild(document.createTextNode(transition->getCondition()));
            qInfo() << "Set condition code to:" << transition->getCondition();
            transition_node.appendChild(condition_node);
        }

        if (transition->getDelay() != -1)
        {
            qInfo() << "Creating delay node";
            QDomElement delay_node = document.createElement("delay");
            delay_node.appendChild(document.createTextNode(transition->getDelayVariableName()));
            qInfo() << "Set delay to:" << transition->getDelayVariableName();
            transition_node.appendChild(delay_node);
        }

        transitions_node.appendChild(transition_node);
    }
    root.appendChild(transitions_node);

    qInfo() << "Exporting FSM to XML file:" << file_path;
    QFile file(file_path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qCritical() << "Couldn't open file" << file_path;
        return false;
    }

    qInfo() << "Writing XML content to file";
    QTextStream stream(&file);
    stream << document.toString(4); // 4 space indentation
    file.close();

    qInfo() << "Successully exported FSM to XML file:" << file_path;
    return true;
}