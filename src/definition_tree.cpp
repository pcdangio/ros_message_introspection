#include "message_introspection/definition_tree.h"

#include <boost/tokenizer.hpp>

using namespace message_introspection;

// PATH
bool definition_tree_t::get_path_definitions(const std::string& path, std::vector<definition_t> definitions) const
{
    // Track the current tree level, starting at the root.
    const definition_tree_t* current_tree = this;

    // Tokenize the path.
    boost::char_separator<char> delimiter(".");
    boost::tokenizer<boost::char_separator<char>> tokenizer(path, delimiter);
    std::vector<std::string> path_parts(tokenizer.begin(), tokenizer.end());

    // Iterate through the path to build a new path with array indices.
    for(auto path_part = path_parts.cbegin(); path_part != path_parts.cend(); ++path_part)
    {
        // Find the matching field by name and grab it's definition.
        bool definition_found = false;
        for(auto field = current_tree->fields.cbegin(); field != current_tree->fields.cend(); ++field)
        {
            if(field->definition.name().compare(*path_part) == 0)
            {
                // Definition found.
                
                // Add the definition to the output.
                definitions.push_back(field->definition);

                // Flag that it was found.
                definition_found = true;

                // Update the current_tree to this field.
                current_tree = &(*field);

                // Stop the search in this tree's children.
                break;
            }
        }

        // Check if definition was not found.
        if(!definition_found)
        {
            definitions.clear();
            return false;
        }
    }

    return true;
}

// PRINT
std::string definition_tree_t::print() const
{
    // Create output stream.
    std::stringstream output;

    // Recursively print the tree.
    definition_tree_t::print(output, this, 0);

    return output.str();
}
void definition_tree_t::print(std::stringstream& stream, const definition_tree_t* definition_tree, uint32_t level) const
{
    // Print definition's info in one line.
    stream << "name = " << definition_tree->definition.name() << " type = " << definition_tree->definition.type() << " size = " << definition_tree->definition.size() << " array = " << definition_tree->definition.array() << std::endl;

    // Print sub definitions indented.
    for(auto field = definition_tree->fields.begin(); field != definition_tree->fields.end(); ++field)
    {
        for(uint32_t i = 0; i <= level; ++i)
        {
            stream << "\t";
        }
        definition_tree_t::print(stream, &(*field), level+1);
    }
}