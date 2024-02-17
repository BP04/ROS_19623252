#include "ros/ros.h"
#include "tugas1/DronePos.h"
#include "tugas1/LandDrone.h"

ros::Publisher pub;
ros::ServiceServer srv;

bool landDroneCallback(tugas1::LandDrone::Request &req, tugas1::LandDrone::Response &res) {
    ROS_INFO("Received request to land the drone");

    res.response = "success"; 
    return true;
}

void publishData() {
    ros::Rate loop_rate(0.1);

    float x = 495.0;
    float y = -395.0;
    float z = 0.0;
    std::string status;

    for (int i = 0; i < 1000; ++i) {
        tugas1::DronePos msg;
        msg.x = x;
        msg.y = y;
        msg.z = z;

        if (z >= 1 && z <= 9) {
            msg.status = "takeoff";
        }
        else if (z == 10) {
            msg.status = "fly";
        }
        else if (z == 0) {
            msg.status = "land";
        }

        x += 1.0;
        y -= 1.0;
        z += (z < 10 ? 1.0 : 0.0);

        ROS_INFO("Publishing DronePos: x=%.2f, y=%.2f, z=%.2f, status=%s", msg.x, msg.y, msg.z, msg.status.c_str());
        pub.publish(msg);

        ros::spinOnce();
        loop_rate.sleep();
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "navigator");
    ros::NodeHandle nh;

    pub = nh.advertise<tugas1::DronePos>("/drone_status", 10);
    srv = nh.advertiseService("land_drone", landDroneCallback);

    publishData();

    return 0;
}
