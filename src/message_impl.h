#ifndef MESSAGE_INTROSPECTION___MESSAGE_IMPL_H
#define MESSAGE_INTROSPECTION___MESSAGE_IMPL_H

#include "message_introspection/message.h"

#include <unordered_map>

struct message_introspection::message::impl
{
    impl();

    void parse_message(const topic_tools::ShapeShifter& shape_shifter);
};

#endif