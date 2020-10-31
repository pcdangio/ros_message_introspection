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

    std::cout << m_definition.print_components() << std::endl << m_definition.print_definition_tree() << std::endl;

    std::vector<definition_t> fields;
    if(m_definition.list_fields(fields, "poses.pose.position"))
    {
        for(auto field = fields.begin(); field != fields.end(); ++field)
        {
            std::cout << "path: " << field->path() << " type: " << field->type() << " array: " << field->array() << " primitive: " << field->is_primitive() << std::endl;
        }
    }

    definition_t field;
    if(m_definition.field_info(field, "poses.pose"))
    {
        std::cout << "path: " << field.path() << " type: " << field.type() << " array: " << field.array() << " primitive: " << field.is_primitive() << std::endl;
    }
}