
#include<ros/ros.h>
#include<gazebo_msgs/SetModelState.h>
#include<geometry_msgs/PoseStamped.h>
#include<geometry_msgs/Pose.h>



geometry_msgs::PoseStamped pose;
gazebo_msgs::SetModelState srv;

void updateModelState(){
    srv.request.model_state.model_name ="robot";
    srv.request.model_state.pose.position.x = pose.pose.position.x+8;
    srv.request.model_state.pose.position.y = pose.pose.position.y;
    srv.request.model_state.pose.position.z = pose.pose.position.z+3;
    srv.request.model_state.pose.orientation.x = pose.pose.orientation.x;
    srv.request.model_state.pose.orientation.y = pose.pose.orientation.y;
    srv.request.model_state.pose.orientation.z = pose.pose.orientation.z;
    srv.request.model_state.pose.orientation.w = pose.pose.orientation.w;
    
    // ROS_INFO_THROTTLE(1, "Updated model state");
    // ROS_INFO("%f %f %f %f %f %f %f", srv.request.model_state.pose.position.x, srv.request.model_state.pose.position.y, srv.request.model_state.pose.position.z, srv.request.model_state.pose.orientation.x, srv.request.model_state.pose.orientation.y, srv.request.model_state.pose.orientation.z, srv.request.model_state.pose.orientation.w);
}

void poseCallback(const geometry_msgs::PoseStamped::ConstPtr& msg){
    pose.pose.position.x = msg->pose.position.x;
    pose.pose.position.y = msg->pose.position.y;
    pose.pose.position.z = msg->pose.position.z;
    pose.pose.orientation.x = msg->pose.orientation.x;
    pose.pose.orientation.y = msg->pose.orientation.y;
    pose.pose.orientation.z = msg->pose.orientation.z;
    pose.pose.orientation.w = msg->pose.orientation.w;
    ROS_INFO("Updated pose");
    ROS_INFO("%f %f %f %f %f %f %f", pose.pose.position.x, pose.pose.position.y, pose.pose.position.z, pose.pose.orientation.x, pose.pose.orientation.y, pose.pose.orientation.z, pose.pose.orientation.w);
    updateModelState();
}

int main(int argc, char **argv){
    
    ros::init(argc, argv, "update_model_state");
    ros::NodeHandle n;
    ros::Duration(10).sleep();
    ros::topic::waitForMessage<geometry_msgs::PoseStamped>("/mavros/local_position/pose");
    ros::ServiceClient service = n.serviceClient<gazebo_msgs::SetModelState>("/gazebo/set_model_state");
    ros::Subscriber sub = n.subscribe("/mavros/local_position/pose", 1000, poseCallback);
    ROS_INFO("LOOP STARTED");
    ros::Rate loop_rate(60);

    while(ros::ok()){
        // send service request and capture the response
        auto response = service.call(srv);
        // service.call(srv);
        

        ros::spinOnce();
        loop_rate.sleep();
    }
}