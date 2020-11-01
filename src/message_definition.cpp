#include "message_introspection/message_definition.h"

#include <boost/tokenizer.hpp>
#include <endian.h>

using namespace message_introspection;

// CONSTRUCTORS
message_definition::message_definition()
{
    // Initialize serialized bytes.
    message_definition::m_bytes = nullptr;
}
message_definition::~message_definition()
{
    // Clean up message bytes.
    delete [] message_definition::m_bytes;
}

// MESSAGE
void message_definition::new_message(const topic_tools::ShapeShifter& message)
{
    // First register the message if it hasn't been registered already.
    if(!message_definition::is_registered(message.getMD5Sum()))
    {
        // Register message.
        message_definition::register_message(message.getMD5Sum(), message.getDataType(), message.getMessageDefinition());
    }

    // Clear old data.
    delete [] message_definition::m_bytes;

    // Get serialized length and set up bytes for capture.
    uint32_t message_length = ros::serialization::serializationLength(message);
    message_definition::m_bytes = new uint8_t[message_length];

    // Serialize data into byte storage.
    ros::serialization::OStream stream(message_definition::m_bytes, message_length);
    ros::serialization::serialize(stream, message);

    // Update serialized positions of definition tree.
    uint32_t current_position = 0;
    message_definition::update_positions(message_definition::m_definition_tree, current_position);
}
void message_definition::register_message(const std::string& md5, const std::string& type, const std::string& definition)
{
    // Extract message component types.
    message_definition::parse_components(type, definition);

    // Add top level message to the definition, and let recursion handle the rest.
    message_definition::add_definition("", message_definition::m_definition_tree, definition_t(type, "", ""));

    // Store MD5.
    message_definition::m_md5 = md5;    
}
bool message_definition::is_registered(const std::string& md5)
{
    return (message_definition::m_md5.compare(md5) == 0);
}

// LISTING
const definition_tree_t* message_definition::get_definition_tree(const std::string& path) const
{
    // Split up the path into its component pieces.
    boost::char_separator<char> delimiter(".");
    boost::tokenizer<boost::char_separator<char>> tokenizer(path, delimiter);
    std::vector<std::string> path_parts(tokenizer.begin(), tokenizer.end());

    // Iterate over path parts to traverse the tree.
    auto parent_definition = &(message_definition::m_definition_tree);
    for(auto path_part = path_parts.cbegin(); path_part != path_parts.cend(); ++path_part)
    {
        bool path_part_found = false;
        // Iterate through current parent definition to find the path part as a field name.
        for(auto field = parent_definition->fields.begin(); field != parent_definition->fields.end(); ++field)
        {
            // Check if field name matches path name.
            if(field->definition.name().compare(*path_part) == 0)
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
definition_tree_t message_definition::definition_tree() const
{
    return message_definition::m_definition_tree;
}
bool message_definition::list_fields(std::vector<definition_t>&  fields, std::string parent_path) const
{
    // Get the definition tree for the parent path.
    auto parent_definition = message_definition::get_definition_tree(parent_path);

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
        // Add field to output.
        fields.push_back(field->definition);
    }

    return true;
}
bool message_definition::field_info(definition_t& field_info, const std::string& path) const
{
    // Get definition for path.
    auto definition_tree = message_definition::get_definition_tree(path);

    // Quit if path wasn't found.
    if(!definition_tree)
    {
        return false;
    }

    // Output the definition.
    field_info = definition_tree->definition;

    return true;
}

// GET
bool message_definition::get_field(double& value, const std::string& path) const
{

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
        output << component->first << std::endl;;

        // Output component fields.
        auto& fields = component->second;
        for(auto field = fields.begin(); field != fields.end(); ++field)
        {
            output << "\tname = " << field->name() << " type = " << field->type() << " array = " << field->array() << std::endl;
        }
    }

    return output.str();
}
std::string message_definition::print_definition_tree() const
{
    // Create output stream.
    std::stringstream output;

    // Recursively print the tree.
    message_definition::print_definition_tree(output, message_definition::m_definition_tree, 0);

    return output.str();
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

            // Create a new component definition.
            definition_t new_component(type, array, tokens[1]);

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
            if(field->is_primitive())
            {
                continue;
            }

            // Check if the field's type definition exists in the definition map.
            if(message_definition::m_component_definitions.count(field->type()) == 0)
            {
                // Exact typename not found. Search through definitions to find the matching full type name.
                for(auto candidate = message_definition::m_component_definitions.begin(); candidate != message_definition::m_component_definitions.end(); ++ candidate)
                {
                    // Check if partial type matches full candidate type.
                    if(candidate->first.find(field->type()) != std::string::npos)
                    {
                        // Match found. Update partial type to full type.
                        field->update_type(candidate->first);
                        // Stop search.
                        break;
                    }
                }
            }
        }
    }
}

