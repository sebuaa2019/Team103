// author@ ChrisChen
#ifndef YUYIN_H
#define YUYIN_H

using namespace std;

void dstSetUp(const std_msgs::String::ConstPtr & msg);
void fetchActionListener();
void markPoints(string strKeyword);
void stopFollow(int nFindIndex);
void AddNewWaypoint(string inStr);
void Speak(string inStr);
static void FollowSwitch(bool inActive, float inDist);
static string FindKeyword(string inSentence);

#endif YUYIN_H