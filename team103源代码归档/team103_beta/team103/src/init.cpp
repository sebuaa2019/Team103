#include "init.h"

// namespace robot{
    void start() {
        // 启动后延迟一段时间然后开始跟随
        nDelay ++;
        // ROS_WARN("[STATE_READY] - nDelay = %d", nDelay);
        if(nDelay > 100)
        {
            AddNewWaypoint("start");
            nDelay = 0;
            nState = STATE_FOLLOW;
        }
    }

    void follow() {
        if(nDelay == 0)
        {
            FollowSwitch(true, 0.7);
        }
        nDelay ++;
    }
// }
