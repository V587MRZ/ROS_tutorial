#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "base_interfaces_demo/action/nav.hpp"

using base_interfaces_demo::action::Nav;
using namespace std::chrono_literals;
using namespace std::placeholders;
class ExeNavActionClient : public rclcpp::Node
{
public:
    ExeNavActionClient() : Node("exe_nav_action_client")
    {
        nav_client = rclcpp_action::create_client<Nav>(this, "nav");
    }
    void send_goal(float x, float y, float theta)
    {
        if (!nav_client->wait_for_action_server(5s))
        {
            RCLCPP_ERROR(this->get_logger(), "服务连接失败!");
            return;
        }
        auto goal_msg = Nav::Goal();
        goal_msg.goal_x = x;
        goal_msg.goal_y = y;
        goal_msg.goal_theta = theta;
        rclcpp_action::Client<base_interfaces_demo::action::Nav>::SendGoalOptions options;
        options.feedback_callback = std::bind(&ExeNavActionClient::feedback_callback, this, _1, _2);
        options.goal_response_callback = std::bind(&ExeNavActionClient::goal_response_callback, this, _1);
        options.result_callback = std::bind(&ExeNavActionClient::result_callback, this, _1);
        // rclcpp_action::Client<base_interfaces_demo::action::Nav>::SendGoalOptions &options
        // = rclcpp_action::Client<base_interfaces_demo::action::Nav>::SendGoalOptions())
        nav_client->async_send_goal(goal_msg, options);
    }

private:
    rclcpp_action::Client<Nav>::SharedPtr nav_client;
    void goal_response_callback(rclcpp_action::ClientGoalHandle<Nav>::SharedPtr goal_handle)
    {
        if (!goal_handle)
        {
            RCLCPP_ERROR(this->get_logger(), "目标请求被服务器拒绝");
        }
        else
        {
            RCLCPP_INFO(this->get_logger(), "目标请求被接收!");
        }
    }
    void feedback_callback(rclcpp_action::ClientGoalHandle<Nav>::SharedPtr goal_handle,
                           const std::shared_ptr<const Nav::Feedback> feedback)
    {
        (void)goal_handle;
        RCLCPP_INFO(this->get_logger(), "距离目标点还有 %.2f 米。", feedback->distance);
    }
    void result_callback(const rclcpp_action::ClientGoalHandle<Nav>::WrappedResult &result)
    {
        switch (result.code)
        {
        case rclcpp_action::ResultCode::SUCCEEDED:
            RCLCPP_INFO(this->get_logger(),
                        "乌龟最终坐标:(%.2f,%.2f),航向:%.2f",
                        result.result->turtle_x,
                        result.result->turtle_y,
                        result.result->turtle_theta);
            break;
        case rclcpp_action::ResultCode::CANCELED:
            RCLCPP_ERROR(this->get_logger(), "任务被取消");
            break;
        case rclcpp_action::ResultCode::ABORTED:
            RCLCPP_ERROR(this->get_logger(), "任务被中止");
            break;
        default:
            RCLCPP_ERROR(this->get_logger(), "未知异常");
            break;
        }
    }
};
int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ExeNavActionClient>();
    if (argc != 5)
    {
        RCLCPP_INFO(node->get_logger(), "请传入目标的位姿参数:(x,y,theta)");
        return 1;
    }
    node->send_goal(atof(argv[1]), atof(argv[2]), atof(argv[3]));
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}