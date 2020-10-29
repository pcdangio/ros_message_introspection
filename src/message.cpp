#include "message_impl.h"

#include <boost/tokenizer.hpp>

#include <unordered_map>

using namespace message_introspection;

message::message(const topic_tools::ShapeShifter& shape_shifter)
{
    // Initialize pimpl.
    message::pimpl = new message::impl;

    // Parse message description.
    message::pimpl->parse_message_definition(shape_shifter);
}

message::~message()
{
    delete message::pimpl;
}

message::impl::impl()
{

}
void message::impl::parse_message_definition(const topic_tools::ShapeShifter& shape_shifter)
{
    // Create map of primitives to compare tokens against.
    std::unordered_map<std::string, message::field_type_t> primitives;
    primitives["bool"] = message::field_type_t::BOOL;
    primitives["uint8"] = message::field_type_t::UINT8;
    primitives["uint16"] = message::field_type_t::UINT16;
    primitives["uint32"] = message::field_type_t::UINT32;
    primitives["uint64"] = message::field_type_t::UINT64;
    primitives["int8"] = message::field_type_t::INT8;
    primitives["int16"] = message::field_type_t::INT16;
    primitives["int32"] = message::field_type_t::INT32;
    primitives["int64"] = message::field_type_t::INT64;
    primitives["float32"] = message::field_type_t::FLOAT32;
    primitives["float64"] = message::field_type_t::FLOAT64;
    primitives["string"] = message::field_type_t::STRING;
    primitives["time"] = message::field_type_t::TIME;
    primitives["duration"] = message::field_type_t::DURATION;

    // Create map for capturing message structure.
    struct field_t
    {
        std::string type;
        std::string name;
    };
    std::unordered_map<std::string, std::vector<field_t>> message_definitions;
    // Add top-level message to definition and set it as the current workspace.
    auto* fields_workspace = &message_definitions[shape_shifter.getDataType()];

    // Convert description into a stringstream.
    std::stringstream description_stream(shape_shifter.getMessageDefinition());

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
            fields_workspace = &message_definitions[tokens[1]];
        }
        else
        {
            // Add to fields workspace.
            fields_workspace->push_back({tokens[0], tokens[1]});
        }
    }

    // Create the 

    std::cout << "message definition:" << std::endl;
    for(auto msg_def = message_definitions.begin(); msg_def != message_definitions.end(); ++msg_def)
    {
        auto& field_defs = msg_def->second;
        for(auto field_def = field_defs.begin(); field_def != field_defs.end(); ++field_def)
        {
            std::cout << msg_def->first << "." << field_def->name << " (" << field_def->type << ")" << std::endl;
        }
    }
}