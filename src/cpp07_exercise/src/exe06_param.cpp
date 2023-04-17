#include "rclcpp/rclcpp.hpp"
using namespace std::chrono_literals;
class ExeParamClient : public rclcpp::Node
{
public:
    ExeParamClient() : Node("exe_param_client"), red(0), green(0)
    {
        param_client = std::make_shared<rclcpp::SyncParametersClient>(this, "/turtlesim");
    }
    bool connect_server()
    {
        while (!param_client->wait_for_service(5s))
        {
            if (!rclcpp::ok())
            {
                RCLCPP_INFO(this->get_logger(), "终端退出!");
                return false;
            }
            RCLCPP_INFO(this->get_logger(), "参数服务连接中，请稍等......");
        }
        return true;
    }
    void update_param()
    {
        red = param_client->get_parameter<int32_t>("background_r");
        green = param_client->get_parameter<int32_t>("background_g");

        int i = 0;
        while (rclcpp::ok())
        {
            i < 255 ? red += 5 : red -= 5;
            i += 5;
            green = 255 - red;
            RCLCPP_INFO(this->get_logger(),"red = %d", red);
            if (i >= 510)
                i = 0;
            param_client->set_parameters({rclcpp::Parameter("background_r", red),
                                          rclcpp::Parameter("background_g", green)});
        }
    }

private:
    rclcpp::SyncParametersClient::SharedPtr param_client;
    int32_t red;
    int32_t green;
};
int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ExeParamClient>();
    if (!node->connect_server())
        return 1;
    node->update_param();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}