// 1.包含头文件；
// 2.初始化 ROS2 客户端；
// 3.定义节点类；
//   3-1.创建客户端；
//   3-2.等待服务连接；
//   3-3.组织请求数据并发送；
// 4.创建对象指针调用其功能,并处理响应；
// 5.释放资源。
#include "rclcpp/rclcpp.hpp"
#include "base_interfaces_demo/srv/add_ints.hpp"
using base_interfaces_demo::srv::AddInts;
using namespace std::chrono_literals;
class MinimalClient : public rclcpp::Node
{
public:
    MinimalClient() : Node("Minimal_client")
    {
        client_ = this->create_client<AddInts>("add_ints");
        RCLCPP_INFO(this->get_logger(), "客户端创建，等待连接服务端！");
    }
    bool connect_server()
    {
        while (!client_->wait_for_service(1s))
        {
            if (!rclcpp::ok())
            {
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "强制退出！");
                return false;
            }
            RCLCPP_INFO(this->get_logger(), "服务连接中，请稍候...");
        }
        return true;
    }
    rclcpp::Client<AddInts>::SharedPtr client_;
};

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "please input two intergal");
        return 1;
    }
    rclcpp::init(argc, argv);
    auto client = std::make_shared<MinimalClient>();
    if (!client->connect_server())
    {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "connect fail");
        return 1;
    }
    auto request = std::make_shared<AddInts::Request>();
    request->num1 = atoll(argv[1]);
    request->num2 = atoll(argv[2]);
    auto result = client->client_->async_send_request(request);
    if (rclcpp::spin_until_future_complete(client, result) ==
        rclcpp::FutureReturnCode::SUCCESS)
    {
        RCLCPP_INFO(client->get_logger(), "Sum: %d", result.get()->sum);
    }
    else
    {
        RCLCPP_ERROR(client->get_logger(), "Failed to call service add_three_ints"); // CHANGE
    }

    rclcpp::shutdown();
    return 0;
}