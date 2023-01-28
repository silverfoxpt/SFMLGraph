#include "RPN.h"
#include "StringHelper.h"

#pragma region vars
std::map<char, int> RPN::precedence = {
    {'^',3},
    {'*',2},
    {'/',2},
    {'+',1},
    {'-',1},

    {'!', 4},
    {'@', 4},
    {'#', 4},
    {'$', 4},

    {'|', 4},

    {'%', 4},
    {'&', 4},

    {'{', 4},
    {'}', 4},
    {'[', 4},
    {']', 4},
};

//commented operations are handled seperately
std::map<char, sf::String> RPN::opDisplay = {
    //optional
    {'(', "("},
    {')', ")"},

    //{'^',3},
    {'*', sf::String(L"⋅")},
    //{'/', ""},
    {'+', "+"},
    {'-', "-"},

    {'!', "sin"},
    {'@', "cos"},
    {'#', "tan"},
    {'$', "cot"},

    //{"abs", '|'},

    //{"sqrt", '%'},
    //{"cbrt", '&'},

    {'{', "exp"},
    {'}', "ln"},
    {'[', "log10"},
    {']', "log2"}
};
std::map<char, textInfoString> RPN::opDefault;

std::string RPN::alpha = "qwertyuiopasdfghjklzxcvbnm";
std::string RPN::num = "1234567890.";
std::map<std::string, char> RPN::opConvert = {
    {"sin", '!'},
    {"cos", '@'},
    {"tan", '#'},
    {"cot", '$'},

    {"abs", '|'},

    {"sqrt", '%'},
    {"cbrt", '&'},

    {"exp", '{'},
    {"ln", '}'},
    {"log10", '['},
    {"log2", ']'},
};
#pragma endregion

void RPN::RPNInitialize(ASTHelper &helper) {
    for (std::map<char, sf::String>::iterator it = opDisplay.begin(); it != opDisplay.end(); it++) {
        char c = it->first;
        sf::String s = it->second;

        textInfoString tmp = helper.GetTextFromDefaultString(s, false);
        tmp.setPriority(-1);
        tmp.setPrevOp('?');

        opDefault[c] = tmp;
    }
}

std::string RPN::MultiplicationInfixToRPNAdder(std::string inp) {
    std::string res = inp;

    int x = 0;
    while(x < (int) res.size()-1) {
        char c = res[x];
        char d = res[x+1];
        if (StringHelper::isNum(c) && (d == '(' || StringHelper::isAlpha(d))) {
            res.insert(x+1, "*");
            //std::cout << c << " " << d << " ;1" << '\n';
        } else if (c == ')' && (StringHelper::isNum(d) || StringHelper::isAlpha(d) || d == '(')) {
            res.insert(x+1, "*");
            //std::cout << c << " " << d << " ;2" << '\n';
        } else if (c == 'x' && (StringHelper::isNum(d) || d == '(' || StringHelper::isAlpha(d)) ) {
            res.insert(x+1, "*");
            //std::cout << c << " " << d << " ;3" << '\n';
        }
        x++;
    }
    return res;
}

bool RPN::isTrigFunction(char c) {
    return ((c == '!') || (c == '@') || (c == '#') || (c == '$'));
}

