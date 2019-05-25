#include "unit_test.h"
#include <sstream>

using namespace std;
// @author : Chenqixian
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
