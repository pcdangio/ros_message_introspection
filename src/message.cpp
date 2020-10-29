#include "message_impl.h"

#include "parser.h"

using namespace message_introspection;

message::message(const topic_tools::ShapeShifter& shape_shifter)
{
    // Initialize pimpl.
    message::pimpl = new message::impl;

    // Parse message description.
    message::pimpl->parse_message(shape_shifter);
}

message::~message()
{
    delete message::pimpl;
}

message::impl::impl()
{

}
void message::impl::parse_message(const topic_tools::ShapeShifter& shape_shifter)
{
    parser m_parser;

    auto components = m_parser.parse_message(shape_shifter.getDataType(), shape_shifter.getMessageDefinition());    
}