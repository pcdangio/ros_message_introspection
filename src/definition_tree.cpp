#include "message_introspection/definition_tree.h"

#include <boost/tokenizer.hpp>

using namespace message_introspection;

// PATH
std::vector<std::string> definition_tree_t::tokenize_path(const std::string& path) const
{
    // Set up tokenizer.
    boost::char_separator<char> delimiter(".");
    boost::tokenizer<boost::char_separator<char>> tokenizer(path, delimiter);

    // Create output vector.
    std::vector<std::string> output;

    // Iterate through tokens to populate output while removing array indicators.
    for(auto token = tokenizer.begin(); token != tokenizer.end(); ++ token)
    {
        std::string path_part = *token;
        auto array_indicator = path_part.find_first_of('[');
        if(array_indicator != std::string::npos)
        {
            path_part.erase(array_indicator);
        }
        output.push_back(path_part);
    }
    
    return output;
}
bool definition_tree_t::path_exists(const std::string& path) const
{
    // Get the path components.
    // NOTE: the tokenize function already removes array indicators.
    auto path_parts = definition_tree_t::tokenize_path(path);

    // Iterate through the tree.
    const definition_tree_t* current_tree = this;
    for(auto path_part = path_parts.cbegin(); path_part != path_parts.cend(); ++path_part)
    {
        bool path_part_found = false;
        // Iterate through the tree's fields.
        for(auto field = current_tree->fields.cbegin(); field != current_tree->fields.cend(); ++field)
        {
            // Check if field matches the path part.
            if(field->definition.name().compare(*path_part) == 0)
            {
                // Path part exists.
                path_part_found = true;
                current_tree = &(*field);
                break;
            }
        }
        // Check if path_part was found in any of the fields.
        if(!path_part_found)
        {
            return false;
        }
    }

    // If this point reached, all path parts exist.
    return true;
}
bool definition_tree_t::path_has_arrays(const std::string& path) const
{
    // Get the path components.
    // NOTE: the tokenize function already removes array indicators.
    auto path_parts = definition_tree_t::tokenize_path(path);

    // Iterate through the tree.
    const definition_tree_t* current_tree = this;
    for(auto path_part = path_parts.cbegin(); path_part != path_parts.cend(); ++path_part)
    {
        bool path_part_found = false;
        // Iterate through the tree's fields.
        for(auto field = current_tree->fields.cbegin(); field != current_tree->fields.cend(); ++field)
        {
            // Check if field matches the path part.
            if(field->definition.name().compare(*path_part) == 0)
            {
                // Path part exists.
                // Check if the field is an array.
                if(field->definition.is_array())
                {
                    return true;
                }
                // Otherise, continue searching through tree.
                path_part_found = true;
                current_tree = &(*field);
                break;
            }
        }
        // Check if path_part was found in any of the fields.
        if(!path_part_found)
        {
            return false;
        }
    }

    // If this point reached, all path parts exist but no arrays were found.
    return false;
}
bool definition_tree_t::get_path_definition(const std::string& path, definition_t& definition) const
{
    // Get the path components.
    // NOTE: the tokenize function already removes array indicators.
    auto path_parts = definition_tree_t::tokenize_path(path);

    // Iterate through the path to get the final definition.
    const definition_tree_t* current_tree = this;
    for(auto path_part = path_parts.cbegin(); path_part != path_parts.cend(); ++path_part)
    {
        // Iterate through fields to find the matching path part.
        bool path_part_found = false;
        for(auto field = current_tree->fields.cbegin(); field != current_tree->fields.cend(); ++field)
        {
            if(field->definition.name().compare(*path_part) == 0)
            {
                // Path part found.
                // Store this definition as the output (last path part goes out)
                definition = field->definition;
                // Indicate that path part was found.
                path_part_found = true;
                // Update the current_tree to this field.
                current_tree = &(*field);
                // Stop the search in this tree's children.
                break;
            }
        }
        // Check if path part was found.
        if(!path_part_found)
        {
            definition = definition_t();
            return false;
        }
    }

    return true;
}
bool definition_tree_t::get_path_definitions(const std::string& path, std::vector<definition_t>& definitions) const
{
    // Get the path components.
    // NOTE: the tokenize function already removes array indicators.
    auto path_parts = definition_tree_t::tokenize_path(path);

    // Iterate through the path parts to build the definitions.
    const definition_tree_t* current_tree = this;
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