// DEFINITION
void message_definition::add_definition(const std::string& parent_path, definition_tree_t& definition_tree, const definition_t& component_definition)
{
    // Set the tree's definition.
    definition_tree.definition = component_definition;
    definition_tree.definition.update_parent_path(parent_path);

    // Find the definition's type.
    if(!component_definition.is_primitive())
    {
        // This definition's type is NOT primitive, so it has fields to it.

        // Initialize the definition's size to 0 so it can be summed over the fields.
        uint32_t total_size = 0;

        // Get the fields of this definition from the component map.
        auto fields = message_definition::m_component_definitions[component_definition.type()];
        // Iterate through each field to add it recursively to the definition.
        for(auto field = fields.begin(); field != fields.end(); ++field)
        {
            // Add a new definition to the array BEFORE populating it for copy efficiency.
            definition_tree.fields.push_back(definition_tree_t());
            auto& field_reference = definition_tree.fields.back();
            // Now add the details to the field recursively and in place.
            message_definition::add_definition(definition_tree.definition.path(), field_reference, *field);
            // Add field's computed size to the definition's size.
            total_size += field_reference.definition.size();
        }

        // Update the top level size.
        definition_tree.definition.update_size(total_size);
    }
}
void message_definition::update_positions(definition_tree_t& definition_tree, uint32_t& current_position)
{
    // If this is a primitive type, update it's serialized position.
    if(definition_tree.definition.is_primitive())
    {
        definition_tree.definition.update_serialized_position(current_position);
    }

    // Check if this field is an array or repeated field, and get it's length.
    uint32_t array_length;
    switch(definition_tree.definition.array_type())
    {
        case definition_t::array_type_t::NONE:
        {
            array_length = 1;
            break;
        }
        case definition_t::array_type_t::FIXED_LENGTH:
        {
            array_length = definition_tree.definition.array_length();
            break;
        }
        case definition_t::array_type_t::VARIABLE_LENGTH:
        {
            // Read the length, converting from little endian.
            array_length = le32toh(*reinterpret_cast<uint32_t*>(&message_definition::m_bytes[current_position]));
            // Update current position to account for the length bytes;
            current_position += 4;
            // Update the definition's array length.
            definition_tree.definition.update_array_length(array_length);
            break;
        }
    }

    // Update the current position.
    if(definition_tree.definition.is_primitive())
    {
        // Check if string.
        if(definition_tree.definition.primitive_type() == definition_t::primitive_type_t::STRING)
        {
            // Read possibly repeated string.
            for(uint32_t a = 0; a < array_length; ++a)
            {
                // Read string length, converting from little endian.
                uint32_t string_length = le32toh(*reinterpret_cast<uint32_t*>(&message_definition::m_bytes[current_position]));
                // Update current position.
                current_position += 4 + string_length;
            }
        }
        else
        {
            // Update current position with possibly repeated type.
            current_position += array_length * definition_tree.definition.size();
        }
    }
    else
    {
        // Definition tree is not a primitive type, and thus has fields.

        // Update current position over this possibly repeated field.
        for(uint32_t a = 0; a < array_length; ++a)
        {
            // Iterate through fields.
            for(auto field = definition_tree.fields.begin(); field != definition_tree.fields.end(); ++field)
            {
                // Recurse into field.
                message_definition::update_positions(*field, current_position);
            }
        }
    }
}
void message_definition::print_definition_tree(std::stringstream& stream, const definition_tree_t& definition_tree, uint32_t level) const
{
    // Print definition's info in one line.
    stream << "name = " << definition_tree.definition.name() << " type = " << definition_tree.definition.type() << " size = " << definition_tree.definition.size() << " array = " << definition_tree.definition.array() << std::endl;

    // Print sub definitions indented.
    for(auto field = definition_tree.fields.begin(); field != definition_tree.fields.end(); ++field)
    {
        for(uint32_t i = 0; i <= level; ++i)
        {
            stream << "\t";
        }
        message_definition::print_definition_tree(stream, *field, level+1);
    }
}