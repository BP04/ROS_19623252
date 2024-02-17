#include "ros/ros.h"
#include "tugas1/LandDrone.h"
#include "tugas1/DronePos.h"

ros::ServiceClient client;

void receiveData(const tugas1::DronePos::ConstPtr& msg) {
    ROS_INFO("Received DronePos: x=%.2f, y=%.2f, z=%.2f, status=%s", msg->x, msg->y, msg->z, msg->status.c_str());

    if (msg->x > 500 && msg->y < -400) {
        tugas1::LandDrone srv;
        srv.request.command = "land";

        if (client.call(srv)) {
            ROS_INFO("Service response: %s", srv.response.response.c_str());
        } else {
            ROS_ERROR("Failed to call service land_drone");
        }
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "commander");
    ros::NodeHandle nh;

    client = nh.serviceClient<tugas1::LandDrone>("land_drone");
    ros::Subscriber sub = nh.subscribe("/drone_status", 10, receiveData);

    ros::spin();

    return 0;
}