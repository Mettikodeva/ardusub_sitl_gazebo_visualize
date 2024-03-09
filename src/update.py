import rospy
from gazebo_msgs.srv import SetModelState, SetModelStateRequest
if __name__ == "__main__":
    rospy.init_node("update_model_state")
    pindah_service = rospy.ServiceProxy("/gazebo/set_model_state", SetModelState)
    req = SetModelStateRequest()
    req.model_state.model_name = "unit_box"
    req.model_state.pose.position.x = 1
    req.model_state.pose.position.y = 0
    req.model_state.pose.position.z = 0
    res = pindah_service(req)
    rospy.loginfo(req)
    rospy.loginfo(res)
