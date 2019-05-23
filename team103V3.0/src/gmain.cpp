#include "gmain.h"
//namespace robot{

    std::string strGoto;
    sound_play::SoundRequest spk_msg;
    ros::Publisher spk_pub;
    ros::Publisher vel_pub;
    // std::string strToSpeak = "";
    // std::string strKeyWord = "";

    std::string strToSpeak;
    std::string strKeyWord;

    ros::ServiceClient clientIAT;
    xfyun_waterplus::IATSwitch srvIAT;
    ros::ServiceClient cliGetWPName;
    waterplus_map_tools::GetWaypointByName srvName;
    ros::Publisher add_waypoint_pub;
    ros::ServiceClient follow_start;
    ros::ServiceClient follow_stop;
    ros::ServiceClient follow_resume;
    wpb_home_tutorials::Follow srvFlw;
    ros::Publisher behaviors_pub;
    std_msgs::String behavior_msg;

    ros::Subscriber grab_result_sub;
    ros::Subscriber pass_result_sub;
    bool bGrabDone;
    bool bPassDone;

    // int nState = STATE_READY;
    // int nDelay = 0;

    int nState;
    int nDelay;

    std::vector<std::string> arKeyword;
// // }


    // strToSpeak = "";
    // strKeyWord = "";

    // nState = STATE_READY;
    // nDelay = 0;

    // ros::NodeHandle n;
    // ros::Subscriber sub_sr = n.subscribe("/xfyun/iat", 10, KeywordCB);
    // follow_start = n.serviceClient<wpb_home_tutorials::Follow>("wpb_home_follow/start");
    // follow_stop = n.serviceClient<wpb_home_tutorials::Follow>("wpb_home_follow/stop");
    // follow_resume = n.serviceClient<wpb_home_tutorials::Follow>("wpb_home_follow/resume");
    // cliGetWPName = n.serviceClient<waterplus_map_tools::GetWaypointByName>("/waterplus/get_waypoint_name");
    // add_waypoint_pub = n.advertise<waterplus_map_tools::Waypoint>( "/waterplus/add_waypoint", 1);
    // spk_pub = n.advertise<sound_play::SoundRequest>("/robotsound", 20);
    // spk_msg.sound = sound_play::SoundRequest::SAY;
    // spk_msg.command = sound_play::SoundRequest::PLAY_ONCE;
    // vel_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    // clientIAT = n.serviceClient<xfyun_waterplus::IATSwitch>("xfyun_waterplus/IATSwitch");
    // behaviors_pub = n.advertise<std_msgs::String>("/wpb_home/behaviors", 30);
    // grab_result_sub = n.subscribe<std_msgs::String>("/wpb_home/grab_result",30,&GrabResultCallback);
    // pass_result_sub = n.subscribe<std_msgs::String>("/wpb_home/pass_result",30,&PassResultCallback);
