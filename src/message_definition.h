/// \file message_definition.h
/// \brief Defines the message_introspection::message_definition class.
#ifndef MESSAGE_INTROSPECTION___MESSAGE_DEFINITION_H
#define MESSAGE_INTROSPECTION___MESSAGE_DEFINITION_H

#include "message_introspection/field_info.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

namespace message_introspection {

/// \brief Parses and provides the defintion of a message.
class message_definition
{
public:
    // CONSTRUCTORS
    /// \brief Parses a message definition into a new instance.
    /// \param message_type The type string of the ROS message.
    /// \param message_definition The definition string of the ROS message.
    message_definition(const std::string& message_type, const std::string& message_definition);

    // PRINTING
    /// \brief Prints the message's component definitions to a string.
    /// \returns The component definitions.
    std::string print_components() const;
    /// \brief Prints the message's definition tree to a string.
    /// \returns The message's definition tree.
    std::string print_definition() const;

    // LISTING
    /// \brief Lists the fields under a parent path.
    /// \param fields The field info vector to capture the results in.
    /// \param parent_path The absolute path of the parent field delimited by '.'
    /// \returns TRUE if the parent path exists, otherwise FALSE.
    /// \details This method only populates the fields output if the path exists.
    /// An example parent path is pose.position
    bool list_fields(std::vector<field_info_t>& fields, std::string parent_path = "") const;
    /// \brief Gets the description of a single field.
    /// \param field_info The field info object to capture the result in.
    /// \param path The absolute path of the field to get the description of.
    /// \returns TRUE if the path exists, otherwise FALSE.
    /// \details This method only populates the field_info output if the path exists.
    /// An example path is pose.position.x
    bool field_info(field_info_t& field_info, const std::string& path) const;

    void new_message(const std::vector<uint8_t>& serialized_data);
    bool get_field(double& value, const std::string& path) const;

private:
    // PRIMITIVES
    /// \brief A map of primitive type strings and their size information.
    std::unordered_map<std::string, uint32_t> m_primitive_types;
    /// \brief Checks if a type is a primitive type.
    bool is_primitive_type(const std::string& type) const;

    // COMPONENTS
    /// \brief A component description.
    struct component_t
    {
        /// \brief The component's type.
        std::string type;
        /// \brief The component's array designation.
        std::string array;
        /// \brief The component's field name.
        std::string name;

        /// \brief Indicates if the definition is a primitive type.
        bool is_primitive;
        /// \brief Indicates if the definition's type is a string.
        bool is_string;
        /// \brief Indicates if the definition is an array.
        bool is_array;
        /// \brief The size of the array. 0 indicates variable size.
        uint32_t array_length;
    };
    /// \brief A map of component message definitions (top level only)
    std::unordered_map<std::string, std::vector<component_t>> m_component_definitions;
    /// \brief Parses a message definition string into the component definition map.
    /// \param message_type The ROS message type string.
    /// \param message_definition The ROS message definition string.
    void parse_components(std::string message_type, std::string message_definition);

    // DEFINITION
    /// \brief A message definition tree.
    struct definition_t
    {
        /// \brief The definition's type.
        std::string type;
        /// \brief The definition's array designation.
        std::string array;
        /// \brief The definition's field name.
        std::string name;

        /// \brief Indicates if the definition is a primitive type.
        bool is_primitive;
        /// \brief Indicates if the definition's type is a string.
        bool is_string;
        /// \brief Indicates if the definition is an array.
        bool is_array;
        /// \brief The size of the array. 0 indicates variable size.
        uint32_t array_length;

        /// \brief The size of the definition, in bytes.
        uint32_t size;

        /// \brief The fields belonging to this definition.
        std::vector<definition_t> fields;
    };
    /// \brief The message's calculated definition tree.
    definition_t m_definition;
    /// \brief A recursive method for adding new definitions to the definition tree.
    /// \param definition A reference to the definition being added.
    /// \param component_definition The component information to add to the definition.
    void add_definition(definition_t& definition, const component_t& component_definition);
    /// \brief A recursive method for printing a definition to a stringstream.
    /// \param stream The output stream to print to.
    /// \param definition The definition to print.
    /// \param level The current depth of the tree.
    void print_definition(std::stringstream& stream, const definition_t& definition, uint32_t level) const;

    // LISTING
    /// \brief A method for getting the definition of a specified path.
    /// \param path The path to get the definition of.
    /// \returns If the path exists, a pointer to the definition, otherwise nullptr.
    const definition_t* get_definition(const std::string& path) const;

    std::unordered_map<std::string, std::vector<uint8_t>> m_value_map;
};

}

#endif