import rospy
from sim.srv import Activate, ActivateRequest


if __name__ == "__main__":
    rospy.init_node("demo_drop_ball", log_level=rospy.INFO)
    drop_ball_client = rospy.ServiceProxy("/krbai/drop_ball", Activate)
    drop_ball = ActivateRequest()
    drop_ball.data = True
    response = drop_ball_client(drop_ball)
    rospy.loginfo(f"Drop ball response: {response}")
