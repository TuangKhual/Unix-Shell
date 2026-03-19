#ifndef SHELL_H
#define SHELL_H
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

class Shell {
public:
    void loop();
    vector<string> splitLine(const string& line);
    bool runCd(const vector<string>& tokens);
    void runExecvp(const vector<std::string>& tokens);
};

#endif