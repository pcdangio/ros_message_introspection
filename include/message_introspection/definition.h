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
    // CONSTRUCTORS
    /// \brief Creates an empty definition instance.
    definition_t();
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
    std::string type() const;
    /// \brief Indicates if the definition is a primitive type.
    /// \returns TRUE if the definition is a primitive type, otherwise FALSE.
    bool is_primitive() const;
    /// \brief Gets the primitive type of the definition.
    /// \returns The primitive type of the definition.
    primitive_type_t primitive_type() const;
    /// \brief Gets the size of the definition in bytes.
    /// \returns The size of the definition.
    uint32_t size() const;

    // ARRAY
    /// \brief Gets the array type as a string.
    /// \returns The array type.
    std::string array() const;
    /// \brief Indicates if the definition is an array.
    /// \returns TRUE if the definition is an array, otherwise FALSE.
    bool is_array() const;
    /// \brief Gets the array type of the definition.
    /// \returns The array type of the definition.
    array_type_t array_type() const;
    /// \brief Gets the size of the definitions array.
    /// \returns The size of the array.
    uint32_t array_length() const;

    // NAME
    /// \brief Gets the name of the definition.
    /// \returns The name of the definition.
    std::string name() const;

    // PATH
    /// \brief Gets the definition's full path.
    /// \returns The full path of the definition.
    std::string path() const;

    // POSITION
    /// \brief Gets the definition's serialized byte position in the message.
    /// \returns The serialized byte position.
    uint32_t serialized_position() const;

    // UPDATE
    /// \brief Updates the definition's type.
    /// \param type The new type string to assign.
    void update_type(const std::string& type);
    /// \brief Updates the size of the definition in bytes.
    /// \param size The new size of the definition in bytes.
    void update_size(uint32_t size);
    /// \brief Updates the definition's array length.
    /// \param length The new length to set.
    void update_array_length(uint32_t length);
    /// \brief Updates the definition's parent path.
    /// \returns The definition's parent path.
    void update_parent_path(const std::string& parent_path);
    /// \brief Updates the definition's serialized byte position.
    /// \param position The new position.
    void update_serialized_position(uint32_t position);

private:
    // TYPE
    /// \brief The definition's type string.
    std::string m_type;
    /// \brief The definition's primitive type.
    primitive_type_t m_primitive_type;
    /// \brief The definition's size in bytes.
    uint32_t m_size;

    // ARRAY
    /// \brief The definition's array type string.
    std::string m_array;
    /// \brief The definition's array type.
    array_type_t m_array_type;
    /// \brief The definition's array length.
    uint32_t m_array_length;

    // NAME
    /// \brief The definition's name.
    std::string m_name;

    // PATH
    /// \brief The definition's full path.
    std::string m_path;

    // POSITION
    /// \brief The position of this definition in the serialized byte array.
    uint32_t m_serialized_position;
};

}

#endif