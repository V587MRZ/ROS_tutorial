#include "rclcpp/rclcpp.hpp"
using namespace std::chrono_literals;
class DemoTime : public rclcpp::Node
{
public:
  DemoTime() : Node("demo_time")
  {
    // demo_rate();
    demo_duration();
  }

private:
  void demo_duration()
  {
    rclcpp::Duration d1(1,500000000L);

    rclcpp::Time right_now(this->now());
    RCLCPP_INFO(this->get_logger(), "s = %.2f, ns = %ld", d1.seconds(), d1.nanoseconds());
    RCLCPP_INFO(this->get_logger(), "s = %.2f, ns = %ld", right_now.seconds(), right_now.nanoseconds());
  }
  void demo_rate()
  {
    rclcpp::Rate rate(500ms);
    while (rclcpp::ok())
    {
      RCLCPP_INFO(this->get_logger(), "Time now: %f", this->now().seconds());
      rate.sleep();
    }
  }
};
int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<DemoTime>());
  rclcpp::shutdown();
  return 0;
}