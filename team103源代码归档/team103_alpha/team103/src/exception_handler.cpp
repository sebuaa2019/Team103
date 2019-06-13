#include "exception_handler.h"

void noWayPoint(){
    ROS_ERROR("Failed to call service GetWaypointByName");
    Speak("There is no this waypoint.");
    nState = STATE_ASK;
}

void failToGetWayPoint(){
    ROS_INFO("Failed to get to %s ...",strGoto.c_str() );
    Speak("Failed to go to the waypoint.");
    nState = STATE_ASK;
}

void grabFailed(){
    Speak("I failed to grab the item. I am coming back.");
    nState = STATE_COMEBACK;
}

void passFailed(){
    Speak("I am sorry. I failed to pass the item. What do you want next?");
    nState = STATE_ASK;
}
