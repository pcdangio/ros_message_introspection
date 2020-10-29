#ifndef DEFINITION_H
#define DEFINITION_H

#include <string>
#include <vector>

struct definition_t
{
    std::string type;
    std::string array;
    std::string name;
    uint32_t size;

    std::vector<definition_t> fields;
};

#endif