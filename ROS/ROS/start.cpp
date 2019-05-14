// @author LiuLiang
#include "start.h"

static void FollowSwitch(bool inActive, float inDist);
void AddNewWaypoint(string inStr);

// 1、刚启动，准备
void start() {
    if(nState == STATE_READY)
    {
        // 启动后延迟一段时间然后开始跟随
        nDelay ++;
        // ROS_WARN("[STATE_READY] - nDelay = %d", nDelay);
        if(nDelay > 100)
        {
            AddNewWaypoint("start");
            nDelay = 0;
            nState = STATE_FOLLOW;
        }
    }
}
void follow() {
    //2  、跟随阶段
    if(nState == STATE_FOLLOW)
    {
        if(nDelay == 0)
        {
           FollowSwitch(true, 0.7);
        }
        nDelay ++;
    }
}

// 将机器人当前位置保存为新航点
void AddNewWaypoint(string inStr) {
    tf::TransformListener listener;
    tf::StampedTransform transform;
    try
    {
        listener.waitForTransform("/map","/base_footprint",  ros::Time(0), ros::Duration(10.0) );
        listener.lookupTransform("/map","/base_footprint", ros::Time(0), transform);
    }
    catch (tf::TransformException &ex)
    {
        ROS_ERROR("[lookupTransform] %s",ex.what());
        return;
    }

    float tx = transform.getOrigin().x();
    float ty = transform.getOrigin().y();
    tf::Stamped<tf::Pose> p = tf::Stamped<tf::Pose>(tf::Pose(transform.getRotation() , tf::Point(tx, ty, 0.0)), ros::Time::now(), "map");
    geometry_msgs::PoseStamped new_pos;
    tf::poseStampedTFToMsg(p, new_pos);

    waterplus_map_tools::Waypoint new_waypoint;
    new_waypoint.name = inStr;
    new_waypoint.pose = new_pos.pose;
    add_waypoint_pub.publish(new_waypoint);

    ROS_WARN("[New Waypoint] %s ( %.2f , %.2f )" , new_waypoint.name.c_str(), tx, ty);
}

// 跟随模式开关
static void FollowSwitch(bool inActive, float inDist)
{
    if(inActive == true)
    {
        srvFlw.request.thredhold = inDist;
        if (!follow_start.call(srvFlw))
        {
            ROS_WARN("[CActionManager] - follow start failed...");
        }
    }
    else
    {
        if (!follow_stop.call(srvFlw))
        {
            ROS_WARN("[CActionManager] - failed to stop following...");
        }
    }
}
