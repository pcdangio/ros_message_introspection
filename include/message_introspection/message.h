#ifndef MESSAGE_INTROSPECTION___MESSAGE_H
#define MESSAGE_INTROSPECTION___MESSAGE_H

#include <topic_tools/shape_shifter.h>

namespace message_introspection
{

class message
{
public:
    message(const topic_tools::ShapeShifter& shape_shifter);
    ~message();

    enum class field_type_t
    {
        BOOL = 0,
        UINT8 = 1,
        UINT16 = 2,
        UINT32 = 3,
        UINT64 = 4,
        INT8 = 5,
        INT16 = 6,
        INT32 = 7,
        INT64 = 8,
        FLOAT32 = 9,
        FLOAT64 = 10,
        STRING = 11,
        TIME = 12,
        DURATION = 13
    };


private:
    struct impl;
    impl* pimpl;
};

}

#endif