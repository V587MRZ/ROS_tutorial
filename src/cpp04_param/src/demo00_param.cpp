#include "rclcpp/rclcpp.hpp"

class ParamIntro : public rclcpp::Node
{
public:
  ParamIntro() : Node("param_intro")
  {
    RCLCPP_INFO(this->get_logger(), "parameter API introduction");
    rclcpp::Parameter p1("car_name", "Tiger");
    rclcpp::Parameter p2("width", 0.15);
    rclcpp::Parameter p3("wheels", 2);
    RCLCPP_INFO(this->get_logger(), "%s = %s", p1.get_name().c_str(), p1.as_string().c_str());
    RCLCPP_INFO(this->get_logger(), "p2 is %.2f, type name is %s", p2.as_double(), p2.get_type_name().c_str());
    RCLCPP_INFO(this->get_logger(), "p3 is %s", p3.value_to_string().c_str());
  }
};
int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  std::make_shared<ParamIntro>();
  rclcpp::shutdown();
  return 0;
}