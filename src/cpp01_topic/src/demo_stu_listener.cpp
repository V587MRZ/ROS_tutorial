#include "rclcpp/rclcpp.hpp"
#include "base_interfaces_demo/msg/student.hpp"
using base_interfaces_demo::msg::Student;
using std::placeholders::_1;
class MinimalSubsriber : public rclcpp::Node
{
public:
  MinimalSubsriber() : Node("student_subsriber")
  {
    subscription_ = this->create_subscription<Student>(
        "topic_stu", 10, std::bind(&MinimalSubsriber::topic_callback, this, _1));
  }

private:
  void topic_callback(const Student &msg) const
  {
    RCLCPP_INFO(
        this->get_logger(), "订阅的学生消息name=%s,age=%d,height=%.2f", msg.name.c_str(), msg.age, msg.height);
  }
  rclcpp::Subscription<Student>::SharedPtr subscription_;
};
int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalSubsriber>());
  rclcpp::shutdown();
  return 0;
}