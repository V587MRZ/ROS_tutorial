#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "base_interfaces_demo/action/nav.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "turtlesim/msg/pose.hpp"
using base_interfaces_demo::action::Nav;
using namespace std::placeholders;
class ExeNavActionServer : public rclcpp::Node
{
public:
    ExeNavActionServer() : Node("exe_nav_action_server")
    {
        pose_sub = this->create_subscription<turtlesim::msg::Pose>(
            "turtle1/pose", 10, std::bind(&ExeNavActionServer::PoseCallBack, this, _1));
        cmd_vel_pub = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
        nav_action_server = rclcpp_action::create_server<Nav>(
            this,
            "nav",
            std::bind(&ExeNavActionServer::handle_goal, this, _1, _2),
            std::bind(&ExeNavActionServer::handle_cancel, this, _1),
            std::bind(&ExeNavActionServer::handle_accepted, this, _1));
    }

private:
    rclcpp_action::Server<Nav>::SharedPtr nav_action_server;
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_sub;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_pub;
    turtlesim::msg::Pose::SharedPtr turtle1_pose = nullptr;
    rclcpp_action::GoalResponse handle_goal(const rclcpp_action::GoalUUID &goal_uuid, std::shared_ptr<const Nav::Goal> goal)
    {
        (void)goal_uuid;
        RCLCPP_INFO(this->get_logger(), "请求坐标:(%.2f,%.2f),航向:%.2f", goal->goal_x, goal->goal_y, goal->goal_theta);
        if (goal->goal_x < 0 || goal->goal_x > 11.1 || goal->goal_y < 0 || goal->goal_y > 11.1)
        {
            return rclcpp_action::GoalResponse::REJECT;
        }
        return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    }
    void PoseCallBack(const turtlesim::msg::Pose::SharedPtr pose)
    {
        turtle1_pose = pose;
    }
    rclcpp_action::CancelResponse handle_cancel(std::shared_ptr<rclcpp_action::ServerGoalHandle<Nav>> goal_handle)
    {
        (void)goal_handle;
        RCLCPP_INFO(this->get_logger(), "任务取消!");
        return rclcpp_action::CancelResponse::ACCEPT;
    }
    void execute(std::shared_ptr<rclcpp_action::ServerGoalHandle<Nav>> goal_handle)
    {
        RCLCPP_INFO(this->get_logger(), "开始执行任务......");
        float goal_x = goal_handle->get_goal()->goal_x;
        float goal_y = goal_handle->get_goal()->goal_y;
        auto feedback = std::make_shared<Nav::Feedback>();
        auto result = std::make_shared<Nav::Result>();
        rclcpp::Rate rate(1.0);
        while (true)
        {
            if (goal_handle->is_canceling())
            {
                goal_handle->canceled(result);
                return;
            }
            float distance_x = goal_x - turtle1_pose->x;
            float distance_y = goal_y - turtle1_pose->y;
            geometry_msgs::msg::Twist twist;
            double scale = 0.5;
            twist.linear.x = scale * distance_x;
            twist.linear.y = scale * distance_y;
            cmd_vel_pub->publish(twist);
            float distance = sqrt(pow(distance_x, 2) + pow(distance_y, 2));
            if (distance < 0.1)
            {
                result->turtle_x = turtle1_pose->x;
                result->turtle_y = turtle1_pose->y;
                result->turtle_theta = turtle1_pose->theta;
                break;
            }
            feedback->distance = distance;
            goal_handle->publish_feedback(feedback);
            rate.sleep();
        }
        if (rclcpp::ok())
        {
            goal_handle->succeed(result);
            RCLCPP_INFO(this->get_logger(), "任务结束!");
        }
    }
    void handle_accepted(std::shared_ptr<rclcpp_action::ServerGoalHandle<Nav>> goal_handle)
    {
        std::thread{std::bind(&ExeNavActionServer::execute, this, _1), goal_handle}.detach();
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ExeNavActionServer>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}