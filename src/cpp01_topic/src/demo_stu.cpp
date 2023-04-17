#include "rclcpp/rclcpp.hpp"
#include "base_interfaces_demo/msg/student.hpp"

using namespace std::chrono_literals;
using base_interfaces_demo::msg::Student;

class MinimalPublisher : public rclcpp::Node
{
public:
  MinimalPublisher() : Node("student_publisher", "ns1"), count_(0)
  {
    publisher_ = this->create_publisher<Student>("topic_stu", 10);
    timer_ = this->create_wall_timer(1s, std::bind(&MinimalPublisher::timer_callback, this));
  }

private:
  void timer_callback()
  {
    auto stu = Student();
    stu.name = "张三";
    stu.age = count_++;
    stu.height = 1.65;
    RCLCPP_INFO(this->get_logger(),
                "学生信息:name=%s,age=%d,height=%.2f", stu.name.c_str(), stu.age, stu.height);
    publisher_->publish(stu);
  }
  size_t count_;
  rclcpp::Publisher<Student>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};
int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  rclcpp::shutdown();
  return 0;
}