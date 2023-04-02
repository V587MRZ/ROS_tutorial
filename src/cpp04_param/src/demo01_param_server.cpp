#include "rclcpp/rclcpp.hpp"

class MinimalParamServer : public rclcpp::Node
{
public:
    MinimalParamServer() : Node("minimal_parameter_server",
                                rclcpp::NodeOptions().allow_undeclared_parameters(true))
    {
    }
    void declear_param()
    {
        this->declare_parameter("car_speed", 100);
        this->declare_parameter("car_color", "red");
        this->declare_parameter("car_weight", 1000.0);
        this->set_parameter(rclcpp::Parameter("anther_car_name", "BMW"));
    }
    void get_param()
    {
        RCLCPP_INFO(this->get_logger(), "------------------查----------------");
        rclcpp::Parameter car_speed = this->get_parameter("car_speed");
        RCLCPP_INFO(this->get_logger(), "car_speed: %ld", car_speed.as_int());
        RCLCPP_INFO(this->get_logger(), "car_color: %s", this->get_parameter("car_color").as_string().c_str());
        //   std::vector<rclcpp::Parameter>
        //   get_parameters(const std::vector<std::string> & names) const;
        std::vector<rclcpp::Parameter> params = this->get_parameters(
            {"car_speed", "car_color", "car_weight", "anther_car_name"});
        for (auto &param : params)
        {
            RCLCPP_INFO(this->get_logger(), "%s: %s", param.get_name().c_str(), param.value_to_string().c_str());
        }
    }
    void update_param()
    {
        RCLCPP_INFO(this->get_logger(), "------------------改----------------");
        this->set_parameter(rclcpp::Parameter("car_speend", 99));
        RCLCPP_INFO(this->get_logger(), "car_speed: %ld", get_parameter("car_speed").as_int());
    }
    void delete_param()
    {
        RCLCPP_INFO(this->get_logger(), "------------------删----------------");
        RCLCPP_INFO(this->get_logger(),"删除操作前car_weight 存在? %d", this->has_parameter("car_weight"));
        // this->undeclare_parameter("anther_car_name");
        // RCLCPP_INFO(this->get_logger(),"after delete car_weight 存在? %d", this->has_parameter("anther_car_name"));
    }
};
int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto paramServer = std::make_shared<MinimalParamServer>();
    paramServer->declear_param();
    paramServer->get_param();
    paramServer->update_param();
    paramServer->delete_param();
    rclcpp::spin(paramServer);
    rclcpp::shutdown();
    return 0;
}