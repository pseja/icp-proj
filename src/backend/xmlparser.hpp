/**
 * @file xmlparser.hpp
 * @brief Provides functionality for parsing XML files into a finite state machine (FSM) and vice versa.
 *
 * This file contains the declaration of the XMLParser class, which provides static methods
 * for converting XML files into FSM objects and exporting FSM objects back into XML files.
 *
 * @author Lukas Pseja (xpsejal00)
 * @date 04-04-2025
 */

#pragma once

#include "fsm.hpp"
#include "logger.hpp"
#include <QDomDocument>
#include <QFile>

/**
 * @class XMLParser
 * @brief A utility class for parsing XML files into FSM objects and exporting FSM objects to XML.
 *
 * The XMLParser class provides static methods for reading an XML file and converting it into
 * a finite state machine (FSM) object, as well as exporting an FSM object into an XML file.
 */
class XMLParser
{
  public:
    /**
     * @brief Parses an XML file and converts it into an FSM object.
     * @param file_path The path to the XML file to parse.
     * @param state_machine The FSM object to populate with the parsed data.
     * @return True if the parsing was successful, false otherwise.
     */
    static bool XMLtoFSM(const QString &file_path, FSM &state_machine);

    /**
     * @brief Exports an FSM object into an XML file.
     * @param state_machine The FSM object to export.
     * @param file_path The path to the XML file to create.
     * @return True if the export was successful, false otherwise.
     */
    static bool FSMtoXML(FSM &state_machine, const QString &file_path);
};