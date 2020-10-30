#ifndef MESSAGE_INTROSPECTION___FIELD_INFO_H
#define MESSAGE_INTROSPECTION___FIELD_INFO_H

#include <string>

namespace message_introspection {

struct field_info_t
{
    std::string name;
    std::string type;
    std::string array;
    bool is_primitive;
    std::string path;
};

}

#endif