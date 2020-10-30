#include "message_definition.h"

#include <boost/tokenizer.hpp>

using namespace message_introspection;

// CONSTRUCTORS
message_definition::message_definition(const std::string& message_type, const std::string& message_definition)
{
    // Set up primitives map.
    message_definition::m_primitive_types["bool"] = 1;
    message_definition::m_primitive_types["int8"] = 1;
    message_definition::m_primitive_types["int16"] = 2;
    message_definition::m_primitive_types["int32"] = 4;
    message_definition::m_primitive_types["int64"] = 8;
    message_definition::m_primitive_types["uint8"] = 1;
    message_definition::m_primitive_types["uint16"] = 2;
    message_definition::m_primitive_types["uint32"] = 4;
    message_definition::m_primitive_types["uint64"] = 8;
    message_definition::m_primitive_types["float32"] = 4;
    message_definition::m_primitive_types["float64"] = 8;
    message_definition::m_primitive_types["string"] = 0;
    message_definition::m_primitive_types["time"] = 64;
    message_definition::m_primitive_types["duration"] = 64;

    // First extract message component types.
    message_definition::parse_components(message_type, message_definition);

    // Add top level message to the definition, and let recursion handle the rest.
    message_definition::add_definition(message_definition::m_definition, {message_type});
}

