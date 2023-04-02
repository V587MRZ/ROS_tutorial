/*
  需求：编写动作服务端实习，可以提取客户端请求提交的整型数据，并累加从1到该数据之间的所有整数以求和，
       每累加一次都计算当前运算进度并连续反馈回客户端，最后，在将求和结果返回给客户端。
  步骤：
    1.包含头文件；
    2.初始化 ROS2 客户端；
    3.定义节点类；
      3-1.创建动作服务端；
      3-2.处理请求数据；
      3-3.处理取消任务请求；
      3-4.生成连续反馈。
    4.调用spin函数，并传入节点对象指针；
    5.释放资源。

*/
// #include <rclcpp/rclcpp.hpp>
// #include <rclcpp_action/rclcpp_action.hpp>
// #include <base_interfaces_demo/action/progress.hpp>
// using base_interfaces_demo::action::Progress;
// class MinimalActionServer : public rclcpp::Node
// {
// private:
//   rclcpp_action::Server<Progress>::SharedPtr action_server_;

// public:
//   MinimalActionServer() : Node("minimal_action_server")
//   {
//     RCLCPP_INFO(this->get_logger(), "Action server is ready");
//     this->action_server_ = rclcpp_action::create_server<Progress>(
//         this, "get_sun",
//         std::bind(&MinimalActionServer::handle_goal, this, std::placeholders::_1, std::placeholders::_2),
//         std::bind(&MinimalActionServer::handle_cancel, this, std::placeholders::_1),
//         std::bind(&MinimalActionServer::handle_accepted, this, std::placeholders::_1));
//     //  /// Signature of a callback that accepts or rejects goal requests.
//     //   using GoalCallback = std::function<GoalResponse(
//     //         const GoalUUID &, std::shared_ptr<const typename ActionT::Goal>)>;
//     //   /// Signature of a callback that accepts or rejects requests to cancel a goal.
//     //   using CancelCallback = std::function<CancelResponse(std::shared_ptr<ServerGoalHandle<ActionT>>)>;
//     //   /// Signature of a callback that is used to notify when the goal has been accepted.
//     //   using AcceptedCallback = std::function<void (std::shared_ptr<ServerGoalHandle<ActionT>>)>;
//   }
//   rclcpp_action::GoalResponse handle_goal(
//       const rclcpp_action::GoalUUID &uuid, std::shared_ptr<const typename Progress::Goal> goal)
//   {
//     RCLCPP_INFO(this->get_logger(), "Received goal request, the number is %d", goal->num);
//     (void)uuid;
//     // reject the goal reason
//     if (goal->num < 1)
//     {
//       return rclcpp_action::GoalResponse::REJECT;
//     }
//     return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
//   }
//   rclcpp_action::CancelResponse handle_cancel(std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>> goal_handle)
//   {
//     RCLCPP_INFO(this->get_logger(), "Received request to cancel goal");
//     (void)goal_handle;
//     return rclcpp_action::CancelResponse::ACCEPT;
//   }

