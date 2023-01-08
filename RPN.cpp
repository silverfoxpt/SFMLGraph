#include "RPN.h"

std::map<char, int> RPN::precedence = {
    {'^',3},
    {'*',2},
    {'/',2},
    {'+',1},
    {'-',1},
};
std::string RPN::alpha = "qwertyuiopasdfghjklzxcvbnm";
std::string RPN::num = "1234567890.";

std::vector<std::string> RPN::infixToRPN(std::string inp, int xVal)  {
    std::string xPress = std::to_string(xVal);

    //conversion
    std::vector<std::string> RPN;
    std::stack<char> operators;

    std::string curNumStr = "";

    for (int i = 0; i < inp.size(); i++) {
        char c = inp[i];
        if (c == 'x') {
            RPN.push_back(xPress);
        } 
        
        //not an operator
        else if (precedence.find(c) == precedence.end() || c == '(' || c == ')') { 
            //right bracket
            if (c == '(') { 
                if (!curNumStr.empty()) {
                    RPN.push_back(curNumStr); curNumStr = "";
                }
                operators.push(c);
            } 
            
            //left bracket
            else if (c == ')') { 
                if (!curNumStr.empty()) {
                    RPN.push_back(curNumStr); curNumStr = "";
                }
                while(operators.size() > 0 && operators.top() != '(') {
                    RPN.push_back(std::string(1, operators.top())); operators.pop();
                }
                operators.pop();
            } 
            
            //number
            else {
                curNumStr += c; 
            }
            
        } 
        
        //operator
        else { 
            if (!curNumStr.empty()) {
                RPN.push_back(curNumStr); curNumStr = "";
            }

            //While there is an operator O2 at the top of the operator stack with greater or equal precedence than O1, 
            //pop O2 from the operator stack and push it on the output.
            int pred = precedence[c];
            while(operators.size() > 0) {
                int otherPred = precedence[operators.top()];

                if (otherPred >= pred) {
                    RPN.push_back(std::string(1, operators.top()));
                    char cur = operators.top(); operators.pop();                    
                } 
                else {break;}
            }

            //Push O1 on the operator stack.
            operators.push(c);
        }
    }

    //curNumStr != empty -> push
    if (!curNumStr.empty()) {RPN.push_back(curNumStr);}

    //operator not empty -> push
    while(operators.size() > 0) {
        RPN.push_back(std::string(1, operators.top())); operators.pop();
    }
    return RPN;
}

float RPN::RPNToValue(std::vector<std::string> &rp) {
    std::stack<float> val;
    for (const std::string &x: rp) {
        if (num.find(x[0]) != std::string::npos) { //is a number
            val.push(std::stof(x));
        } 
        
        else { //operator
            switch(x[0]) {
                case '+': {
                    float x = val.top(); val.pop();
                    float y = val.top(); val.pop();
                    val.push(y+x);
                    break;
                }

                case '-': {
                    float x = val.top(); val.pop();
                    float y = val.top(); val.pop();
                    val.push(y-x);
                    break;
                }

                case '*': {
                    float x = val.top(); val.pop();
                    float y = val.top(); val.pop();
                    val.push(y*x);
                    break;
                }

                case '/': {
                    float x = val.top(); val.pop();
                    float y = val.top(); val.pop();
                    val.push(y/x);
                    break;
                }

                case '^': {
                    float x = val.top(); val.pop();
                    float y = val.top(); val.pop();
                    val.push(pow(y, x));
                    break;
                }

                default: {
                    std::cout << "Unknown" << '\n'; break;
                }
            }
        }
    }
    return val.top();
}