// PRINTING
std::string message_definition::print_components() const
{
    // Create output stream.
    std::stringstream output;

    // Iterate over component definitions.
    for(auto component = message_definition::m_component_definitions.begin(); component != message_definition::m_component_definitions.end(); ++component)
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
std::string message_definition::print_definition() const
{
    // Create output stream.
    std::stringstream output;

    // Recursively print the tree.
    message_definition::print_definition(output, message_definition::m_definition, 0);

    return output.str();
}

// LISTING
const message_definition::definition_t* message_definition::get_definition(const std::string& path) const
{
    // Split up the path into its component pieces.
    boost::char_separator<char> delimiter(".");
    boost::tokenizer<boost::char_separator<char>> tokenizer(path, delimiter);
    std::vector<std::string> path_parts(tokenizer.begin(), tokenizer.end());

    // Iterate over path parts to traverse the tree.
    auto parent_definition = &(message_definition::m_definition);
    for(auto path_part = path_parts.cbegin(); path_part != path_parts.cend(); ++path_part)
    {
        bool path_part_found = false;
        // Iterate through current parent definition to find the path part as a field name.
        for(auto field = parent_definition->fields.begin(); field != parent_definition->fields.end(); ++field)
        {
            // Check if field name matches path name.
            if(field->name.compare(*path_part) == 0)
            {
                // Path matches. Update parent_definition to this field and stop search.
                parent_definition = &(*field);
                path_part_found = true;
                break;
            }
        }

        // Quit if the path part was not found.
        if(!path_part_found)
        {
            return nullptr;
        }
    }

    return parent_definition;
}
bool message_definition::list_fields(std::vector<field_info_t>&  fields, std::string parent_path) const
{
    // Get the definition for the parent path.
    auto parent_definition = message_definition::get_definition(parent_path);

    // If the parent path couldn't be found, quit.
    if(!parent_definition)
    {
        return false;
    }

    // Clear the output vector.
    fields.clear();

    // Populate the fields output from the parent definition.
    for(auto field = parent_definition->fields.begin(); field != parent_definition->fields.end(); ++field)
    {
        // Create output field.
        field_info_t output_field;
        
        // Set type information.
        output_field.type = field->type;
        output_field.array = field->array;
        output_field.is_primitive = message_definition::is_primitive_type(field->type);

        // Set name information.
        output_field.name = field->name;

        // Set path information.
        if(!parent_path.empty())
        {
            output_field.path = parent_path + ".";
        }
        output_field.path += field->name;

        // Add to output.
        fields.push_back(output_field);
    }

    return true;
}
bool message_definition::field_info(field_info_t& field_info, const std::string& path) const
{
    // Get definition for path.
    auto definition = message_definition::get_definition(path);

    // Quit if path wasn't found.
    if(!definition)
    {
        return false;
    }

    // Set the field info.
    field_info.type = definition->type;
    field_info.is_primitive = message_definition::is_primitive_type(definition->type);
    field_info.array = definition->array;
    field_info.name = definition->name;
    field_info.path = path;

    return true;
}

void message_definition::new_message(const std::vector<uint8_t>& serialized_data)
{
   message_definition::m_value_map.clear();

    // Iterate through the definition tree linearly to segment out the bytes.
    auto byte = serialized_data.cbegin();
    auto* definition = &(message_definition::m_definition);
    std::string parent_path = "";
    for(auto field = definition->fields.begin(); field != definition->fields.end(); ++field)
    {
        // Build current path.
        std::string current_path = "";
        if(!parent_path.empty())
        {
            current_path = parent_path + ".";
        }
        current_path += field->name;

        // Check if field is primitive.
        if(field->is_primitive)
        {
            // Check if field is string.
            if(field->is_string)
            {
                // Field is string.
                // Grab vector for this path.
                auto& bytes = message_definition::m_value_map[current_path];

                // Read string bytes from current position until null termination.
                for(;byte != serialized_data.cend(); ++byte)
                {
                    bytes.push_back(*byte);
                    if(*byte == 0)
                    {
                        break;
                    }
                }

                // Done reading this field. Continue to next field.
                continue;
            }

            // Check if field is array.
            if(field->is_array)
            {
                // Grab vector for this path.
                
                // Check if fixed or variable size.
                if(field->array_length > 0)
                {
                    // Fixed size.
                    // Get total length of bytes for this array.
                    uint32_t data_length = field->array_length * field->size;
                    // Read bytes in.
                }
                else
                {
                    // Variable size.
                }
            }
        }
        else
        {
            // ? RECURSE ?
        }
    }
}

// GET
bool message_definition::get_field(double& value, const std::string& path) const
{

}

// PRIMITIVES
bool message_definition::is_primitive_type(const std::string& type) const
{
    return (message_definition::m_primitive_types.count(type) != 0);
}

// COMPONENT PARSING
void message_definition::parse_components(std::string message_type, std::string message_definition)
{
    // Clear current component definitions.
    message_definition::m_component_definitions.clear();

    // Add top-level message to definition and set it as the current workspace.
    auto* fields_workspace = &message_definition::m_component_definitions[message_type];

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
            fields_workspace = &message_definition::m_component_definitions[tokens[1]];
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

            // Create a new component.
            message_definition::component_t new_component;

            // Set type information.
            new_component.type = type;
            new_component.is_primitive = message_definition::is_primitive_type(type);
            new_component.is_string = (type.compare("string") == 0);
            // Set array information.
            new_component.array = array;
            new_component.is_array = !array.empty();
            if(array.size() > 2)
            {
                new_component.array_length = std::stoul(array.substr(1, array.size() - 2));
            }
            else
            {
                new_component.array_length = 0;
            }
            // Set name information.
            new_component.name = tokens[1];

            // Add to fields workspace.
            fields_workspace->push_back(new_component);
        }
    }

    // Iterate through the definition map to correct incomplete types.
    for(auto definition = message_definition::m_component_definitions.begin(); definition != message_definition::m_component_definitions.end(); ++definition)
    {
        auto& fields = definition->second;
        for(auto field = fields.begin(); field != fields.end(); ++field)
        {
            // Check if field is a primitive field.
            if(message_definition::is_primitive_type(field->type))
            {
                continue;
            }

            // Check if the field's type definition exists in the definition map.
            if(message_definition::m_component_definitions.count(field->type) == 0)
            {
                // Exact typename not found. Search through definitions to find the matching full type name.
                for(auto candidate = message_definition::m_component_definitions.begin(); candidate != message_definition::m_component_definitions.end(); ++ candidate)
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

// DEFINITION
void message_definition::add_definition(definition_t& definition, const component_t& component_definition)
{
    // Set the definition's type, array, and name.
    definition.type = component_definition.type;
    definition.array = component_definition.array;
    definition.name = component_definition.name;

    // Set description elements.
    definition.is_primitive = component_definition.is_primitive;
    definition.is_string = component_definition.is_string;
    definition.is_array = component_definition.is_array;
    definition.array_length = component_definition.array_length;

    // Find the definition's type.
    if(message_definition::is_primitive_type(definition.type))
    {
        // This definition is primitive. Set it's size and complete.
        definition.size = message_definition::m_primitive_types[definition.type];
    }
    else
    {
        // This definition's type is NOT primitive, and it has fields to it.

        // Initialize the definition's size to 0 so it can be summed over the fields.
        definition.size = 0;

        // Get the fields of this definition from the component map.
        auto fields = message_definition::m_component_definitions[definition.type];
        // Iterate through each field to add it recursively to the definition.
        for(auto field = fields.begin(); field != fields.end(); ++field)
        {
            // Add a new definition to the array BEFORE populating it for copy efficiency.
            definition.fields.push_back(definition_t());
            auto& field_definition_reference = definition.fields.back();
            // Now add the details to the field recursively and in place.
            message_definition::add_definition(field_definition_reference, *field);
            // Add field's computed size to the definition's size.
            definition.size += field_definition_reference.size;
        }
    }
}
void message_definition::print_definition(std::stringstream& stream, const definition_t& definition, uint32_t level) const
{
    // Print definition's info in one line.
    stream << "name = " << definition.name << " type = " << definition.type << " size = " << definition.size << " array = " << definition.array << std::endl;

    // Print sub definitions indented.
    for(auto field = definition.fields.begin(); field != definition.fields.end(); ++field)
    {
        for(uint32_t i = 0; i <= level; ++i)
        {
            stream << "\t";
        }
        message_definition::print_definition(stream, *field, level+1);
    }
}