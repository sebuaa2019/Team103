#include "unit_test.h"
#include <sstream>

using namespace std;

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




void Speak_test_callback(const sound_play::SoundRequest& res) {
    assert(res.arg == "Speak_test");
}
void Speak_test() {
    cout << "Speak_test start..." << endl;
    ros::NodeHandle n;
    ros::Subscriber spk_msg_test = n.subscribe("/robotsound", 20, &Speak_test_callback);
    std::string str = "Speak_test";
    Speak(str);
    cout << "Speak_test done." << endl;
}


void AddNewWaypoint_test_callback(const waterplus_map_tools::Waypoint& res) {
    assert(res.name == "coffee");
}
void AddNewWaypoint_test() {
    cout << "AddNewWaypoint_test start..." << endl;
    ros::NodeHandle n;
    ros::Subscriber AddNewWaypoint_test_sub = n.subscribe("/waterplus/add_waypoint", 1, &AddNewWaypoint_test_callback);
    std::string str = "coffee";
    AddNewWaypoint(str);
    cout << "AddNewWayPoint_test done." << endl;
}

void InitKeyword_test() {
    InitKeyword();
    std::set<std::string> tmp(arKeyword.begin(), arKeyword.end());
    assert(tmp.count("start"));
    assert(tmp.count("water"));
    assert(tmp.count("tea"));
    assert(tmp.count("cola"));
}


void GrabSwitch_test_callback1(const std_msgs::String& res) {
    assert(res.data == "grab start");
}
void GrabSwitch_test_callback2(const std_msgs::String& res) {
    assert(res.data == "grab stop");
}
void GrabSwitch_test() {
    cout << "GrabSwitch_test start..." << endl;
    ros::NodeHandle n;
    ros::Subscriber behaviors_test_sub = n.subscribe("/wpb_home/behaviors", 30, &GrabSwitch_test_callback1);
    bool inActive = true;
    GrabSwitch(inActive);
    
    // behaviors_test_sub.unsubscribe();
    // behaviors_test_sub = n.subscribe("/wpb_home/behaviors", 30, &GrabSwitch_test_callback2);
    // inActive = false;
    // GrabSwitch(inActive);
    cout << "GrabSwtich_test done." << endl;
}



void PassSwitch_test_callback1(const std_msgs::String& res) {
    assert(res.data == "pass start");
}
void PassSwitch_test_callback2(const std_msgs::String& res) {
    assert(res.data == "pass stop");
}
void PassSwitch_test() {
    cout << "PassSwtich_test start..." << endl;
    ros::NodeHandle n;
    ros::Subscriber behaviors_test_sub = n.subscribe("/wpb_home/behaviors", 30, &PassSwitch_test_callback1);
    bool inActive = true;
    PassSwitch(inActive);
    
    // behaviors_test_sub.unsubscribe();
    // behaviors_test_sub = n.subscribe("/wpb_home/behaviors", 30, &PassSwitch_test_callback2);
    // inActive = false;
    // PassSwitch(inActive);
    cout << "PassSwtich_test done." << endl;
}



void xxgrab_test() {
    cout << "xxgrab_test start..." << endl;
    nDelay = 0;
    nState = STATE_GRAB;
    bGrabDone = true;
    xxgrab();
    assert(nDelay == 1);
    assert(bGrabDone == false);
    assert(nState == STATE_GRAB);

    bGrabDone = true;
    xxgrab();
    assert(nDelay == 2);
    assert(bGrabDone == true);
    assert(nState == STATE_COMEBACK);
    cout << "xxgrab_test done." << endl;
}


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



void FindKeyword_test(){
    cout << "FindKeyword_test start..." << endl;
    std::string args0 = "Bring me a bottle of water";
    std::string args1 = "";
    assert(FindKeyword(args0) == "water");
    assert(FindKeyword(args1) == "");
    cout << "FindKeyword_test done." << endl;
}


void Keyword_test_CB(){
    cout << "Keywrod_test_CB start..." << endl;
    ros::NodeHandle n;
    ros::Publisher keyword_pub = n.advertise<std_msgs::String>("/xfyun/iat", 10);



    // STATE_FOLLOW
    nState = STATE_FOLLOW;
    std_msgs::String msg;


    // std_msgs::String::ConstPtr msgp = &msg;
    
    // ros::NodeHandle n;
    // ros::Subscriber sub_sr = n.subscribe("/xfyun/");

    msg.data = "Bring me a bottle of water.";
    keyword_pub.publish(msg);
    // KeywordCB(msg);

    msg.data = "Stop following.";
    keyword_pub.publish(msg);
    // KeywordCB(msgp);
    assert(nDelay == 0);
    assert(nState == STATE_ASK);

    nState = STATE_ASK;
    msg.data = "Bring me a bottle of water.";
    keyword_pub.publish(msg);
    // KeywordCB(&msgp);
    assert(nState == STATE_GOTO);
    cout << "Keywrod_test_CB done." << endl;
}



void xxgotoitem_test(){
    cout << "xxgotoitem_test start..." << endl;
	srvName.response.pose.position.x = 0.1;
	srvName.response.pose.position.y = 0.1;

	// test case 2
	nState = STATE_GOTO;
	xxgotoitem();
	assert(srvName.request.name == strGoto);
	assert(nState == STATE_GRAB);
	assert(nDelay == 0);
	
	// test case 3
    cout << "xxgotoitem_test done." << endl;
}


void xxcomeback_test(){
    cout << "xxcomeback_test start..." << endl;
	srvName.response.pose.position.x = 0.1;
	srvName.response.pose.position.y = 0.1;


	// test case 1
	nState = STATE_COMEBACK;
	xxcomeback();
	assert(srvName.request.name == "master");
	assert(nState == STATE_PASS);
	assert(nDelay == 0);
	
	// test case 3
    cout << "xxcomeback_test done." << endl;
}
