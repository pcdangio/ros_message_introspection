/// \file message_introspection/definition_tree.h
/// \brief Defines the message_introspection::definition_tree_t class.
#ifndef MESSAGE_INTROSPECTION___DEFINITION_TREE_H
#define MESSAGE_INTROSPECTION___DEFINITION_TREE_H

#include <message_introspection/definition.h>

#include <vector>
#include <sstream>

namespace message_introspection {

/// \brief A message definition tree.
class definition_tree_t
{
public:
    /// \brief The message's definition.
    definition_t definition;
    /// \brief The fields belonging to this message.
    std::vector<definition_tree_t> fields;

    /// \brief Checks if a path exists in the definition tree.
    /// \param path The relative path in the tree to check existence for.
    /// \returns TRUE if the path exists, otherwise false.
    /// \note Array indices and bounds are ignored.
    bool path_exists(const std::string& path) const;
    /// \brief Checks if any component of a path is an array.
    /// \param path The relative path in the tree to check for array components.
    /// \returns TRUE if the path exists and has array components, otherwise FALSE.
    bool path_has_arrays(const std::string& path) const;
    /// \brief Gets the definition for a path in the tree.
    /// \param path The relative path in the tree to get the definition for.
    /// \param definition The definition instance to store the result in.
    /// \returns TRUE if succeeded, otherwise FALSE when the path does not exist.
    bool get_path_definition(const std::string& path, definition_t& definition) const;
    /// \brief Gets an ordered list of definitions from a path in the tree.
    /// \param path The relative path in the tree to get definitions for.
    /// \param definitions The vector to store ordered definitions in.
    /// \returns TRUE if succeeded, otherwise FALSE when path does not exist.
    bool get_path_definitions(const std::string& path, std::vector<definition_t>& definitions) const;
    /// \brief Prints the definition tree to a string.
    /// \returns The definition tree as a string.
    std::string print() const;

private:
    /// \brief Splits a path up into it's component pieces.
    /// \param path The path to tokenize.
    /// \returns A vector of the path components.
    /// \note This method removes any array indicators.
    std::vector<std::string> tokenize_path(const std::string& path) const;
    /// \brief A recursive method for printing a definition tree to a stringstream.
    /// \param stream The output stream to print to.
    /// \param definition_tree The definition tree to print.
    /// \param level The current depth of the tree.
    void print(std::stringstream& stream, const definition_tree_t* definition_tree, uint32_t level) const;
};

}

#endif