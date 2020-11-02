#include "message_introspection/introspector.h"

#include <boost/tokenizer.hpp>
#include <endian.h>

using namespace message_introspection;

// CONSTRUCTORS
introspector::introspector()
{
    // Initialize serialized bytes.
    introspector::m_bytes = nullptr;
}
introspector::~introspector()
{
    // Clean up message bytes.
    delete [] introspector::m_bytes;
}

// MESSAGE
void introspector::new_message(const topic_tools::ShapeShifter& message)
{
    // First register the message if it hasn't been registered already.
    if(!introspector::is_registered(message.getMD5Sum()))
    {
        // Register message.
        introspector::register_message(message.getMD5Sum(), message.getDataType(), message.getMessageDefinition());
    }

    // Clear old data.
    delete [] introspector::m_bytes;

    // Get serialized length and set up bytes for capture.
    uint32_t message_length = ros::serialization::serializationLength(message);
    introspector::m_bytes = new uint8_t[message_length];

    // Serialize data into byte storage.
    ros::serialization::OStream stream(introspector::m_bytes, message_length);
    ros::serialization::serialize(stream, message);

    // Update field map.
    introspector::m_field_map.clear();
    std::string current_path = "";
    uint32_t current_position = 0;
    introspector::update_field_map(introspector::m_definition_tree, current_path, current_position);
}
void introspector::new_message(const rosbag::MessageInstance& message)
{
    // First register the message if it hasn't been registered already.
    if(!introspector::is_registered(message.getMD5Sum()))
    {
        // Register message.
        introspector::register_message(message.getMD5Sum(), message.getDataType(), message.getMessageDefinition());
    }

    // Clear old data.
    delete [] introspector::m_bytes;

    // Copy serialized bytes to this instance.
    introspector::m_bytes = new uint8_t[message.size()];
    ros::serialization::OStream stream(introspector::m_bytes, message.size());
    message.write(stream);

    // Update field map.
    introspector::m_field_map.clear();
    std::string current_path = "";
    uint32_t current_position = 0;
    introspector::update_field_map(introspector::m_definition_tree, current_path, current_position);
}
void introspector::register_message(const std::string& md5, const std::string& type, const std::string& definition)
{
    // Extract message component types.
    introspector::parse_components(type, definition);

    // Add top level message to the definition, and let recursion handle the rest.
    introspector::m_definition_tree = definition_tree_t();
    introspector::add_definition("", introspector::m_definition_tree, definition_t(type, "", ""));

    // Store MD5.
    introspector::m_md5 = md5;
}
bool introspector::is_registered(const std::string& md5)
{
    return (introspector::m_md5.compare(md5) == 0);
}

