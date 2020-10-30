#include "message_impl.h"

#include "message_definition.h"

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
    message_definition m_definition(shape_shifter.getDataType(), shape_shifter.getMessageDefinition());    

    std::cout << m_definition.print_components() << std::endl << m_definition.print_definition() << std::endl;

    auto fields = m_definition.list_fields("poses.pose.position");
    for(auto field = fields.begin(); field != fields.end(); ++field)
    {
        std::cout << field->path << std::endl;
    }
}