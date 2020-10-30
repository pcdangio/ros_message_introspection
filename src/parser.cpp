#include "parser.h"

#include <sstream>
#include <boost/tokenizer.hpp>

#include <iostream>

parser::parser()
{
    // Set up primitives map.
    parser::m_primitive_types["bool"] = 1;
    parser::m_primitive_types["int8"] = 1;
    parser::m_primitive_types["int16"] = 2;
    parser::m_primitive_types["int32"] = 4;
    parser::m_primitive_types["int64"] = 8;
    parser::m_primitive_types["uint8"] = 1;
    parser::m_primitive_types["uint16"] = 2;
    parser::m_primitive_types["uint32"] = 4;
    parser::m_primitive_types["uint64"] = 8;
    parser::m_primitive_types["float32"] = 4;
    parser::m_primitive_types["float64"] = 8;
    parser::m_primitive_types["string"] = 0;
    parser::m_primitive_types["time"] = 64;
    parser::m_primitive_types["duration"] = 64;
}

definition_t parser::parse_message(std::string message_type, std::string message_definition)
{
    // First extract message component types.
    parser::parse_components(message_type, message_definition);

    // Reset primary definition.
    parser::m_definition = definition_t();

    // Add top level message to the definition, and let recursion handle the rest.
    parser::add_definition(parser::m_definition, message_type, "", "");

    // Return a copy of the internal definition.
    return parser::m_definition;
}
std::string parser::print_components() const
{
    std::stringstream output;

    output << "component definitions:" << std::endl;
    for(auto component = parser::m_component_definitions.begin(); component != parser::m_component_definitions.end(); ++component)
    {
        // Output component overall type.
        output << component->first;

        // Output component fields.
        auto& fields = component->second;
        for(auto field = fields.begin(); field != fields.end(); ++field)
        {
            output << "\tname = " << field->name << " type = " << field->type << " array = " << field->array << std::endl;
        }
    }

    return output.str();
}
std::string parser::print_definition() const
{
    std::stringstream output;

    output << "message definition:" << std::endl;

    parser::print_definition(&output, parser::m_definition, 0);

    return output.str();
}

bool parser::is_primitive_type(const std::string& type)
{
    return (parser::m_primitive_types.count(type) != 0);
}


void parser::parse_components(std::string message_type, std::string message_definition)
{
    // Clear current component definitions.
    parser::m_component_definitions.clear();

    // Add top-level message to definition and set it as the current workspace.
    auto* fields_workspace = &parser::m_component_definitions[message_type];

    // Convert description into a stringstream.
    std::stringstream description_stream(message_definition);

    // Set up tokenizer delimiter.
    boost::char_separator<char> delimiter(" ");

    // Iterate through description.
    std::string current_line;
    while(std::getline(description_stream, current_line))
    {
        // Remove any comments from the line before tokenizing.
        auto comment_position = current_line.find_first_of('#');
        if(comment_position != std::string::npos)
        {
            current_line.erase(comment_position);
        }

        // Check if line is empty, an equals separator line, or defining a constant
        if(current_line.empty() || current_line.find_first_of('=') != std::string::npos)
        {
            // Skip this line.
            continue;
        }

        // Tokenize line into vector.
        boost::tokenizer<boost::char_separator<char>> tokenizer(current_line, delimiter);
        std::vector<std::string> tokens(tokenizer.begin(), tokenizer.end());
        
        // Check if first token is a new sub-message designator.
        if(tokens[0].compare("MSG:") == 0)
        {
            // Initiate new sub-message and switch workspace to it.
            fields_workspace = &parser::m_component_definitions[tokens[1]];
        }
        else
        {
            // Check if type is an array.
            auto array_position = tokens[0].find_first_of('[');
            std::string type = tokens[0];
            std::string array = "";
            if(array_position != std::string::npos)
            {
                array = tokens[0].substr(array_position);
                type.erase(array_position);
            }

            // Add to fields workspace.
            fields_workspace->push_back({type, array, tokens[1]});
        }
    }

    // Iterate through the definition map to correct incomplete types.
    for(auto definition = parser::m_component_definitions.begin(); definition != parser::m_component_definitions.end(); ++definition)
    {
        auto& fields = definition->second;
        for(auto field = fields.begin(); field != fields.end(); ++field)
        {
            // Check if field is a primitive field.
            if(parser::is_primitive_type(field->type))
            {
                continue;
            }

            // Check if the field's type definition exists in the definition map.
            if(parser::m_component_definitions.count(field->type) == 0)
            {
                // Exact typename not found. Search through definitions to find the matching full type name.
                for(auto candidate = parser::m_component_definitions.begin(); candidate != parser::m_component_definitions.end(); ++ candidate)
                {
                    // Check if partial type matches full candidate type.
                    if(candidate->first.find(field->type) != std::string::npos)
                    {
                        // Match found. Update partial type to full type.
                        field->type = candidate->first;
                        // Stop search.
                        break;
                    }
                }
            }
        }
    }
}

void parser::add_definition(definition_t& definition, std::string type, std::string array, std::string name)
{
    // Set the definition's type, array, and name.
    definition.type = type;
    definition.array = array;
    definition.name = name;

    // Find the definition's type.
    if(parser::is_primitive_type(type))
    {
        // This definition is primitive. Set it's size and complete.
        definition.size = parser::m_primitive_types[type];
    }
    else
    {
        // This definition's type is NOT primitive, and it has fields to it.

        // Initialize the definition's size to 0 so it can be summed over the fields.
        definition.size = 0;

        // Get the fields of this definition from the component map.
        auto fields = parser::m_component_definitions[type];
        // Iterate through each field to add it recursively to the definition.
        for(auto field = fields.begin(); field != fields.end(); ++field)
        {
            // Create new definition for this field.
            definition_t field_definition;
            // Add it to the array BEFORE populating it for copy efficiency.
            definition.fields.push_back(field_definition);
            auto& field_definition_reference = definition.fields.back();
            // Now add the details to the field recursively and in place.
            parser::add_definition(field_definition_reference, field->type, field->array, field->name);
            // Add field's computed size to the definition's size.
            definition.size += field_definition_reference.size;
        }
    }
}

void parser::print_definition(std::iostream* stream, const definition_t& definition, uint32_t level) const
{
    // Print definition's info in one line.
    *stream << "name = " << definition.name << " type = " << definition.type << " size = " << definition.size << " array = " << definition.array << std::endl;

    // Print sub definitions indented.
    for(auto field = definition.fields.begin(); field != definition.fields.end(); ++field)
    {
        for(uint32_t i = 0; i <= level; ++i)
        {
            *stream << "\t";
        }
        parser::print_definition(stream, *field, level+1);
    }
}