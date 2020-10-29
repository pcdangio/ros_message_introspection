#ifndef MESSAGE_INTROSPECTION___MESSAGE_IMPL_H
#define MESSAGE_INTROSPECTION___MESSAGE_IMPL_H

#include "message_introspection/message.h"

struct message_introspection::message::impl
{
    impl();

    void parse_message_definition(const topic_tools::ShapeShifter& shape_shifter);
};

#endif