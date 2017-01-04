// ROS includes
#include <ros/ros.h>
#include <tf/transform_datatypes.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/PoseStamped.h>
//#include <geometry_msgs/Pose2D.h>
#include <visualization_msgs/Marker.h>

// System includes
#include <string>
#include <unistd.h>

visualization_msgs::Marker marker;
ros::Publisher rviz_uav_marker_pub;
ros::Publisher rviz_target_marker_pub;

void publishUAVMarker(const geometry_msgs::PoseStamped::ConstPtr &msg) {
    // Send the current location to RVIZ
    marker.header.frame_id = "/world_ned";
    marker.header.stamp = ros::Time::now();
    marker.ns = "uav";
    marker.id = 0;
    marker.type = visualization_msgs::Marker::ARROW;
    marker.action = visualization_msgs::Marker::ADD;

    // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
    marker.pose.position.x = msg->pose.position.x;
    marker.pose.position.y = msg->pose.position.y;
    marker.pose.position.z = msg->pose.position.z;
//    // translate roll, pitch and yaw into a Quaternion
//    tf::Quaternion q;
//    //        q.setRPY(3.14159, 0.0, 0.0);
//    //        geometry_msgs::Quaternion qt;
//    //        tf::quaternionTFToMsg(q, qt);
//    //        printf("******* x[%0.7f] y[%0.7f] z[%0.7f] w[%0.7f] \n", qt.x, qt.y, qt.z, qt.w);
//    q.setRPY(getAttitude().roll,
//            getAttitude().pitch,
//            getAttitude().yaw);
//    geometry_msgs::Quaternion odom_quat;
//    tf::quaternionTFToMsg(q, odom_quat);
//
//    marker.pose.orientation.x = odom_quat.x;
//    marker.pose.orientation.y = odom_quat.y;
//    marker.pose.orientation.z = odom_quat.z;
//    marker.pose.orientation.w = odom_quat.w;
//
    marker.pose.orientation.x = msg->pose.orientation.x;
    marker.pose.orientation.y = msg->pose.orientation.y;
    marker.pose.orientation.z = msg->pose.orientation.z;
    marker.pose.orientation.w = msg->pose.orientation.w;

    // Set the scale of the marker -- 1x1x1 here means 1m on a side
    marker.scale.x = 1.0;
    marker.scale.y = 0.4;
    marker.scale.z = 0.05;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 0.0f;
    marker.color.g = 1.0f;
    marker.color.b = 0.0f;
    marker.color.a = 0.9;

    marker.lifetime = ros::Duration();
    rviz_uav_marker_pub.publish(marker);
}

void publishTargetMarker(const geometry_msgs::PoseStamped::ConstPtr &msg) {
    // Send the current location to RVIZ
    marker.header.frame_id = "/world_ned";
    marker.header.stamp = ros::Time::now();
    marker.ns = "target";
    marker.id = 0;
    marker.type = visualization_msgs::Marker::CUBE;
    marker.action = visualization_msgs::Marker::ADD;

    // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
    marker.pose.position.x = msg->pose.position.x;
    marker.pose.position.y = msg->pose.position.y;
    marker.pose.position.z = msg->pose.position.z;
//    // translate roll, pitch and yaw into a Quaternion
//    tf::Quaternion q;
//    //        q.setRPY(3.14159, 0.0, 0.0);
//    //        geometry_msgs::Quaternion qt;
//    //        tf::quaternionTFToMsg(q, qt);
//    //        printf("******* x[%0.7f] y[%0.7f] z[%0.7f] w[%0.7f] \n", qt.x, qt.y, qt.z, qt.w);
//    q.setRPY(getAttitude().roll,
//            getAttitude().pitch,
//            getAttitude().yaw);
//    geometry_msgs::Quaternion odom_quat;
//    tf::quaternionTFToMsg(q, odom_quat);
//
//    marker.pose.orientation.x = odom_quat.x;
//    marker.pose.orientation.y = odom_quat.y;
//    marker.pose.orientation.z = odom_quat.z;
//    marker.pose.orientation.w = odom_quat.w;
//
    marker.pose.orientation.x = msg->pose.orientation.x;
    marker.pose.orientation.y = msg->pose.orientation.y;
    marker.pose.orientation.z = msg->pose.orientation.z;
    marker.pose.orientation.w = msg->pose.orientation.w;

    // Set the scale of the marker -- 1x1x1 here means 1m on a side
    marker.scale.x = 1.0;
    marker.scale.y = 0.15;
    marker.scale.z = 1.0;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 1.0f;
    marker.color.g = 0.0f;
    marker.color.b = 0.0f;
    marker.color.a = 0.9;

    marker.lifetime = ros::Duration();
    rviz_target_marker_pub.publish(marker);
}

void dronePoseCB(const geometry_msgs::PoseStamped::ConstPtr &msg) {
//    ROS_INFO("GOT drone pose");
//    publishUAVMarker(msg);
    publishTargetMarker(msg);
    return;
}

void targetPoseCB(const geometry_msgs::PoseStamped::ConstPtr &msg) {
    ROS_INFO("GOT target pose");
    return;
}

void bridgePoseCB(const geometry_msgs::PoseStamped::ConstPtr &msg) {
    ROS_INFO("GOT bridge pose");
    return;
}

int main( int argc, char* argv[] )
{ 
  
  // Initialize ROS node
  ros::init(argc, argv, "mocap_rviz");
  ros::NodeHandle n("~");

  ros::Subscriber drone_pose_sub = n.subscribe("/Drone/pose", 1000, &dronePoseCB);
  ros::Subscriber target_pose_sub = n.subscribe("/Target/pose", 1000, &targetPoseCB);
  ros::Subscriber bridge_pose_sub = n.subscribe("/Bridge/pose", 1000, &bridgePoseCB);

  rviz_uav_marker_pub = n.advertise<visualization_msgs::Marker>("/uav_marker", 1);
  rviz_target_marker_pub = n.advertise<visualization_msgs::Marker>("/target_marker", 1);

  ROS_INFO("MOCAP_RVIZ");

  // Process mocap data until SIGINT
  ROS_INFO("STARTING RVIZ SIMULATION");

  ros::spin();

  return 0;
}
