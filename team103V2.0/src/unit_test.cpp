#include "unit_test.h"
#include <sstream>

using namespace std;

// @author :ChenQixian
void follow_test() {
    cout << "follow_test start..." << endl;
    nDelay = 0;
    follow();
    assert(nDelay == 1);
    cout << "follow_test done." << endl;
}


void start_test() {
    cout << "start_test start..." << endl;
    nDelay = 99;
    nState = STATE_READY;
    start();
    assert(nDelay == 100);
    assert(nState == STATE_READY);
    start();
    assert(nDelay == 0);
    assert(nState == STATE_FOLLOW);
    cout << "start_test done." << endl;
}



// }
void FollowSwitch_test() {
    cout << "FollowSwitch_test start..." << endl;
    bool inActive = true;
    float inDist = 0.7;
    FollowSwitch(inActive, inDist);
    assert(srvFlw.request.thredhold == inDist);

    inActive = false;
    inDist = 0.5;
    FollowSwitch(inActive, inDist);
    assert(srvFlw.request.thredhold != inDist);
    cout << "FollowSwitch_test done." << endl;
}
//@author qinfeng
void xxpass_test() {
    cout << "xxpass_test start..." << endl;
    nDelay = 0;
    nState = STATE_PASS;
    bPassDone = true;
    xxpass();
    assert(nDelay == 1);
    assert(bPassDone == false);
    assert(nState == STATE_PASS);

    bPassDone = true;
    xxpass();
    assert(nDelay == 2);
    assert(bPassDone == true);
    assert(nState == STATE_ASK);
    cout << "xxpass_test done." << endl;
}


void GrabResultCallback_test() {
    cout << "GrabResultCallback_test start..." << endl;
    ros::NodeHandle n;
    ros::Publisher grab_result_pub = n.advertise<std_msgs::String>("/wpb_home/grab_result",30);
    std_msgs::String msg;

    msg.data = "foo";
    bGrabDone = false;
    grab_result_pub.publish(msg);
    assert(bGrabDone == false);

    msg.data = "done";
    bGrabDone = false;
    grab_result_pub.publish(msg);
    assert(bGrabDone == true);
    cout << "GrabResultCallback_test done." << endl;
}


void PassResultCallback_test() {
    cout << "PassResultCallback_test start..." << endl;
    ros::NodeHandle n;
    ros::Publisher pass_result_pub = n.advertise<std_msgs::String>("/wpb_home/pass_result",30);
    std_msgs::String msg;

    msg.data = "foo";
    bPassDone = false;
    pass_result_pub.publish(msg);
    assert(bPassDone == false);

    msg.data = "done";
    bPassDone = false;
    pass_result_pub.publish(msg);
    assert(bPassDone == true);
    cout << "PassResultCallback_test done." << endl;
}
