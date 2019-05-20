// @author QinFeng
#include "main.h"
#include "Navigation.h"
void navigation(){
    if(nState == STATE_COMEBACK)
        srvName.request.name = "master";
    else
        srvName.request.name = strGoto;
    if (cliGetWPName.call(srvName)){
        std::string name = srvName.response.name;
        float x = srvName.response.pose.position.x;
        float y = srvName.response.pose.position.y;
        if(nState == STATE_COMEBACK)
            ROS_INFO("[STATE_COMEBACK] Get_wp_name = %s (%.2f,%.2f)", strGoto.c_str(),x,y);
        else
            ROS_INFO("[STATE_GOTO] Get_wp_name = %s (%.2f,%.2f)", strGoto.c_str(),x,y);

        MoveBaseClient ac("move_base", true);
        if(!ac.waitForServer(ros::Duration(5.0))){
            ROS_INFO("The move_base action server is no running. action abort...");
        }
        else{
            move_base_msgs::MoveBaseGoal goal;
            goal.target_pose.header.frame_id = "map";
            goal.target_pose.header.stamp = ros::Time::now();
            goal.target_pose.pose = srvName.response.pose;
            ac.sendGoal(goal);
            ac.waitForResult();
            if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
                ROS_INFO("Arrived at %s!",strGoto.c_str());
                if(nState == STATE_COMEBACK){
                    Speak("Hi,master. This is what you wanted.");
                    nState = STATE_PASS;
                }
                else{
                    Speak("OK. I am taking it.");
                    nState = STATE_GRAB;
                }
                nDelay = 0;
            }
            else{
                ROS_INFO("Failed to get to %s ...",strGoto.c_str() );
                if(nState == STATE_COMEBACK)
                    Speak("Failed to go to the master.");
                else
                    Speak("Failed to go to the waypoint.");
                nState = STATE_ASK;
            }
        }
    }
    else{
        ROS_ERROR("Failed to call service GetWaypointByName");
        if(nState == STATE_COMEBACK)
            Speak("There is no waypoint named master.");
        else
            Speak("There is no this waypoint.");
        nState = STATE_ASK;
    }
}
