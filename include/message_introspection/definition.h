/// \file message_introspection/definition.h
/// \brief Defines the message_introspection::definition_t class.
#ifndef MESSAGE_INTROSPECTION___DEFINITION_H
#define MESSAGE_INTROSPECTION___DEFINITION_H

#include <string>

namespace message_introspection {

/// \brief A definition of a message.
class definition_t
{
public:
    // CONSTRUCTOR
    /// \brief Creates a new definition instance.
    /// \param type The definition's type string.
    /// \param array The definition's array type string.
    /// \param name The definition's name string.
    definition_t(const std::string& type, const std::string& array, const std::string& name);

    // ENUMS
    /// \brief An enumeration of primitive types.
    enum class primitive_type_t
    {
        NON_PRIMITIVE = 0,
        BOOL = 1,
        INT8 = 2,
        INT16 = 3,
        INT32 = 4,
        INT64 = 5,
        UINT8 = 6,
        UINT16 = 7,
        UINT32 = 8,
        UINT64 = 9,
        FLOAT32 = 10,
        FLOAT64 = 11,
        STRING = 12,
        TIME = 13,
        DURATION = 14
    };
    /// \brief An enumeration of array types.
    enum class array_type_t
    {
        NONE = 0,
        FIXED_LENGTH = 1,
        VARIABLE_LENGTH = 2,
    };

    // TYPE
    /// \brief Gets the type string of the definition.
    /// \returns The type as a string.
    std::string type();
    /// \brief Indicates if the definition is a primitive type.
    /// \returns TRUE if the definition is a primitive type, otherwise FALSE.
    bool is_primitive();
    /// \brief Gets the primitive type of the definition.
    /// \returns The primitive type of the definition.
    primitive_type_t primitive_type();
    /// \brief Gets the size of the definition in bytes.
    /// \returns The size of the definition.
    uint32_t size();

    // ARRAY
    /// \brief Indicates if the definition is an array.
    /// \returns TRUE if the definition is an array, otherwise FALSE.
    bool is_array();
    /// \brief Gets the array type of the definition.
    /// \returns The array type of the definition.
    array_type_t array_type();
    /// \brief Gets the size of the definitions array.
    /// \returns The size of the array.
    uint32_t array_size();

    // NAME
    /// \brief Gets the name of the definition.
    /// \returns The name of the definition.
    std::string name();

    // UPDATE
    /// \brief Updates the type of the definition.
    /// \param new_type The new type string to assign.
    void update_type(const std::string& new_type);
    /// \brief Updates the size of the definition.
    /// \param new_size The new size of the definition in bytes.
    void update_size(uint32_t new_size);

private:
    // TYPE
    /// \brief The definition's type.
    std::string m_type;
    /// \brief The definition's primitive type.
    primitive_type_t m_primitive_type;
    /// \brief The definition's size in bytes.
    uint32_t m_size;

    // ARRAY
    /// \brief The definition's array type.
    array_type_t m_array_type;
    /// \brief The definition's array size.
    uint32_t m_array_size;

    // NAME
    /// \brief The definition's name.
    std::string m_name;
};

}

#endif