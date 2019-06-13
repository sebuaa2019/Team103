#include "voice.h"



// 从句子里找arKeyword里存在的关键词
std::string FindKeyword(std::string inSentence)
{
    std::string res = "";
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


// 语音识别结果处理函数
void KeywordCB(const std_msgs::String::ConstPtr & msg)
{
    ROS_WARN("------ Keyword = %s ------",msg->data.c_str());
    if(nState == STATE_FOLLOW)
    {
        // 从识别结果句子中查找物品（航点）关键词
        std::string strKeyword = FindKeyword(msg->data);
        int nLenOfKW = strlen(strKeyword.c_str());
        if(nLenOfKW > 0)
        {
            // 发现物品（航点）关键词
            AddNewWaypoint(strKeyword);
            std::string strSpeak = strKeyword + " . OK. I have memoried. Next one , please"; 
            Speak(strSpeak);
        }

        // 停止跟随的指令
        int nFindIndex = msg->data.find("别跟了");
        if(nFindIndex >= 0)
        {
            FollowSwitch(false, 0);
            AddNewWaypoint("master");
            nState = STATE_ASK;
            nDelay = 0;
            Speak("OK. What do you want me to fetch?");
        }
    }

    if(nState == STATE_ASK)
    {
        // 从识别结果句子中查找物品（航点）关键词
        std::string strKeyword = FindKeyword(msg->data);
        int nLenOfKW = strlen(strKeyword.c_str());
        if(nLenOfKW > 0)
        {
            // 发现物品（航点）关键词
            strGoto = strKeyword;
            std::string strSpeak = strKeyword + " . OK. I will go to get it for you."; 
            Speak(strSpeak);
            nState = STATE_GOTO;
        }
    }
}