std::vector<std::string> RPN::infixToRPN(std::string inp)  {
    //add multiplcation values
    inp = MultiplicationInfixToRPNAdder(inp);

    //replace uncommon functions
    for (std::map<std::string, char>::iterator it = opConvert.begin(); it != opConvert.end(); it++) {
        inp = StringHelper::ReplaceString(inp, it->first, std::string(1, it->second));
    }

    //conversion
    std::vector<std::string> RPN;
    std::stack<char> operators;

    std::string curNumStr = "";

    for (int i = 0; i < (int) inp.size(); i++) {
        char c = inp[i];
        if (c == 'x') {
            RPN.push_back("x");
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
                    operators.pop();                    
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

float RPN::RPNToValue(std::vector<std::string> &rp, float xVal) {
    //evaluate
    std::stack<float> val;
    for (const std::string &x: rp) {
        if (x[0] == 'x') {
            val.push(xVal);
        }

        else if (num.find(x[0]) != std::string::npos) { //is a number
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

                //TRIG FUNCTIONS
                case '!': {
                    float x = val.top(); val.pop();
                    val.push(std::sin(x));
                    break;
                }

                case '@': {
                    float x = val.top(); val.pop();
                    val.push(std::cos(x));
                    break;
                }

                case '#': {
                    float x = val.top(); val.pop();
                    val.push(std::tan(x));
                    break;
                }

                case '$': {
                    float x = val.top(); val.pop();
                    val.push(1/std::tan(x));
                    break;
                }

                //abs
                case '|': {
                    float x = val.top(); val.pop();
                    val.push(std::abs(x));
                    break;
                }

                //root
                case '%': {
                    float x = val.top(); val.pop();
                    val.push(std::sqrt(x));
                    break;
                }

                case '&': {
                    float x = val.top(); val.pop();
                    val.push(std::cbrt(x));
                    break;
                }

                //log
                case '{': {
                    float x = val.top(); val.pop();
                    val.push(std::exp(x));
                    break;
                }

                case '}': {
                    float x = val.top(); val.pop();
                    val.push(std::log(x));
                    break;
                }

                case '[': {
                    float x = val.top(); val.pop();
                    val.push(std::log10(x));
                    break;
                }

                case ']': {
                    float x = val.top(); val.pop();
                    val.push(std::log2(x));
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

textInfoString RPN::RPNToDisplay(std::vector<std::string> &rpn, ASTHelper &myASTHelper) {
    //evaluate
    //for (std::string s: rpn) {std::cout << s << " ";} std::cout << '\n';
    std::stack<textInfoString> val;

    for (const std::string &x: rpn) {
        if (num.find(x[0]) != std::string::npos) { //is a number 
            textInfoString text = myASTHelper.GetTextFromDefaultString(x, false);
            text.setPriority(9999); //highest priority
            text.setPrevOp('0');

            val.push(text);
        } 

        else if (x[0] == 'x' ) { //is x
            textInfoString text = myASTHelper.GetTextFromDefaultString(x, false);
            text.setPriority(10000); //highest priority for x
            text.setPrevOp('x');

            val.push(text);
        }
        
        else { //operator
            switch(x[0]) {
                case '*': { //brackets both side needed
                    textInfoString leftVal = val.top(); val.pop();
                    textInfoString rightVal = val.top(); val.pop();
                    
                    textInfoString sign = opDefault[x[0]];

                    bool leftBrac = false, rightBrac = false;
                    int rightPri = leftVal.prevPriority, leftPri = rightVal.prevPriority;
                    char rightOp = leftVal.prevOp, leftOp = rightVal.prevOp;

                    if (precedence[x[0]] > leftVal.prevPriority) {
                        textInfoString rightBracket = opDefault[')'];
                        textInfoString leftBracket = opDefault['('];

                        float inc = (float) leftVal.getTotalHeight() / rightBracket.getTotalHeight();
                        //std::cout << leftVal.getTotalHeight() << " " << inc << " " << '\n';

                        myASTHelper.MergeTwoTextsToRight(leftBracket, leftVal, inc, 1.0);
                        myASTHelper.MergeTwoTextsToRight(leftBracket, rightBracket, 1.0, inc);

                        leftVal = leftBracket;
                        rightBrac = true;
                    }

                    if (precedence[x[0]] > rightVal.prevPriority) {
                        textInfoString rightBracket = opDefault[')'];
                        textInfoString leftBracket = opDefault['('];

                        float inc = (float) rightVal.getTotalHeight() / rightBracket.getTotalHeight();
                        myASTHelper.MergeTwoTextsToRight(leftBracket, rightVal, inc, 1.0);
                        myASTHelper.MergeTwoTextsToRight(leftBracket, rightBracket, 1.0, inc);

                        rightVal = leftBracket;
                        leftBrac = true;
                    }

                    if  (
                        (leftPri == 9999    && (rightPri == 10000   || rightBrac || isTrigFunction(rightOp) )) ||
                        (leftPri == 10000   && (rightPri == 9999    || rightBrac || isTrigFunction(rightOp) )) ||
                        (leftBrac           && (rightPri == 9999    || rightBrac || rightPri == 10000 || isTrigFunction(rightOp) )) ||
                        (leftPri == precedence[x[0]]) || //also a multiplication 
                        (isTrigFunction(leftOp) && (rightPri == 9999    || rightBrac || rightPri == 10000 || isTrigFunction(rightOp) ))
                    ) {}
                    else { //only add * sign if upper requirements aren't sastified
                        myASTHelper.MergeTwoTextsToRight(rightVal, sign, 1.0, 1.0);     
                    }
                    
                    myASTHelper.MergeTwoTextsToRight(rightVal, leftVal, 1.0, 1.0);

                    rightVal.setPriority(precedence[x[0]]);
                    rightVal.setPrevOp(x[0]);
                    val.push(rightVal);

                    break;
                }

                case '+': { //no brackets needed, it's the lowest priority anyway
                    textInfoString leftVal = val.top(); val.pop();
                    textInfoString rightVal = val.top(); val.pop();
                    
                    textInfoString sign = opDefault[x[0]];

                    myASTHelper.MergeTwoTextsToRight(rightVal, sign, 1.0, 1.0);
                    myASTHelper.MergeTwoTextsToRight(rightVal, leftVal, 1.0, 1.0);

                    rightVal.setPriority(precedence[x[0]]);
                    rightVal.setPrevOp(x[0]);
                    val.push(rightVal);

                    break;
                }

                case '-': { //brackets needed on leftVal (right side) only
                    textInfoString leftVal = val.top(); val.pop();
                    textInfoString rightVal = val.top(); val.pop();
                    
                    textInfoString sign = opDefault[x[0]];

                    if (precedence[x[0]] >= leftVal.prevPriority) {
                        textInfoString rightBracket = opDefault[')'];
                        textInfoString leftBracket = opDefault['('];

                        float inc = (float) leftVal.getTotalHeight() / rightBracket.getTotalHeight();
                        //std::cout << leftVal.getTotalHeight() << " " << inc << " " << '\n';

                        myASTHelper.MergeTwoTextsToRight(leftBracket, leftVal, inc, 1.0);
                        myASTHelper.MergeTwoTextsToRight(leftBracket, rightBracket, 1.0, inc);

                        leftVal = leftBracket;
                    }

                    myASTHelper.MergeTwoTextsToRight(rightVal, sign, 1.0, 1.0);
                    myASTHelper.MergeTwoTextsToRight(rightVal, leftVal, 1.0, 1.0);

                    rightVal.setPriority(precedence[x[0]]);
                    rightVal.setPrevOp(x[0]);
                    val.push(rightVal);

                    break;
                }

                case '/': {
                    textInfoString leftVal = val.top(); val.pop();
                    textInfoString rightVal = val.top(); val.pop();

                    int maxWidth = std::max(leftVal.getTotalWidth(), rightVal.getTotalWidth()) + 8 * 2;
                    int height = 3;

                    textInfoString slash    = myASTHelper.GetVerticalSlash(maxWidth, height);
                    
                    myASTHelper.MergeTwoTextsToDown(rightVal, slash, 1.0, 1.0);
                    myASTHelper.MergeTwoTextsToDown(rightVal, leftVal, 1.0, 1.0);

                    rightVal.setPriority(precedence[x[0]]);
                    rightVal.setPrevOp(x[0]);
                    val.push(rightVal);

                    break;
                }
                
                case '^': {
                    textInfoString leftVal = val.top(); val.pop();
                    textInfoString rightVal = val.top(); val.pop();

                    //only need brackets for base, exponents are moved upward anyway
                    if (precedence[x[0]] >= rightVal.prevPriority) {
                        textInfoString rightBracket = opDefault[')'];
                        textInfoString leftBracket = opDefault['('];

                        float inc = (float) rightVal.getTotalHeight() / rightBracket.getTotalHeight();
                        myASTHelper.MergeTwoTextsToRight(leftBracket, rightVal, inc, 1.0);
                        myASTHelper.MergeTwoTextsToRight(leftBracket, rightBracket, 1.0, inc);

                        rightVal = leftBracket;
                    }

                    myASTHelper.MergeTwoTextsToUpLeft(rightVal, leftVal, 0.9, 1.0, 0.7);

                    rightVal.setPriority(precedence[x[0]]);
                    rightVal.setPrevOp(x[0]);
                    val.push(rightVal);
                    break;
                }
                
                //Trig func
                case '!' : case '@' : case '#' : case '$' : {
                    textInfoString leftVal = val.top(); val.pop();
                    textInfoString sign = opDefault[x[0]];

                    //add bracket
                    textInfoString rightBracket = opDefault[')'];
                    textInfoString leftBracket = opDefault['('];

                    float inc = (float) leftVal.getTotalHeight() / rightBracket.getTotalHeight();
                    
                    myASTHelper.MergeTwoTextsToRight(sign, leftBracket, 1.0, inc);
                    myASTHelper.MergeTwoTextsToRight(sign, leftVal, 1.0, 1.0);
                    myASTHelper.MergeTwoTextsToRight(sign, rightBracket, 1.0, inc);
                    
                    leftVal = sign;

                    leftVal.setPriority(precedence[x[0]]);
                    leftVal.setPrevOp(x[0]);
                    val.push(leftVal);

                    break;
                }
                /*
                //abs
                case '|': {
                    float x = val.top(); val.pop();
                    val.push(std::abs(x));
                    break;
                }

                //root
                case '%': {
                    float x = val.top(); val.pop();
                    val.push(std::sqrt(x));
                    break;
                }

                case '&': {
                    float x = val.top(); val.pop();
                    val.push(std::cbrt(x));
                    break;
                }

                //log
                case '{': {
                    float x = val.top(); val.pop();
                    val.push(std::exp(x));
                    break;
                }

                case '}': {
                    float x = val.top(); val.pop();
                    val.push(std::log(x));
                    break;
                }

                case '[': {
                    float x = val.top(); val.pop();
                    val.push(std::log10(x));
                    break;
                }

                case ']': {
                    float x = val.top(); val.pop();
                    val.push(std::log2(x));
                    break;
                }*/

                /*default: {
                    std::cout << "Unknown" << '\n'; return textInfoString(); break;
                }*/
            }
        }
    }
    return val.top();
}   