//   void handle_accepted(std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>> goal_handle)
//   {
//     RCLCPP_INFO(this->get_logger(), "Goal accepted");
//     std::thread{std::bind(&MinimalActionServer::execute, this, std::placeholders::_1), goal_handle}.detach();
//   }
//   void execute(std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>> goal_handle)
//   {
//     RCLCPP_INFO(this->get_logger(), "Executing goal");
//     rclcpp::Rate loop_rate(10);
//     auto goal = goal_handle->get_goal();
//     auto feedback = std::make_shared<Progress::Feedback>();
//     auto result = std::make_shared<Progress::Result>();
//     int sum = 0;
//     for (int i = 1; i <= goal->num; i++)
//     {
//       if (goal_handle->is_canceling())
//       {
//         result->sun = sum;
//         goal_handle->canceled(result);
//         RCLCPP_INFO(this->get_logger(), "Goal canceled");
//         return;
//       }
//       feedback->progress = (double_t)i / goal->num;
//       goal_handle->publish_feedback(feedback);
//       RCLCPP_INFO(this->get_logger(), "Publish feedback, progress is %f", feedback->progress);
//       loop_rate.sleep();
//     }
//     if (rclcpp::ok())
//     {
//       result->sun = sum;
//       goal_handle->succeed(result);
//     }
//   }
// };
// int main(int argc, char **argv)
// {
//   rclcpp::init(argc, argv);
//   auto action_server = std::make_shared<MinimalActionServer>();
//   rclcpp::spin(action_server);
//   rclcpp::shutdown();
//   return 0;
// }
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "base_interfaces_demo/action/progress.hpp"
using namespace rclcpp_action;
using base_interfaces_demo::action::Progress;
using namespace std::placeholders;
class MinimalActionServer : public rclcpp::Node
{
public:
  MinimalActionServer() : Node("Minimal_action_server")
  {
    // template<typename ActionT, typename NodeT>
    // typename Server<ActionT>::SharedPtr
    // create_server(
    //   NodeT node,
    //   const std::string & name,
    //   typename Server<ActionT>::GoalCallback handle_goal,
    //   typename Server<ActionT>::CancelCallback handle_cancel,
    //   typename Server<ActionT>::AcceptedCallback handle_accepted,
    //   const rcl_action_server_options_t & options = rcl_action_server_get_default_options(),
    //   rclcpp::CallbackGroup::SharedPtr group = nullptr)
    //     using GoalCallback = std::function<GoalResponse(
    //       const GoalUUID &, std::shared_ptr<const typename ActionT::Goal>)>;
    // /// Signature of a callback that accepts or rejects requests to cancel a goal.
    // using CancelCallback = std::function<CancelResponse(std::shared_ptr<ServerGoalHandle<ActionT>>)>;
    // /// Signature of a callback that is used to notify when the goal has been accepted.
    // using AcceptedCallback = std::function<void (std::shared_ptr<ServerGoalHandle<ActionT>>)>;
    action_server_ = create_server<Progress>(this, "get_sum",
                                             std::bind(&MinimalActionServer::handle_goal, this, _1, _2),
                                             std::bind(&MinimalActionServer::handle_cancel, this, _1),
                                             std::bind(&MinimalActionServer::handle_accepted, this, _1));
    RCLCPP_INFO(this->get_logger(), "动作服务端创建，等待请求...");
  }
  GoalResponse handle_goal(const GoalUUID &uuid, std::shared_ptr<const typename Progress::Goal> goal)
  {
    (void)uuid;
    RCLCPP_INFO(this->get_logger(), "接收到动作客户端请求，请求数字为 %ld", goal->num);
    if (goal->num < 1)
    {
      return GoalResponse::REJECT;
    }
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
  }
  CancelResponse handle_cancel(std::shared_ptr<ServerGoalHandle<Progress>> goal_handle)
  {
    (void)goal_handle;
    RCLCPP_INFO(this->get_logger(), "接收到任务取消请求");
    return CancelResponse::ACCEPT;
  }
  void excute(std::shared_ptr<ServerGoalHandle<Progress>> goal_handle)
  {
    RCLCPP_INFO(this->get_logger(), "misson begin");
    rclcpp::Rate loop_rate(10.0);
    auto goal = goal_handle->get_goal();
    auto feedback = std::make_shared<Progress::Feedback>();
    auto result = std::make_shared<Progress::Result>();
    
    int sum = 0;
    for (int i = 1; (i <= goal->num && rclcpp::ok()); i++)
    {
      if (goal_handle->is_canceling())
      {
        result->sun = sum;
        goal_handle->canceled(result);
        RCLCPP_INFO(this->get_logger(), "mission cancel");
        return;
      }
      sum += i;
      feedback->progress = (double)i / goal->num;
      goal_handle->publish_feedback(feedback);
      RCLCPP_INFO(this->get_logger(), "连续反馈中，进度：%.2f", feedback->progress);
      loop_rate.sleep();
    }
    if (rclcpp::ok())
    {
      result->sun = sum;
      goal_handle->succeed(result);
      RCLCPP_INFO(this->get_logger(), "任务完成！");
    }
  }
  void handle_accepted(std::shared_ptr<ServerGoalHandle<Progress>> goal_handle)
  {
    std::thread{std::bind(&MinimalActionServer::excute, this, _1), goal_handle}.detach();
  }

private:
  Server<Progress>::SharedPtr action_server_;
};
int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalActionServer>());
  rclcpp::shutdown();
  return 0;
}