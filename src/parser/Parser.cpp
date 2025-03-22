#include "Parser.hpp"
#include <iostream>

bool Parser::XMLtoFSM(const std::string &file_path, FSM &fsm)
{
    pugi::xml_document doc;

    pugi::xml_parse_result result = doc.load_file(file_path.c_str());
    if (!result)
    {
        std::cerr << "Error parsing XML file: " << result.description() << "\n";
        return false;
    }

    pugi::xml_node automaton_node = doc.child("automaton");
    if (!automaton_node)
    {
        std::cerr << "Error: No automaton node found in XML file\n";
        return false;
    }

    std::string automaton_name = automaton_node.attribute("name").as_string();
    if (automaton_name.empty())
    {
        std::cerr << "Error: No name attribute found in automaton node\n";
        return false;
    }
    fsm.setName(automaton_name);

    std::string automaton_comment = automaton_node.attribute("comment").as_string();
    if (automaton_comment.empty())
    {
        std::cerr << "Error: No comment attribute found in automaton node\n";
        return false;
    }
    fsm.setComment(automaton_comment);

    pugi::xml_node states = automaton_node.child("states");
    if (!states)
    {
        std::cerr << "Error: No states node found in XML file\n";
        return false;
    }

    for (pugi::xml_node state = states.child("state"); state; state = state.next_sibling("state"))
    {
        std::string state_name = state.attribute("name").as_string();
        if (state_name.empty())
        {
            std::cerr << "Error: No name attribute found in state node\n";
            return false;
        }
        State new_state = State(state_name);
        fsm.addState(new_state);

        // TODO this could maybe be done without nesting
        if (state.attribute("initial").as_bool())
        {
            if (!fsm.getInitialState().getName().empty())
            {
                std::cerr << "Error: Multiple initial states found\n";
                return false;
            }

            fsm.setInitialState(State(state_name));
        }
    }

    return true;
}

bool Parser::FSMtoXML(FSM &fsm, const std::string &file_path)
{
    pugi::xml_document doc;

    pugi::xml_node automaton_node = doc.append_child("automaton");
    automaton_node.append_attribute("name") = fsm.getName().c_str();
    automaton_node.append_attribute("comment") = fsm.getComment().c_str();

    // TODO figure out UTF-8 encoding
    return doc.save_file(file_path.c_str());
}
