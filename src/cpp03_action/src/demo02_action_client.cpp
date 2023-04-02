#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "base_interfaces_demo/action/progress.hpp"
using base_interfaces_demo::action::Progress;
using namespace rclcpp_action;
using namespace std::chrono;
using namespace std::placeholders;
class MinimalActionClient : public rclcpp::Node
{
public:
    MinimalActionClient() : Node("minimal_action_client")
    {
        client_ptr_ = rclcpp_action::create_client<Progress>(this, "get_sum");
    }
    void send_goal(int num)
    {
        if (!this->client_ptr_)
        {
            RCLCPP_ERROR(this->get_logger(), "动作客户端未被初始化。");
        }
        if (!client_ptr_->wait_for_action_server(10s))
        {
            RCLCPP_ERROR(this->get_logger(), "服务连接失败！");
            return;
        }
        Progress::Goal goal_msg = Progress::Goal();
        goal_msg.num = num;
        RCLCPP_INFO(this->get_logger(), "发送请求数据！");
        //   std::shared_future<typename GoalHandle::SharedPtr>
        //   async_send_goal(const Goal & goal, const SendGoalOptions & options = SendGoalOptions())
        auto sendGoalOptions = rclcpp_action::Client<Progress>::SendGoalOptions();
        sendGoalOptions.feedback_callback = std::bind(&MinimalActionClient::feedback_callback, this, _1, _2);
        sendGoalOptions.goal_response_callback = std::bind(&MinimalActionClient::goal_response_callback, this, _1);
        sendGoalOptions.result_callback = std::bind(&MinimalActionClient::result_callback, this, _1);
        auto goal_handle_future = this->client_ptr_->async_send_goal(goal_msg, sendGoalOptions);
    }
    // std::function<void (
    //     typename ClientGoalHandle<ActionT>::SharedPtr,
    //     const std::shared_ptr<const Feedback>)>;
    void feedback_callback(typename ClientGoalHandle<Progress>::SharedPtr goal_handle,
                           const std::shared_ptr<const Progress::Feedback> feedback)
    {
        (void)goal_handle;
        RCLCPP_INFO(this->get_logger(), "the progress is %f", feedback->progress);
    }
    // template<class ActionT> std::function
    // <void (rclcpp_action::ClientGoalHandle<ActionT>::SharedPtr)>
    // rclcpp_action::Client<ActionT>::SendGoalOptions::goal_response_callback
    void goal_response_callback(rclcpp_action::ClientGoalHandle<Progress>::SharedPtr goal_handle)
    {
        if (goal_handle)
            RCLCPP_INFO(this->get_logger(), "目标被接收，等待结果中");
        else
            RCLCPP_ERROR(this->get_logger(), "目标请求被服务器拒绝！");
    }
    // std::function<void (const WrappedResult & result)>;
    void result_callback(const ClientGoalHandle<Progress>::WrappedResult &result)
    {
        switch (result.code)
        {
        case rclcpp_action::ResultCode::SUCCEEDED:
            break;
        case rclcpp_action::ResultCode::CANCELED:
            RCLCPP_INFO(this->get_logger(), "任务被取消");
            return;
        default:
            RCLCPP_ERROR(this->get_logger(), "未知异常");
            return;
        }
        RCLCPP_INFO(this->get_logger(), "任务执行完毕，最终结果: %ld", result.result->sun);
        return;
    }

private:
    Client<Progress>::SharedPtr client_ptr_;
};
int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto action_client = std::make_shared<MinimalActionClient>();

    action_client->send_goal(atoi(argv[1]));
    rclcpp::spin(action_client);
    rclcpp::shutdown();
    return 0;
}