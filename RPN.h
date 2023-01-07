#ifndef RPN_CPP
#define RPN_CPP

#include <string>
#include <map>
#include <stack>
#include <vector>
#include <iostream>

class RPN {
    public:
        static std::vector<std::string> infixToRPN(std::string inp, int xVal);
};

#endif