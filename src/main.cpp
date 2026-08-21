#include <ros/ros.h>

#include <exception>

#include "ioniq5_ecan/node.hpp"

int main(int argc, char** argv) {
  ros::init(argc, argv, "ioniq5_ecan_node");
  try {
    ros::NodeHandle node_handle;
    ros::NodeHandle private_node_handle("~");
    ioniq5_ecan::Ioniq5EcanNode node(node_handle, private_node_handle);
    ros::spin();
  } catch (const std::exception& error) {
    ROS_FATAL("fatal error: %s", error.what());
    return 1;
  }
  return 0;
}
