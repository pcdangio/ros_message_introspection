/// \file introspector.h
/// \brief Defines the message_introspection::introspector class.
#ifndef MESSAGE_INTROSPECTION___INTROSPECTOR_H
#define MESSAGE_INTROSPECTION___INTROSPECTOR_H

#include "message_introspection/definition.h"
#include "message_introspection/definition_tree.h"

#include <topic_tools/shape_shifter.h>
#include <rosbag/message_instance.h>

#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

/// \brief Code components for message introspection.
namespace message_introspection {

/// \brief Parses and provides the defintion of a message.
class introspector
{
public:
    // CONSTRUCTORS
    /// \brief Creates a new introspector instance.
    introspector();
    ~introspector();

    // CONFIG
    /// \brief Sets the type of message for introspection.
    /// \param type The message's ROS type.
    /// \param definition The message's definition string.
    /// \param md5 The message's MD5 hash.
    void new_message_type(const std::string& type, const std::string& definition, const std::string& md5);

    // NEW MESSAGE
    /// \brief Sets a new topic message instance to read from.
    /// \param message The new message instance.
    void new_message(const topic_tools::ShapeShifter& message);
    /// \brief Sets a new rosbag message instance to read from.
    /// \param message The new message instance.
    void new_message(const rosbag::MessageInstance& message);

    // DEFINITION
    /// \brief Gets a copy of the message's definition tree.
    /// \returns The message's current definition tree.
    definition_tree_t definition_tree() const;

    // GET
    /// \brief Indicates if the path to a field exists.
    /// \param path The path to verify.
    /// \returns TRUE if the path exists, otherwise false.
    bool path_exists(const std::string& path) const;
    /// \brief Gets a bool field from the message.
    /// \param path The path to get the field from.
    /// \param value The reference to store the retrieved value in.
    /// \returns TRUE if the value was retrieved.
    /// Returns FALSE if the path does not exist or the field type doesn't match.
    bool get_bool(const std::string& path, bool& value) const;
    /// \brief Gets an int8 field from the message.
    /// \param path The path to get the field from.
    /// \param value The reference to store the retrieved value in.
    /// \returns TRUE if the value was retrieved.
    /// Returns FALSE if the path does not exist or the field type doesn't match.
    bool get_int8(const std::string& path, int8_t& value) const;
    /// \brief Gets an int16 field from the message.
    /// \param path The path to get the field from.
    /// \param value The reference to store the retrieved value in.
    /// \returns TRUE if the value was retrieved.
    /// Returns FALSE if the path does not exist or the field type doesn't match.
    bool get_int16(const std::string& path, int16_t& value) const;
    /// \brief Gets an int32 field from the message.
    /// \param path The path to get the field from.
    /// \param value The reference to store the retrieved value in.
    /// \returns TRUE if the value was retrieved.
    /// Returns FALSE if the path does not exist or the field type doesn't match.
    bool get_int32(const std::string& path, int32_t& value) const;
    /// \brief Gets an int64 field from the message.
    /// \param path The path to get the field from.
    /// \param value The reference to store the retrieved value in.
    /// \returns TRUE if the value was retrieved.
    /// Returns FALSE if the path does not exist or the field type doesn't match.
    bool get_int64(const std::string& path, int64_t& value) const;
    /// \brief Gets a uint8 field from the message.
    /// \param path The path to get the field from.
    /// \param value The reference to store the retrieved value in.
    /// \returns TRUE if the value was retrieved.
    /// Returns FALSE if the path does not exist or the field type doesn't match.
    bool get_uint8(const std::string& path, uint8_t& value) const;
    /// \brief Gets a uint16 field from the message.
    /// \param path The path to get the field from.
    /// \param value The reference to store the retrieved value in.
    /// \returns TRUE if the value was retrieved.
    /// Returns FALSE if the path does not exist or the field type doesn't match.
    bool get_uint16(const std::string& path, uint16_t& value) const;
    /// \brief Gets a uint32 field from the message.
    /// \param path The path to get the field from.
    /// \param value The reference to store the retrieved value in.
    /// \returns TRUE if the value was retrieved.
    /// Returns FALSE if the path does not exist or the field type doesn't match.
    bool get_uint32(const std::string& path, uint32_t& value) const;
    /// \brief Gets a uint64 field from the message.
    /// \param path The path to get the field from.
    /// \param value The reference to store the retrieved value in.
    /// \returns TRUE if the value was retrieved.
    /// Returns FALSE if the path does not exist or the field type doesn't match.
    bool get_uint64(const std::string& path, uint64_t& value) const;
    /// \brief Gets a float32 field from the message.
    /// \param path The path to get the field from.
    /// \param value The reference to store the retrieved value in.
    /// \returns TRUE if the value was retrieved.
    /// Returns FALSE if the path does not exist or the field type doesn't match.
    bool get_float32(const std::string& path, float& value) const;
    /// \brief Gets a float64 field from the message.
    /// \param path The path to get the field from.
    /// \param value The reference to store the retrieved value in.
    /// \returns TRUE if the value was retrieved.
    /// Returns FALSE if the path does not exist or the field type doesn't match.
    bool get_float64(const std::string& path, double& value) const;
    /// \brief Gets a string field from the message.
    /// \param path The path to get the field from.
    /// \param value The reference to store the retrieved value in.
    /// \returns TRUE if the value was retrieved.
    /// Returns FALSE if the path does not exist or the field type doesn't match.
    bool get_string(const std::string& path, std::string& value) const;
    /// \brief Gets a time field from the message.
    /// \param path The path to get the field from.
    /// \param value The reference to store the retrieved value in.
    /// \returns TRUE if the value was retrieved.
    /// Returns FALSE if the path does not exist or the field type doesn't match.
    bool get_time(const std::string& path, ros::Time& value) const;
    /// \brief Gets a duration field from the message.
    /// \param path The path to get the field from.
    /// \param value The reference to store the retrieved value in.
    /// \returns TRUE if the value was retrieved.
    /// Returns FALSE if the path does not exist or the field type doesn't match.
    bool get_duration(const std::string& path, ros::Duration& value) const;
    /// \brief Gets any primitive field from the message as a number.
    /// \param path The path to get the field from.
    /// \param value The reference to store the retrieved value in.
    /// \returns TRUE if the value was retrieved.
    /// Returns FALSE if the path does not exist or the field type doesn't match.
    /// \details This method will convert any existing primitive field value into a double.
    /// Time/Duration fields return seconds as a double.
    /// String fields are attempted to be parsed into a number (may return NaN if string is not a number).
    bool get_number(const std::string& path, double& value) const;

