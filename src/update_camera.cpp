#include <ros/ros.h>
#include <gazebo_msgs/SetModelConfiguration.h>
#include <std_msgs/Float32.h>


std_msgs::Float32 angle;
ros::NodeHandle *n_ptr;

void camera_angle_callback(const std_msgs::Float32::ConstPtr& msg){
    angle = *msg;
}

void update_camera_angle(){
    ros::NodeHandle n = *n_ptr;
    ros::ServiceClient set_model_state = n.serviceClient<gazebo_msgs::SetModelConfiguration>("/gazebo/set_model_configuration");
    gazebo_msgs::SetModelConfiguration model;
    
    
    if(angle.data){
        if(angle.data > 1.5707){
            angle.data = 1.5707;
        }
        else if(angle.data < -1.5707){
            angle.data = -1.5707;
        }

        model.request.model_name = "robot";
        model.request.joint_names[0] = "robot::camera_joint";
        model.request.joint_positions[0] = angle.data;
        set_model_state.call(model);
    }
}

int main(int argc, char **argv){
    ros::init(argc, argv, "update_camera");
    ros::NodeHandle n;
    n_ptr = &n;
    ros::topic::waitForMessage<std_msgs::Float32>("/krbai/servo_angle");
    ros::Duration(30).sleep();
    ros::Subscriber camera_sub = n.subscribe("/krbai/servo_angle", 10, camera_angle_callback);

    update_camera_angle();

    ros::spin();
}