// GET
bool introspector::path_exists(const std::string& path) const
{
    return introspector::m_field_map.count(path);
}
bool introspector::get_bool(const std::string& path, bool& value) const
{
    try
    {
        // Get field info from map.
        auto field_info = introspector::m_field_map.at(path);

        // Check field type.
        if(field_info.primitive_type != definition_t::primitive_type_t::BOOL)
        {
            return false;
        }

        // Extract value.
        value = *reinterpret_cast<bool*>(&(introspector::m_bytes[field_info.position]));
    }
    catch(...)
    {
        return false;
    }

    return true;
}
bool introspector::get_int8(const std::string& path, int8_t& value) const
{
    try
    {
        // Get field info from map.
        auto field_info = introspector::m_field_map.at(path);

        // Check field type.
        if(field_info.primitive_type != definition_t::primitive_type_t::INT8)
        {
            return false;
        }

        // Extract value.
        value = *reinterpret_cast<int8_t*>(&(introspector::m_bytes[field_info.position]));
    }
    catch(...)
    {
        return false;
    }

    return true;
}
bool introspector::get_int16(const std::string& path, int16_t& value) const
{
    try
    {
        // Get field info from map.
        auto field_info = introspector::m_field_map.at(path);

        // Check field type.
        if(field_info.primitive_type != definition_t::primitive_type_t::INT16)
        {
            return false;
        }

        // Extract value.
        value = le16toh(*reinterpret_cast<int16_t*>(&(introspector::m_bytes[field_info.position])));
    }
    catch(...)
    {
        return false;
    }

    return true;
}
bool introspector::get_int32(const std::string& path, int32_t& value) const
{
    try
    {
        // Get field info from map.
        auto field_info = introspector::m_field_map.at(path);

        // Check field type.
        if(field_info.primitive_type != definition_t::primitive_type_t::INT32)
        {
            return false;
        }

        // Extract value.
        value = le32toh(*reinterpret_cast<int32_t*>(&(introspector::m_bytes[field_info.position])));
    }
    catch(...)
    {
        return false;
    }

    return true;
}
bool introspector::get_int64(const std::string& path, int64_t& value) const
{
    try
    {
        // Get field info from map.
        auto field_info = introspector::m_field_map.at(path);

        // Check field type.
        if(field_info.primitive_type != definition_t::primitive_type_t::INT64)
        {
            return false;
        }

        // Extract value.
        value = le64toh(*reinterpret_cast<int64_t*>(&(introspector::m_bytes[field_info.position])));
    }
    catch(...)
    {
        return false;
    }

    return true;
}
bool introspector::get_uint8(const std::string& path, uint8_t& value) const
{
    try
    {
        // Get field info from map.
        auto field_info = introspector::m_field_map.at(path);

        // Check field type.
        if(field_info.primitive_type != definition_t::primitive_type_t::UINT8)
        {
            return false;
        }

        // Extract value.
        value = *reinterpret_cast<uint8_t*>(&(introspector::m_bytes[field_info.position]));
    }
    catch(...)
    {
        return false;
    }

    return true;
}
bool introspector::get_uint16(const std::string& path, uint16_t& value) const
{
    try
    {
        // Get field info from map.
        auto field_info = introspector::m_field_map.at(path);

        // Check field type.
        if(field_info.primitive_type != definition_t::primitive_type_t::UINT16)
        {
            return false;
        }

        // Extract value.
        value = le16toh(*reinterpret_cast<uint16_t*>(&(introspector::m_bytes[field_info.position])));
    }
    catch(...)
    {
        return false;
    }

    return true;
}
bool introspector::get_uint32(const std::string& path, uint32_t& value) const
{
    try
    {
        // Get field info from map.
        auto field_info = introspector::m_field_map.at(path);

        // Check field type.
        if(field_info.primitive_type != definition_t::primitive_type_t::UINT32)
        {
            return false;
        }

        // Extract value.
        value = le32toh(*reinterpret_cast<uint32_t*>(&(introspector::m_bytes[field_info.position])));
    }
    catch(...)
    {
        return false;
    }

    return true;
}
bool introspector::get_uint64(const std::string& path, uint64_t& value) const
{
    try
    {
        // Get field info from map.
        auto field_info = introspector::m_field_map.at(path);

        // Check field type.
        if(field_info.primitive_type != definition_t::primitive_type_t::UINT64)
        {
            return false;
        }

        // Extract value.
        value = le64toh(*reinterpret_cast<uint64_t*>(&(introspector::m_bytes[field_info.position])));
    }
    catch(...)
    {
        return false;
    }

    return true;
}
bool introspector::get_float32(const std::string& path, float& value) const
{
    try
    {
        // Get field info from map.
        auto field_info = introspector::m_field_map.at(path);

        // Check field type.
        if(field_info.primitive_type != definition_t::primitive_type_t::FLOAT32)
        {
            return false;
        }

        // Extract value.
        value = le32toh(*reinterpret_cast<float*>(&(introspector::m_bytes[field_info.position])));
    }
    catch(...)
    {
        return false;
    }

    return true;
}
bool introspector::get_float64(const std::string& path, double& value) const
{
    try
    {
        // Get field info from map.
        auto field_info = introspector::m_field_map.at(path);

        // Check field type.
        if(field_info.primitive_type != definition_t::primitive_type_t::FLOAT64)
        {
            return false;
        }

        // Extract value.
        value = le64toh(*reinterpret_cast<double*>(&(introspector::m_bytes[field_info.position])));
    }
    catch(...)
    {
        return false;
    }

    return true;
}
bool introspector::get_string(const std::string& path, std::string& value) const
{
    try
    {
        // Get field info from map.
        auto field_info = introspector::m_field_map.at(path);

        // Check field type.
        if(field_info.primitive_type != definition_t::primitive_type_t::STRING)
        {
            return false;
        }

        // Read the strings length.
        uint32_t string_length = le32toh(*reinterpret_cast<uint32_t*>(&introspector::m_bytes[field_info.position]));

        // Read the string.
        value = std::string(reinterpret_cast<char*>(&(introspector::m_bytes[field_info.position + 4])), string_length);
    }
    catch(...)
    {
        return false;
    }

    return true;
}
bool introspector::get_time(const std::string& path, ros::Time& value) const
{
    try
    {
        // Get field info from map.
        auto field_info = introspector::m_field_map.at(path);

        // Check field type.
        if(field_info.primitive_type != definition_t::primitive_type_t::TIME)
        {
            return false;
        }

        // Extract secs and nsecs.
        value.sec = le32toh(*reinterpret_cast<uint32_t*>(&(introspector::m_bytes[field_info.position])));
        value.nsec = le32toh(*reinterpret_cast<uint32_t*>(&(introspector::m_bytes[field_info.position + 4])));
    }
    catch(...)
    {
        return false;
    }

    return true;
}
bool introspector::get_duration(const std::string& path, ros::Duration& value) const
{
    try
    {
        // Get field info from map.
        auto field_info = introspector::m_field_map.at(path);

        // Check field type.
        if(field_info.primitive_type != definition_t::primitive_type_t::TIME)
        {
            return false;
        }

        // Extract secs and nsecs.
        value.sec = le32toh(*reinterpret_cast<uint32_t*>(&(introspector::m_bytes[field_info.position])));
        value.nsec = le32toh(*reinterpret_cast<uint32_t*>(&(introspector::m_bytes[field_info.position + 4])));
    }
    catch(...)
    {
        return false;
    }

    return true;
}

