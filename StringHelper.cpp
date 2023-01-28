#include "StringHelper.h"

std::string StringHelper::alpha         = "qwertyuioplkjhgfdsazxcvbnm";
std::string StringHelper::alphaUpper    = "QWERTYUIOPLKJHGFDSAZXCVBNM";
std::string StringHelper::num           = "0123456789";
std::string StringHelper::numAndDot     = "0123456789.";

std::string StringHelper::ReplaceString(std::string subject, const std::string& search,
    const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

bool StringHelper::isAlpha(char c) {
    return alpha.find(c)        != std::string::npos;
}

bool StringHelper::isAlphaUpper(char c) {
    return alphaUpper.find(c)   != std::string::npos;
}

bool StringHelper::isNum(char c) {
    return num.find(c)          != std::string::npos;
}

bool StringHelper::isNumAndDot(char c) {
    return numAndDot.find(c)    != std::string::npos;
}