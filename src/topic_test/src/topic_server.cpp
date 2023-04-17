#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class TopicServer : public rclcpp::Node
{
public:
  TopicServer() : Node("topic_server","ns")
  {
    publisher_ = this->create_publisher<std_msgs::msg::String>("/topic", 10);
  }

private:
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
};
int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TopicServer>());
  rclcpp::shutdown();
  return 0;
}