    // PRINTING
    /// \brief Prints the message's component definitions to a string.
    /// \returns The component definitions.
    std::string print_components() const;
    /// \brief Prints the message's definition tree to a string.
    /// \returns The message's definition tree.
    std::string print_definition_tree() const;

private:
    // MESSAGE
    /// \brief Registers a message.
    /// \param md5 The message's MD5 hash.
    /// \param type The message's data type string.
    /// \param definition The message's definition string.
    /// \details This method will update the component listing and definition tree for the message type.
    void register_message(const std::string& md5, const std::string& type, const std::string& definition);
    /// \brief Indicates if a message MD5 hash is registered or not.
    /// \param md5 The MD5 hash to check.
    /// \returns TRUE if the MD5 hash is registered, otherwise FALSE.
    bool is_registered(const std::string& md5);
    /// \brief Stores the currently registered message MD5 hash.
    std::string m_md5;
    /// \brief Stores the serialized bytes of the most recent message instance.
    uint8_t* m_bytes;

    // COMPONENTS
    /// \brief A map of component message definitions (top level only)
    std::unordered_map<std::string, std::vector<definition_t>> m_component_definitions;
    /// \brief Parses a message definition string into the component definition map.
    /// \param message_type The ROS message type string.
    /// \param introspector The ROS message definition string.
    void parse_components(std::string message_type, std::string introspector);

    // DEFINITION
    /// \brief The message's calculated definition tree.
    definition_tree_t m_definition_tree;    
    /// \brief A recursive method for adding new definitions to the definition tree.
    /// \param parent_path The parent path of the definition tree being added.
    /// \param definition_tree A reference to the definition tree to add to.
    /// \param component_definition The component information to add to the definition's sub tree.
    void add_definition(const std::string& parent_path, definition_tree_t& definition_tree, const definition_t& component_definition);

    // LISTING
    /// \brief A method for getting the definition tree of a specified path.
    /// \param path The path to get the definition tree of.
    /// \returns If the path exists, a pointer to the definition tree, otherwise nullptr.
    const definition_tree_t* get_definition_tree(const std::string& path) const;

    // FIELD MAPPING
    /// \brief Store the position and type of a read field.
    struct field_t
    {
        /// \brief Stores the position of the field in the message's serialized byte array.
        uint32_t position;
        /// \brief Stores the primitive type of the field.
        definition_t::primitive_type_t primitive_type;
    };
    /// \brief Stores a map of fields in a parsed serialized message.
    /// \details Field details are mapped to their fully qualified path string.
    std::unordered_map<std::string, field_t> m_field_map;
    /// \brief Updates the field map by recursively parsing a message's serialized bytes.
    /// \param definition_tree The definition tree to recurse on.
    /// \param current_path The current fully qualified path of recursion.
    /// \param current_position The current position in the message's serialized bytes.
    void update_field_map(const definition_tree_t& definition_tree, const std::string& current_path, uint32_t& current_position);

    // FIELD READING
    /// \brief Reads data out of m_bytes.
    /// \tparam T The data type to read.
    /// \param position The position to read the data from in the m_bytes array.
    /// \returns The read value.
    /// \note Use appropriate exception handling around this method.
    template<typename T>
    T read_value(uint32_t position) const
    {
        // Using endian.h automatically assumes host is little endian. No need for conversion.
        return *reinterpret_cast<T*>(&(introspector::m_bytes[position]));
    }
    /// \brief Reads a field from the message.
    /// \tparam T The data type of the field to read.
    /// \param path The path to read the field from.
    /// \param type The field's primitive type.
    /// \param value The value to store the field data in.
    /// \returns TRUE if the path exists and the field was successfully read, otherwise FALSE.
    /// \note This can only be used on numeric primitive types, and the value type must match the primitive type.
    template<typename T>
    bool get_field(const std::string& path, definition_t::primitive_type_t type, T& value) const
    {
        try
        {
            // Get field info from map.
            auto field_info = introspector::m_field_map.at(path);

            // Check field type.
            if(field_info.primitive_type != type)
            {
                return false;
            }

            // Extract value.
            value = introspector::read_value<T>(field_info.position);
        }
        catch(...)
        {
            return false;
        }

        return true;
    }
};

}

#endif