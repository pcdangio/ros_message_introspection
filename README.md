# message_introspection

An introspection library for processing ROS messages of unknown type in C++ without compile-time requirements.

**Contents:**
- [Overview](#overview) - A description of this library.
- [Usage Examples](#usage-examples) - Examples of how to use this library.
- [Important Considerations](#important-considerations) - A list of important things to consider when using this library.

# Overview

The **message_introspection** library enables C++ code to process arbitrary ROS messages without having to include their definitions at compile time. The library is able to process unknown messages and provide detailed information about their definition as well as extract values from the message itself. These messages can come from topic subscribers or be loaded from ROS bag files.

# Usage Examples

The following snippets provide various examples of how to use the library.  Full documentation of the **message_introspection** library can be generated using doxygen and the doxyfile supplied in the library's package.

- [Example 1: Basic Topic Subscriber](#example-1-basic-topic-subscriber)
- [Example 2: Basic ROS Bag Reader](#example-2-basic-ros-bag-reader)
- [Other Examples](#other-examples)

## Example 1: Basic Topic Subscriber
In this example, a generic topic subscriber is set up using the ROS [topic_tools/ShapeShifter][1] message. The generic subscriber is listening to the `\some_topic` topic, in which another node is publishing [sensor_msgs/Imu][2] messages to. The generic subscriber does not know that the messages received on `\some_topic` are [sensor_msgs/Imu][2] messages, nor does it know how to process them or retrieve data from them. The message_introspection is used here to determine the unknown message's definition, and then read data from it.

```cpp
#include <message_introspection/introspector.h>
#include <iostream>

// Create a global instance of the introspector for EACH topic for maximum efficiency.
message_introspection::introspector introspector;

// Set up generic subscriber callback using ShapeShifter message.
void subscriber_shape_shifter(const topic_tools::ShapeShifter& message)
{
    // Each time a new message is received, pass it to this topic's introspector for parsing.
    introspector.new_message(message);
    // The message is now parsed.

    // OPTIONAL: print the message's parsed definition tree to console.
    std::cout << introspector.print_definition_tree() << std::endl;

    // OPTIONAL: read data from the parsed message.
    std::string path_to_read = "linear_acceleration.x";
    double linear_acceleration_x;
    if(introspector.get_float64(path_to_read, linear_acceleration_x))
    {
        std::cout << "X linear acceleration: " << linear_acceleration_x << std::endl;
    }
    else
    {
        std::cout << "path_to_read does not exist, or used wrong data type to get field" << std::endl;
    }
}

// Set up main function.
int32_t main(int32_t argc, char** argv)
{
    // Initialize ROS and node handle.
    ros::init(argc, argv, "message_introspection_tester");
    ros::NodeHandle node;

    // Set up subscriber.
    ros::Subscriber subscriber_shape_shifter = node.subscribe("/some_topic", 1, subscriber_shape_shifter);

    // Spin to receive messages.
    ros::spin();

    return 0;
}
```

When this code is run, the following console output is generated each time a new [/sensor_msgs/Imu][2] message is published to `/some_topic` by another node:

```
name =  type = sensor_msgs/Imu size = 116 array = 
	name = header type = std_msgs/Header size = 12 array = 
		name = seq type = uint32 size = 4 array = 
		name = stamp type = time size = 8 array = 
		name = frame_id type = string size = 0 array = 
	name = orientation type = geometry_msgs/Quaternion size = 32 array = 
		name = x type = float64 size = 8 array = 
		name = y type = float64 size = 8 array = 
		name = z type = float64 size = 8 array = 
		name = w type = float64 size = 8 array = 
	name = orientation_covariance type = float64 size = 8 array = [9]
	name = angular_velocity type = geometry_msgs/Vector3 size = 24 array = 
		name = x type = float64 size = 8 array = 
		name = y type = float64 size = 8 array = 
		name = z type = float64 size = 8 array = 
	name = angular_velocity_covariance type = float64 size = 8 array = [9]
	name = linear_acceleration type = geometry_msgs/Vector3 size = 24 array = 
		name = x type = float64 size = 8 array = 
		name = y type = float64 size = 8 array = 
		name = z type = float64 size = 8 array = 
	name = linear_acceleration_covariance type = float64 size = 8 array = [9]

X linear acceleration: 1.23
```

## Example 2: Basic ROS Bag Reader
In this example, generic messages with unknown type are read from a ROS bag file. The program reads the `\some_topic` topic from the bag file, which (unbeknownst to this program) contains [sensor_msgs/Imu][2] messages. The code itself does not know that the messages loaded from `\some_topic` are [sensor_msgs/Imu][2] messages, nor does it know how to process them or retrieve data from them. The message_introspection is used here to determine the unknown message's definition, and then read data from it.

```cpp
#include <message_introspection/introspector.h>
#include <iostream>

// Set up main function.
int32_t main(int32_t argc, char** argv)
{
    // Initialize ROS and node handle.
    ros::init(argc, argv, "message_introspection_tester");
    ros::NodeHandle node;

    // Use rosbag to open a bag file.
    rosbag::Bag bag;
    bag.open("some_bag.bag", rosbag::bagmode::Read);

    // Use topic query to get messages from the bag.
    std::vector<std::string> topics;
    topics.push_back("/some_topic");
    robag::View view(bag, rosbag::TopicQuery(topics));

    // Create introspector for parsing/reading this topic's messages.
    message_introspection::introspector introspector;

    // Iterate through message instances loaded from the bag/topic.
    foreach(rosbag::MessageInstance const message, view)
    {
        // Each time a new message is received, pass it to this topic's introspector for parsing.
        introspector.new_message(message);
        // The message is now parsed.

        // OPTIONAL: print the message's parsed definition tree to console.
        std::cout << introspector.print_definition_tree() << std::endl;

        // OPTIONAL: read data from the parsed message.
        std::string path_to_read = "linear_acceleration.x";
        double linear_acceleration_x;
        if(introspector.get_float64(path_to_read, linear_acceleration_x))
        {
            std::cout << "X linear acceleration: " << linear_acceleration_x << std::endl;
        }
        else
        {
            std::cout << "path_to_read does not exist, or used wrong data type to get field" << std::endl;
        }
    }

    // Close the bag.
    bag.close();

    return 0;
}
```

When this code is run, the following console output is generated for each [/sensor_msgs/Imu][2] message read from the bag file at topic `\some_topic`:

```
name =  type = sensor_msgs/Imu size = 116 array = 
	name = header type = std_msgs/Header size = 12 array = 
		name = seq type = uint32 size = 4 array = 
		name = stamp type = time size = 8 array = 
		name = frame_id type = string size = 0 array = 
	name = orientation type = geometry_msgs/Quaternion size = 32 array = 
		name = x type = float64 size = 8 array = 
		name = y type = float64 size = 8 array = 
		name = z type = float64 size = 8 array = 
		name = w type = float64 size = 8 array = 
	name = orientation_covariance type = float64 size = 8 array = [9]
	name = angular_velocity type = geometry_msgs/Vector3 size = 24 array = 
		name = x type = float64 size = 8 array = 
		name = y type = float64 size = 8 array = 
		name = z type = float64 size = 8 array = 
	name = angular_velocity_covariance type = float64 size = 8 array = [9]
	name = linear_acceleration type = geometry_msgs/Vector3 size = 24 array = 
		name = x type = float64 size = 8 array = 
		name = y type = float64 size = 8 array = 
		name = z type = float64 size = 8 array = 
	name = linear_acceleration_covariance type = float64 size = 8 array = [9]

X linear acceleration: 1.23
```

## Other Examples:

The following snippet demonstrates some important features of the library:

```cpp
// Parse a new message, similar to the above examples.
introspector.new_message(message);
// Now that the message is parsed, the following commands can be used:



// This method grabs a copy of the message's definition tree, which is a traversable tree of definition information for each field in the message.
// The definitions for each field include:
//   - The field's name and data type.
//   - If the field is an array type.
//   - The full path string of the field.
// All of this information can be programmatically accessed to find details about the message's structure and it's fields.
message_introspection::definition_tree_t definition_tree = introspector.definition_tree();
// To read the definition tree, iterate through it.
// This example reads the top-level fields of the message (but not their subfields).
std::vector<std::string> field_paths;
for(auto field = definition_tree.fields().begin(); field != definition_tree.fields().end(); ++field)
{
    field_paths.push_back(field->path());
    std::cout << "type: " << field->type() << "\tpath: " << field->path() << std::endl;
}
// Note that the definition tree is recursive, so you need a recursive function to fully traverse the tree.



// Paths used in introspection are strings that point to the location of a field in the definition tree.
// For example, to access the 'x' field of the 'linear_acceleration' field of a sensor_msgs/Imu message:
std::string imu_acceleration_x_path = "linear_acceleration.x";
// To specify a path for an array element:
std::string imu_acceleration_covariance_7 = "linear_acceleration_covariance[7]";
// Sometimes you need to get a subfield contained within a parent field that is an array.
// Take for example nav_msgs/Path message:
std::string path_pose_3_position_y = "poses[3].pose.position.y";



// Paths can be checked for existence if desired.
bool path_exists = introspector.path_exists("linear_acceleration.x");
// Note that any get_*() methods that take a path as an input will return false if the path does not exist.
// So it is NOT necessary to check if a path exists before each get_*() call.



// You can use get_*() methods to read data from the message itself.
// A get_*() method is provided for each ROS primitive type. Only primitive type fields (e.g. int32, string, time, etc) can be read.
// You must use the correct get_*() method that matches the type of the field you are trying to read.
// Use the definition_tree to determine what the field's path and type is.
// For example, the definition_tree specifies that "linear_acceleration.x" has a type: "float_64"
double linear_acceleration_x;
bool success = introspector.get_float64("linear_acceleration.x", linear_acceleration_x);
// This method will return FALSE if the path does not exist, or if the requested field type does not match.
```

# Important Considerations
For maximum speed/memory efficiency, a separate and persistent instance of the `message_introspection::introspector` should be created for **each type** of message you expect to handle.

The `message_introspection::introspector` is only capable of storing a single message type definition. Each time a new message type is presented to the `message_introspection::introspector`, it internally registers the message by parsing the message's structure. This parsing activity only needs to be done once, as new instances of the same message will all have the same structure/definition. Thus, two important considerations need to be made:
1. If you do NOT use a persistent `message_introspection::introspector` instance, and instead create a new instance each time a new message is received, then the registration routine is internally called each time. This is a waste of computation resources if the message structure itself is not changing.
2. If you use the same `message_introspection::introspector` instance to process messages with different types, then it will re-run the internal registration routine each time the message type changes. While this can be used to save memory, it requires extra computation resources to continuously run the registration routine.

A final important consideration is that the `message_introspection::introspector` instance consumes memory in proportion to array sizes. Because each array element/index represents a different path (see [Other Examples](#other-examples) above), messages with very large arrays will consume more memory. Internally, the `message_introspection::introspector` instance keeps a hash map of every possible path in the message.

[1]: http://docs.ros.org/en/melodic/api/topic_tools/html/classtopic__tools_1_1ShapeShifter.html
[2]: http://docs.ros.org/en/api/sensor_msgs/html/msg/Imu.html