import rospy 
from std_msgs.msg import Float32
from math import radians

if __name__ =="__main__":
    rospy.init_node('camera_angle_demos')
    pub = rospy.Publisher('/krbai/servo_angle', Float32, queue_size=10)
    angle = Float32()
    angle.data = 0
    flag = True
    rate = rospy.Rate(20)
    while not rospy.is_shutdown():

        if flag == True:
            angle.data += 0.1
            if angle.data >= 1.5707:
                flag = False
        else:
            angle.data -= 0.1
            if angle.data <= -1.5707:
                flag = True
        pub.publish(angle)

        angle.data = radians(90)
        pub.publish(angle)
        rate.sleep()
