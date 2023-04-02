#include "rclcpp/rclcpp.hpp"
#include "base_interfaces_demo/srv/add_ints.hpp"
using base_interfaces_demo::srv::AddInts;
using std::placeholders::_1;
using std::placeholders::_2;
class MinimalService : public rclcpp::Node
{
public:
  MinimalService() : Node("minimal_server")
  {
    server_ = this->create_service<AddInts>(
        "add_ints", std::bind(&MinimalService::add, this, _1, _2));
    RCLCPP_INFO(this->get_logger(), "add ints server launched");
  }
  void add(const AddInts::Request::SharedPtr req, const AddInts::Response::SharedPtr res)
  {
    res->sum = req->num1 + req->num2;
    RCLCPP_INFO(this->get_logger(), "请求数据:(%d,%d),响应结果:%d", req->num1, req->num2, res->sum);
  }

private:
  rclcpp::Service<AddInts>::SharedPtr server_;
};
int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalService>());
  rclcpp::shutdown();
  return 0;
}