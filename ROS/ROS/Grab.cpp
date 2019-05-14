// @author SunQi
#include "main.h"
#include "Grab.h"

static void GrabSwitch(bool inActive)
{
    if(inActive == true)
    {
        behavior_msg.data = "grab start";
        behaviors_pub.publish(behavior_msg);
    }
    else
    {
        behavior_msg.data = "grab stop";
        behaviors_pub.publish(behavior_msg);
    }
}


static void PassSwitch(bool inActive)
{
    if(inActive == true)
    {
        behavior_msg.data = "pass start";
        behaviors_pub.publish(behavior_msg);
    }
    else
    {
        behavior_msg.data = "pass stop";
        behaviors_pub.publish(behavior_msg);
    }
}


void GrabResultCallback(const std_msgs::String::ConstPtr& res)
{
    int nFindIndex = 0;
    nFindIndex = res->data.find("done");
    if( nFindIndex >= 0 )
    {
        bGrabDone = true;
    }
}


void PassResultCallback(const std_msgs::String::ConstPtr& res)
{
    int nFindIndex = 0;
    nFindIndex = res->data.find("done");
    if( nFindIndex >= 0 )
    {
        bPassDone = true;
    }
}

void grab(){
	if(nDelay == 0){
        bGrabDone = false;
        GrabSwitch(true);
    }
    nDelay ++;
    if(bGrabDone == true){
        GrabSwitch(false);
        Speak("I got it. I am coming back.");
        nState = STATE_COMEBACK;
    }
}

void pass(){
	if(nDelay == 0){
        bPassDone = false;
        PassSwitch(true);
    }
    nDelay ++;
    if(bPassDone == true){
        PassSwitch(false);
        Speak("OK. What do you want next?");
        nState = STATE_ASK;
    }
}

