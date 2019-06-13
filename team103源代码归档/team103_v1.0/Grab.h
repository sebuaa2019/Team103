#ifndef GRAB_H
#define GRAB_H
// @author SunQi
static void GrabSwitch(bool inActive);
static void PassSwitch(bool inActive);
void GrabResultCallback(const std_msgs::String::ConstPtr& res);
void PassResultCallback(const std_msgs::String::ConstPtr& res);
void grab(void);
void pass(void);

#endif GRAB_H
