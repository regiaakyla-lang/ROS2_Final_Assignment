#include<rclcpp/rclcpp.hpp>
#include<sensor_msgs/msg/imu.hpp>
#include<geometry_msgs/msg/twist.hpp>
#include<tf2/LinearMath/Quaternion.h>
#include<tf2/LinearMath/Matrix3x3.h>

std::shared_ptr<rclcpp::Node> node = nullptr;
rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr vel_pub = nullptr;
void IMUCallback(const sensor_msgs::msg::Imu::SharedPtr msg){
    tf2::Quaternion tf2_quaternion;
    tf2_quaternion.setX(msg->orientation.x);
    tf2_quaternion.setY(msg->orientation.y);
    tf2_quaternion.setZ(msg->orientation.z);
    tf2_quaternion.setW(msg->orientation.w);

    tf2::Matrix3x3 tf2_matrix(tf2_quaternion);
    double roll, pitch, yaw;
    tf2_matrix.getRPY(roll, pitch, yaw);
    roll = roll * 180.0 / M_PI;
    pitch = pitch * 180.0 / M_PI;
    yaw = yaw * 180.0 / M_PI;
    RCLCPP_INFO(node->get_logger(), 
        "Roll: %.2f, Pitch: %.2f, Yaw: %.2f", roll, pitch, yaw);

    double target_yaw = 90;

    geometry_msgs::msg::Twist vel_msg;
    double diff_angle = target_yaw - yaw;
    vel_msg.angular.z = 0.01 * diff_angle;
    vel_msg.linear.x = 0.1;
    vel_pub->publish(vel_msg);
}

int main(int argc, char **argv){
    rclcpp::init(argc,argv);
    node = std::make_shared<rclcpp::Node>("imu_behavior_node");
    auto sub = node->create_subscription<sensor_msgs::msg::Imu>("/imu/data", 10, IMUCallback);
    vel_pub = node->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