// PRINTING
std::string introspector::print_components() const
{
    // Create output stream.
    std::stringstream output;

    // Iterate over component definitions.
    for(auto component = introspector::m_component_definitions.begin(); component != introspector::m_component_definitions.end(); ++component)
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
std::string introspector::print_definition_tree() const
{
    // Create output stream.
    std::stringstream output;

    // Recursively print the tree.
    introspector::print_definition_tree(output, introspector::m_definition_tree, 0);

    return output.str();
}

// COMPONENTS
void introspector::parse_components(std::string message_type, std::string introspector)
{
    // Clear current component definitions.
    introspector::m_component_definitions.clear();

    // Add top-level message to definition and set it as the current workspace.
    auto* fields_workspace = &introspector::m_component_definitions[message_type];

    // Convert description into a stringstream.
    std::stringstream description_stream(introspector);

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
            fields_workspace = &introspector::m_component_definitions[tokens[1]];
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
    for(auto definition = introspector::m_component_definitions.begin(); definition != introspector::m_component_definitions.end(); ++definition)
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
            if(introspector::m_component_definitions.count(field->type()) == 0)
            {
                // Exact typename not found. Search through definitions to find the matching full type name.
                for(auto candidate = introspector::m_component_definitions.begin(); candidate != introspector::m_component_definitions.end(); ++ candidate)
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
void introspector::add_definition(const std::string& parent_path, definition_tree_t& definition_tree, const definition_t& component_definition)
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
        auto fields = introspector::m_component_definitions[component_definition.type()];
        // Iterate through each field to add it recursively to the definition.
        for(auto field = fields.begin(); field != fields.end(); ++field)
        {
            // Add a new definition to the array BEFORE populating it for copy efficiency.
            definition_tree.fields.push_back(definition_tree_t());
            auto& field_reference = definition_tree.fields.back();
            // Now add the details to the field recursively and in place.
            introspector::add_definition(definition_tree.definition.path(), field_reference, *field);
            // Add field's computed size to the definition's size.
            total_size += field_reference.definition.size();
        }

        // Update the top level size.
        definition_tree.definition.update_size(total_size);
    }
}
definition_tree_t introspector::definition_tree() const
{
    return introspector::m_definition_tree;
}
void introspector::print_definition_tree(std::stringstream& stream, const definition_tree_t& definition_tree, uint32_t level) const
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
        introspector::print_definition_tree(stream, *field, level+1);
    }
}

// POSITIONING
void introspector::update_field_map(const definition_tree_t& definition_tree, const std::string& current_path, uint32_t& current_position)
{
    // Build the path to this tree.
    std::string path = current_path;
    if(!path.empty())
    {
        path += ".";
    }
    path += definition_tree.definition.name();

    // This tree may or may not be an array.
    // Use array information to determine number of instances.
    uint32_t instances;
    switch(definition_tree.definition.array_type())
    {
        case definition_t::array_type_t::NONE:
        {
            instances = 1;
            break;
        }
        case definition_t::array_type_t::FIXED_LENGTH:
        {
            instances = definition_tree.definition.array_length();
            break;
        }
        case definition_t::array_type_t::VARIABLE_LENGTH:
        {
            // Read the length, converting from little endian.
            instances = le32toh(*reinterpret_cast<uint32_t*>(&introspector::m_bytes[current_position]));
            // Update current position to account for the length bytes;
            current_position += 4;
            break;
        }
    }

    // Check if this tree is primitive or not.
    if(definition_tree.definition.is_primitive())
    {
        // Iterate over instances.
        for(uint32_t i = 0; i < instances; ++i)
        {
            // Build path for this instance.
            std::string instance_path = path;
            if(definition_tree.definition.is_array())
            {
                instance_path += "[" + std::to_string(i) + "]";
            }

            // Add instance to field map.
            introspector::m_field_map[instance_path] = {current_position, definition_tree.definition.primitive_type()};

            // Check if string.
            if(definition_tree.definition.primitive_type() == definition_t::primitive_type_t::STRING)
            {
                // Read string length, converting from little endian.
                uint32_t string_length = le32toh(*reinterpret_cast<uint32_t*>(&introspector::m_bytes[current_position]));
                // Update current position.
                current_position += 4 + string_length;
            }
            else
            {
                // Update current position with this primitive type's size.
                current_position += definition_tree.definition.size();
            }
        }
    }
    else
    {
        // Tree is not a primitive type, and thus has children.
        // For each instance, recurse into that instance's children.
        for(uint32_t i = 0; i < instances; ++i)
        {
            // Build path for this instance.
            std::string instance_path = path;
            if(definition_tree.definition.is_array())
            {
                instance_path += "[" + std::to_string(i) + "]";
            }

            // Iterate through fields.
            for(auto field = definition_tree.fields.begin(); field != definition_tree.fields.end(); ++field)
            {
                // Recurse into field.
                // Use a reset current position as they are all relative to their parent.
                introspector::update_field_map(*field, instance_path, current_position);
            }
        }
    }
}