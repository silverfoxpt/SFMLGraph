#ifndef H_STRINGHELPER
#define H_STRINGHELPER

#include <string>
#include <map>
#include <stack>
#include <vector>
#include <iostream>
#include <cmath>

class StringHelper {
    public:
        static std::string ReplaceString(std::string subject, const std::string& search,
            const std::string& replace);

        static std::string alpha;
        static std::string alphaUpper;
        static std::string num;
        static std::string numAndDot;

        static bool isAlpha(char c);
        static bool isAlphaUpper(char c);
        static bool isNum(char c);
        static bool isNumAndDot(char c);
};

#endif