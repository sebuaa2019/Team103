// author@ ChrisChen
#include "main.h"
#include "yuyin.h"

/*
接受参数：消息数据结构体指针
返回结果：無
功能定义：更随导购员完成初始化航点
*/
void dstSetUp(const std_msgs::String::ConstPtr & msg){
	// 从识别结果句子中查找物品（航点）关键词
    string strKeyword = FindKeyword(msg->data);
    markPoints(strKeyword);

    // 停止跟随的指令
    int nFindIndex = msg->data.find("top follow");
    stopFollow(nFindIndex);
}

/*
接受参数：無
返回结果：無
功能定义：监听物品抓取指令信息
*/
void fetchActionListener(){
	// 从识别结果句子中查找物品（航点）关键词
    string strKeyword = FindKeyword(msg->data);
    int nLenOfKW = strlen(strKeyword.c_str());
    if(nLenOfKW > 0)
    {
        // 发现物品（航点）关键词
        strGoto = strKeyword;
        string strSpeak = strKeyword + " . OK. I will go to get it for you.";
        Speak(strSpeak);
        nState = STATE_GOTO;
    }
}


/*
接受参数：查找到的航点字符串
返回结果：無
功能定义：标记航点
*/
void markPoints(string strKeyword){
	int nLenOfKW = strlen(strKeyword.c_str());
    if(nLenOfKW > 0)
    {
        // 发现物品（航点）关键词
        AddNewWaypoint(strKeyword);
        string strSpeak = strKeyword + " . OK. I have memoried. Next one , please";
        Speak(strSpeak);
    }
}


/*
接受参数：消息数据中接收到'top following'的信息位置
返回结果：無
功能定义：停止导购跟随，进入抓取监听模式
*/
void stopFollow(int nFindIndex){
    if(nFindIndex >= 0)
    {
        FollowSwitch(false, 0);
        AddNewWaypoint("master");
        nState = STATE_ASK;
        nDelay = 0;
        Speak("OK. What do you want me to fetch?");
    }
}

void AddNewWaypoint(string inStr)
{
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

void Speak(string inStr)
{
    spk_msg.arg = inStr;
    spk_pub.publish(spk_msg);
}

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

static string FindKeyword(string inSentence)
{
    string res = "";
    int nSize = arKeyword.size();
    for(int i=0;i<nSize;i++)
    {
        int nFindIndex = inSentence.find(arKeyword[i]);
        if(nFindIndex >= 0)
        {
            res = arKeyword[i];
            break;
        }
    }
    return res;
}