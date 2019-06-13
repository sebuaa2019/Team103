#ifndef VOICE_H
#define VOICE_H

#include "gmain.h"
#include "function.h"

std::string FindKeyword(std::string inSentence);

void KeywordCB(const std_msgs::String::ConstPtr & msg);


#endif