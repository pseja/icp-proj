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

    return true;
}
