#include <ros/ros.h>
#include <gazebo_msgs/SetModelConfiguration.h>
#include <std_msgs/Float32.h>
#include <boost/algorithm/string.hpp>


std_msgs::Float32 angle;


void camera_angle_callback(const std_msgs::Float32::ConstPtr& msg);

int main(int argc, char **argv){
    ros::init(argc, argv, "update_camera");
    ROS_INFO("ROS INITIALIZED");
    ros::NodeHandle n;

    ROS_INFO("Waiting 5s");
    ros::Duration(5).sleep();
    ROS_INFO("servo angle Subscriber created");
    ros::Subscriber camera_sub = n.subscribe("/krbai/servo_angle", 1000, camera_angle_callback);
    ROS_INFO("Creating SetModelConfiguration service client");
    ros::ServiceClient set_model_state = n.serviceClient<gazebo_msgs::SetModelConfiguration>("/gazebo/set_model_configuration");

    ROS_INFO("model");
    gazebo_msgs::SetModelConfiguration model;
    ROS_INFO("model created");

    model.request.model_name = "robot";
    ROS_INFO("model name set");
    std::string joint_name = "robot::camera_joint";

    
    model.request.joint_names.push_back(joint_name);
    ROS_INFO("Joint name set");

    model.request.joint_positions.push_back(angle.data);
    ROS_INFO("angle data set");
    set_model_state.call(model);
    ROS_INFO("SetModelConfiguration service called");
    ROS_INFO("=camera update= LOOP STARTED");
    ros::Rate loop_rate(20);

    while(ros::ok()){
        if(angle.data > 1.5707){
            angle.data = 1.5707;
        }
        else if(angle.data < -1.5707){
            angle.data = -1.5707;
        }
        model.request.joint_positions.pop_back();        
        model.request.joint_positions.push_back(angle.data);
        set_model_state.call(model);

        ROS_INFO_THROTTLE(1, "update_camera loop");
        
        ros::spinOnce();
        loop_rate.sleep();  
    }

}


void camera_angle_callback(const std_msgs::Float32::ConstPtr& msg){
    ROS_INFO_THROTTLE(0.3, "Camera angle: %f", msg->data);
    angle.data = msg->data;
 
}