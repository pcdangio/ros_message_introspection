/// \file message_definition.h
/// \brief Defines the message_introspection::message_definition class.
#ifndef MESSAGE_INTROSPECTION___MESSAGE_DEFINITION_H
#define MESSAGE_INTROSPECTION___MESSAGE_DEFINITION_H

#include "message_introspection/definition.h"
#include "message_introspection/definition_tree.h"

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
    std::string print_definition_tree() const;

    // LISTING
    /// \brief Gets a copy of the message's definition tree.
    /// \returns The message's current definition tree.
    definition_tree_t definition_tree() const;
    /// \brief Lists the fields under a parent path.
    /// \param fields The field info vector to capture the results in.
    /// \param parent_path The absolute path of the parent field delimited by '.'
    /// \returns TRUE if the parent path exists, otherwise FALSE.
    /// \details This method only populates the fields output if the path exists.
    /// An example parent path is pose.position
    bool list_fields(std::vector<definition_t>& fields, std::string parent_path = "") const;
    /// \brief Gets the description of a single field.
    /// \param field_info The field info object to capture the result in.
    /// \param path The absolute path of the field to get the description of.
    /// \returns TRUE if the path exists, otherwise FALSE.
    /// \details This method only populates the field_info output if the path exists.
    /// An example path is pose.position.x
    bool field_info(definition_t& field_info, const std::string& path) const;

    void new_message(const std::vector<uint8_t>& serialized_data);
    bool get_field(double& value, const std::string& path) const;

private:
    // COMPONENTS
    /// \brief A map of component message definitions (top level only)
    std::unordered_map<std::string, std::vector<definition_t>> m_component_definitions;
    /// \brief Parses a message definition string into the component definition map.
    /// \param message_type The ROS message type string.
    /// \param message_definition The ROS message definition string.
    void parse_components(std::string message_type, std::string message_definition);

    // DEFINITION
    /// \brief The message's calculated definition tree.
    definition_tree_t m_definition_tree;
    /// \brief A recursive method for adding new definitions to the definition tree.
    /// \param parent_path The parent path of the definition tree being added.
    /// \param definition_tree A reference to the definition tree to add to.
    /// \param component_definition The component information to add to the definition's sub tree.
    void add_definition(const std::string& parent_path, definition_tree_t& definition_tree, const definition_t& component_definition);
    /// \brief A recursive method for printing a definition tree to a stringstream.
    /// \param stream The output stream to print to.
    /// \param definition The definition tree to print.
    /// \param level The current depth of the tree.
    void print_definition_tree(std::stringstream& stream, const definition_tree_t& definition_tree, uint32_t level) const;

    // LISTING
    /// \brief A method for getting the definition tree of a specified path.
    /// \param path The path to get the definition tree of.
    /// \returns If the path exists, a pointer to the definition tree, otherwise nullptr.
    const definition_tree_t* get_definition_tree(const std::string& path) const;
};

}

#endif