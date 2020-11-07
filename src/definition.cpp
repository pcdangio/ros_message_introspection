#include "message_introspection/definition.h"

using namespace message_introspection;

// CONSTRUCTOR
definition_t::definition_t()
{
    definition_t::m_type = "";
    definition_t::m_primitive_type = definition_t::primitive_type_t::NON_PRIMITIVE;
    definition_t::m_size = 0;
    definition_t::m_array = "";
    definition_t::m_array_type = definition_t::array_type_t::NONE;
    definition_t::m_array_length = 0;
    definition_t::m_name = "";
    definition_t::m_path = "";
}
definition_t::definition_t(const std::string& type, const std::string& array, const std::string& name)
{
    // Update the definition's type.
    definition_t::update_type(type);

    // Store the array.
    definition_t::m_array = array;
    // Parse the array.
    if(array.empty())
    {
        definition_t::m_array_type = definition_t::array_type_t::NONE;
        definition_t::m_array_length = 0;
    }
    else if(array.compare("[]") == 0)
    {
        // Array is variable length.
        definition_t::m_array_type = definition_t::array_type_t::VARIABLE_LENGTH;
        definition_t::m_array_length = 0;
    }
    else
    {
        // Array is fixed length.
        definition_t::m_array_type = definition_t::array_type_t::FIXED_LENGTH;
        // Parse out the array size.
        auto end_bracket_position = array.find_first_of(']');
        definition_t::m_array_length = std::stoul(array.substr(1, end_bracket_position - 1));
    }
    
    // Store the name.
    definition_t::m_name = name;

    // Initialize the path.
    definition_t::update_parent_path("");
}

// TYPE
std::string definition_t::type() const
{
    return definition_t::m_type;
}
bool definition_t::is_primitive() const
{
    return definition_t::m_primitive_type != definition_t::primitive_type_t::NON_PRIMITIVE;
}
definition_t::primitive_type_t definition_t::primitive_type() const
{
    return definition_t::m_primitive_type;
}
uint32_t definition_t::size() const
{
    return definition_t::m_size;
}

// ARRAY
std::string definition_t::array() const
{
    return definition_t::m_array;
}
bool definition_t::is_array() const
{
    return definition_t::m_array_type != definition_t::array_type_t::NONE;
}
definition_t::array_type_t definition_t::array_type() const
{
    return definition_t::m_array_type;
}
uint32_t definition_t::array_length() const
{
    return definition_t::m_array_length;
}

// NAME
std::string definition_t::name() const
{
    return definition_t::m_name;
}

// PATH
std::string definition_t::path() const
{
    return definition_t::m_path;
}

// UPDATE
void definition_t::update_name(const std::string& name)
{
    definition_t::m_name = name;
}
void definition_t::update_type(const std::string& type)
{
    // Store the type.
    definition_t::m_type = type;
    // Parse the type.
    if(type.compare("bool") == 0)
    {
        definition_t::m_primitive_type = definition_t::primitive_type_t::BOOL;
        definition_t::m_size = 1;
    }
    else if(type.compare("int8") == 0 || type.compare("byte") == 0)
    {
        definition_t::m_primitive_type = definition_t::primitive_type_t::INT8;
        definition_t::m_size = 1;
    }
    else if(type.compare("int16") == 0)
    {
        definition_t::m_primitive_type = definition_t::primitive_type_t::INT16;
        definition_t::m_size = 2;
    }
    else if(type.compare("int32") == 0)
    {
        definition_t::m_primitive_type = definition_t::primitive_type_t::INT32;
        definition_t::m_size = 4;
    }
    else if(type.compare("int64") == 0)
    {
        definition_t::m_primitive_type = definition_t::primitive_type_t::INT64;
        definition_t::m_size = 8;
    }
    else if(type.compare("uint8") == 0 || type.compare("char") == 0)
    {
        definition_t::m_primitive_type = definition_t::primitive_type_t::UINT8;
        definition_t::m_size = 1;
    }
    else if(type.compare("uint16") == 0)
    {
        definition_t::m_primitive_type = definition_t::primitive_type_t::UINT16;
        definition_t::m_size = 2;
    }
    else if(type.compare("uint32") == 0)
    {
        definition_t::m_primitive_type = definition_t::primitive_type_t::UINT32;
        definition_t::m_size = 4;
    }
    else if(type.compare("uint64") == 0)
    {
        definition_t::m_primitive_type = definition_t::primitive_type_t::UINT64;
        definition_t::m_size = 8;
    }
    else if(type.compare("float32") == 0)
    {
        definition_t::m_primitive_type = definition_t::primitive_type_t::FLOAT32;
        definition_t::m_size = 4;
    }
    else if(type.compare("float64") == 0)
    {
        definition_t::m_primitive_type = definition_t::primitive_type_t::FLOAT64;
        definition_t::m_size = 8;
    }
    else if(type.compare("string") == 0)
    {
        definition_t::m_primitive_type = definition_t::primitive_type_t::STRING;
        definition_t::m_size = 0;
    }
    else if(type.compare("time") == 0)
    {
        definition_t::m_primitive_type = definition_t::primitive_type_t::TIME;
        definition_t::m_size = 8;
    }
    else if(type.compare("duration") == 0)
    {
        definition_t::m_primitive_type = definition_t::primitive_type_t::DURATION;
        definition_t::m_size = 8;
    }
    else
    {
        definition_t::m_primitive_type = definition_t::primitive_type_t::NON_PRIMITIVE;
        definition_t::m_size = 0;
    }
}
void definition_t::update_size(uint32_t size)
{
    // Update the size.
    definition_t::m_size = size;
}
void definition_t::update_array_length(uint32_t length)
{
    definition_t::m_array_length = length;
}
void definition_t::update_parent_path(const std::string& parent_path)
{
    if(!parent_path.empty())
    {
        definition_t::m_path = parent_path + "." + definition_t::m_name;
    }
    else
    {
        definition_t::m_path = definition_t::m_name;
    }
}