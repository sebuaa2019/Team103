/* @author TEAM 103*/
#include "main.h"

void InitKeyword()
{
    arKeyword.push_back("start");   //机器人开始启动的地点,最后要回去
    arKeyword.push_back("water");
    arKeyword.push_back("tea");
    arKeyword.push_back("cola");
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "wpb_home_shopping");

    ros::NodeHandle n;
    ros::Subscriber sub_sr = n.subscribe("/xfyun/iat", 10, dstSetUp);
    follow_start = n.serviceClient<wpb_home_tutorials::Follow>("wpb_home_follow/start");
    follow_stop = n.serviceClient<wpb_home_tutorials::Follow>("wpb_home_follow/stop");
    follow_resume = n.serviceClient<wpb_home_tutorials::Follow>("wpb_home_follow/resume");
    cliGetWPName = n.serviceClient<waterplus_map_tools::GetWaypointByName>("/waterplus/get_waypoint_name");
    add_waypoint_pub = n.advertise<waterplus_map_tools::Waypoint>( "/waterplus/add_waypoint", 1);
    spk_pub = n.advertise<sound_play::SoundRequest>("/robotsound", 20);
    spk_msg.sound = sound_play::SoundRequest::SAY;
    spk_msg.command = sound_play::SoundRequest::PLAY_ONCE;
    vel_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    clientIAT = n.serviceClient<xfyun_waterplus::IATSwitch>("xfyun_waterplus/IATSwitch");
    behaviors_pub = n.advertise<std_msgs::String>("/wpb_home/behaviors", 30);
    grab_result_sub = n.subscribe<std_msgs::String>("/wpb_home/grab_result",30,&GrabResultCallback);
    pass_result_sub = n.subscribe<std_msgs::String>("/wpb_home/pass_result",30,&PassResultCallback);

    InitKeyword();

    ROS_WARN("[main] wpb_home_shopping");
    ros::Rate r(30);
    while(ros::ok())
    {
        // 1、刚启动，准备
        if(nState == STATE_READY)
        {
           start();
        }

        // 2、跟随阶段
        if(nState == STATE_FOLLOW)
        {
            follow();
        }

        // 3、询问要去哪个航点
        if(nState == STATE_ASK)
        {

        }

        // 4、导航去指定航点
        if(nState == STATE_GOTO)
        {
            navigation();
        }

        // 5、抓取物品
        if(nState == STATE_GRAB)
        {
            grab();
        }

        // 6、抓完物品回来
        if(nState == STATE_COMEBACK)
        {
            navigation();
        }

        // 7、将物品给主人
        if(nState == STATE_PASS)
        {
            pass();
        }

        ros::spinOnce();
        r.sleep();
    }

    return 0;
}
