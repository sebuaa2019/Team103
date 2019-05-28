/* @author team103                                                  */

#include "gmain.h"
#include "init.h"
#include "function.h"
#include "voice.h"
#include "grab.h"
#include "navigation.h"
#include "unit_test.h"


int main(int argc, char** argv)
{

    strToSpeak = "";
    strKeyWord = "";

    nState = STATE_READY;
    nDelay = 0;

    ros::init(argc, argv, "wpb_home_shopping");

    ros::NodeHandle n;
    ros::Subscriber sub_sr = n.subscribe("/xfyun/iat", 10, KeywordCB);
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

    ROS_WARN("[main] team103");

/***    AAO         ***/
    int   sockfd, ffail=0;
    char  recvline[MSGBUFSIZ], sendline[MSGBUFSIZ];
    struct sockaddr_in  servaddr;

    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
        ffail = 1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8000);
    if( !ffail)
        if (inet_pton(AF_INET, "192.168.192.128", &servaddr.sin_addr) <= 0){
            printf("inet_pton error for 192.168.192.128\n");
            ffail = 1;
        }

    if( !ffail) 
        if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
            printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
            ffail = 1;
        }

    printf("able to send msg to server: \n");
    //signal(SIGPIPE, SIG_IGN);
    int mycnt=0;
/*** AAO    ***/

/* unit test begin 
    if (ros::ok())
{
    follow_test();

    start_test();


    FollowSwitch_test();

    //Speak_test_callback(const sound_play::SoundRequest& res);

    Speak_test();
    AddNewWaypoint_test();

    InitKeyword_test();

    xxnav_test();

    GrabSwitch_test();
    PassSwitch_test();

    xxgrab_test();

    xxpass_test();

    GrabResultCallback_test();

    PassResultCallback_test();

    FindKeyword_test();

    Keyword_test_CB();

    xxnav_test();
}
unit test end*/




    ros::Rate r(30);
    while(ros::ok())
    {
        sprintf(sendline, "%d", nState);
        if( !ffail) {
            if (send(sockfd, sendline, strlen(sendline), 0) < 0){
                mycnt++;
                if (mycnt < 10)
                    printf("send msg error: %s(errno: %d)  %s\n", strerror(errno), errno, sendline);
            }else {
                //printf("success to send %s\n", sendline);
            }
        }
        // 1、刚启动，准备
        if(nState == STATE_READY) {
            start();
        }

        // 2、跟随阶段
        if(nState == STATE_FOLLOW) {
            follow();
        }

        // 3、询问要去哪个航点
        if(nState == STATE_ASK) {

        }

        // 4、导航去指定航点
        if(nState == STATE_GOTO) {
            xxgotoitem();
        }

        // 5、抓取物品
        if(nState == STATE_GRAB) {
            xxgrab();
        }

        // 6、抓完物品回来
        if(nState == STATE_COMEBACK) {
            xxcomeback();
        }

        // 7、将物品给主人
        if(nState == STATE_PASS) {
            xxpass();
        }

        ros::spinOnce();
        r.sleep();
    }


    return 0;
}

