#include "rclcpp/rclcpp.hpp"
using namespace std::chrono_literals;

class MinimalParamClient : public rclcpp::Node
{
public:
    MinimalParamClient() : Node("minimal_parameter_client")
    {
        param_client_ = std::make_shared<rclcpp::SyncParametersClient>(this, "minimal_parameter_server");
    }
    bool connect_server()
    {
        while (!param_client_->wait_for_service(1s))
        {
            if (!rclcpp::ok())
                return false;
            RCLCPP_INFO(this->get_logger(), "服务未连接");
        }
        return true;
    }
    void get_param()
    {
        RCLCPP_INFO(this->get_logger(), "-----------参数客户端查询参数-----------");
        double weight = param_client_->get_parameter<double>("car_weight");
        RCLCPP_INFO(this->get_logger(), "car wieght is %f",weight);
        std::vector<rclcpp::Parameter>params = param_client_->get_parameters({"car_speed","car_color","car_weight"});
        for(auto &param : params)
            RCLCPP_INFO(this->get_logger(),"%s = %s",param.get_name().c_str(),param.value_to_string().c_str());
    }
    void update_param()
    {
        RCLCPP_INFO(this->get_logger(),"-----------参数客户端修改参数-----------");
        param_client_->set_parameters({rclcpp::Parameter("car_speed",99),rclcpp::Parameter("car_color","blue"),rclcpp::Parameter("car_wheel",2)});
        std::vector<rclcpp::Parameter>params = param_client_->get_parameters({"car_speed","car_color","car_wheel"});
        for(auto &param : params)
            RCLCPP_INFO(this->get_logger(),"%s = %s",param.get_name().c_str(),param.value_to_string().c_str());
    }

private:
    rclcpp::SyncParametersClient::SharedPtr param_client_;
};
int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto paramClinet = std::make_shared<MinimalParamClient>();
    if (!paramClinet->connect_server())
    {
        return 1;
    }
    paramClinet->get_param();
    paramClinet->update_param();
    rclcpp::shutdown();
    return 0;
}