#include <ros/ros.h>
#include <gazebo_msgs/SetModelState.h>
#include <gazebo_msgs/GetModelState.h>
#include <gazebo_ros_link_attacher/Attach.h>
#include <sim/Activate.h>
#include <boost/shared_ptr.hpp>

ros::NodeHandle *n_ptr;
bool drop_handle(sim::Activate::Request &req, sim::Activate::Response &res)
{
    ros::NodeHandle n = *n_ptr;
    ros::ServiceClient client_detach = n.serviceClient<gazebo_ros_link_attacher::Attach>("/link_attacher_node/detach");
    if(req.data){
        gazebo_ros_link_attacher::Attach detach_srv;
        detach_srv.request.model_name_1 = "robot";
        detach_srv.request.link_name_1 = "robot::link";
        detach_srv.request.model_name_2 = "ball";
        detach_srv.request.link_name_2 = "link";
        client_detach.call(detach_srv);
    }
    return true;
}

int main(int argc, char **argv){
    ros::init(argc, argv, "attach_ball");
    ros::NodeHandle n;
    n_ptr = &n;
    ROS_INFO("Waiting for gazebo to start");
    
    ros::Duration(15).sleep();
    ros::ServiceClient client = n.serviceClient<gazebo_ros_link_attacher::Attach>("/link_attacher_node/attach");
    ros::ServiceClient service = n.serviceClient<gazebo_msgs::SetModelState>("/gazebo/set_model_state");
    ros::ServiceClient get_state = n.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state");
    gazebo_msgs::GetModelState srv;
    srv.request.model_name = "robot";
    get_state.call(srv);
    ros::spinOnce();
    
    ROS_INFO("Model state: %f %f %f %f %f %f %f", srv.response.pose.position.x, srv.response.pose.position.y, srv.response.pose.position.z, srv.response.pose.orientation.x, srv.response.pose.orientation.y, srv.response.pose.orientation.z, srv.response.pose.orientation.w);
    
    gazebo_msgs::SetModelState set_srv;
    set_srv.request.model_state.model_name = "robot";
    set_srv.request.model_state.pose.position.x = srv.response.pose.position.x;
    set_srv.request.model_state.pose.position.y = srv.response.pose.position.y;
    set_srv.request.model_state.pose.position.z = srv.response.pose.position.z+0.4;
    service.call(set_srv);

    set_srv.request.model_state.model_name = "ball";
    set_srv.request.model_state.pose.position.x = srv.response.pose.position.x;
    set_srv.request.model_state.pose.position.y = srv.response.pose.position.y;
    set_srv.request.model_state.pose.position.z = srv.response.pose.position.z-0.2;
    service.call(set_srv);
    ros::spinOnce();

    gazebo_ros_link_attacher::Attach att_srv;
    att_srv.request.model_name_1 = "robot";
    att_srv.request.link_name_1 = "robot::link";
    att_srv.request.model_name_2 = "ball";
    att_srv.request.link_name_2 = "link";
    client.call(att_srv);
    ROS_INFO("Attached ball to robot");
    ros::spinOnce();
    ros::ServiceServer detach = n.advertiseService("/krbai/drop_ball", drop_handle);
    ros::spin();
}