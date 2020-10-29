#ifndef PARSER_H
#define PARSER_H

#include "definition.h"

#include <unordered_map>

class parser
{
public:
    parser();

    definition_t parse_message(std::string message_type, std::string message_definition);

private:
    std::unordered_map<std::string, uint32_t> m_primitive_types;
    bool is_primitive_type(const std::string& type);

    std::unordered_map<std::string, std::vector<definition_t>> m_component_definitions;
    void parse_components(std::string message_type, std::string message_definition);

    void add_definition(definition_t& definition, std::string type, std::string array, std::string name);
    void print_definition(definition_t& definition, uint32_t level);
};

#endif