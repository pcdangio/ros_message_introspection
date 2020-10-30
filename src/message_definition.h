#ifndef MESSAGE_DEFINITION_H
#define MESSAGE_DEFINITION_H

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

class message_definition
{
public:
    // CONSTRUCTORS
    message_definition(const std::string& message_type, const std::string& message_definition);

    // PRINTING
    std::string print_components() const;
    std::string print_definition() const;

private:
    // PRIMITIVES
    std::unordered_map<std::string, uint32_t> m_primitive_types;
    bool is_primitive_type(const std::string& type);

    // COMPONENTS
    struct component_t
    {
        std::string type;
        std::string array;
        std::string name;
    };
    std::unordered_map<std::string, std::vector<component_t>> m_component_definitions;
    void parse_components(std::string message_type, std::string message_definition);

    // DEFINITION
    struct definition_t
    {
        std::string type;
        std::string array;
        std::string name;
        uint32_t size;

        std::vector<definition_t> fields;
    };
    definition_t m_definition;
    void add_definition(definition_t& definition, std::string type, std::string array, std::string name);
    void print_definition(std::iostream* stream, const definition_t& definition, uint32_t level) const;
};

#endif