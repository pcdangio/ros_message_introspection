#ifndef MESSAGE_INTROSPECTION___DEFINITION_TREE_H
#define MESSAGE_INTROSPECTION___DEFINITION_TREE_H

#include <message_introspection/definition.h>

#include <vector>

namespace message_introspection {

/// \brief A message definition tree.
struct definition_tree_t
{
    /// \brief The message's definition.
    definition_t definition;

    /// \brief The fields belonging to this message.
    std::vector<definition_tree_t> fields;
};

}

#endif