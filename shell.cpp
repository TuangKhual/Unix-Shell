#include "shell.h"

#include <sys/types.h>   // pid_t
#include <sys/wait.h>    // waitpid
#include <unistd.h>      // fork, execvp, chdir
#include <cstring>       // strerror
#include <cerrno>        // errno
using namespace std;

vector<string> Shell::splitLine(const string& line) {
    vector<string> tokens;
    stringstream ss(line);
    string token;
    while (ss >> token) {    // extracts
        tokens.push_back(token);
    }
    return tokens;
}

bool Shell::runCd(const vector<string>& tokens) {
    const string& cmd = tokens[0];
    const char* target;
    string path;
    if (cmd == "cd") {
        if (tokens.size() == 2) { // if the user typed something after cd
            target = tokens[1].c_str();
        } else if (tokens.size() > 2) {
            for (size_t i = 1; i < tokens.size(); i++){
                if (i != 1){
                    path += " "; 
                }
                path += tokens[i]; // makes the total folder name the path
            }
            target = path.c_str(); 
        }
        else {
            target = getenv("HOME"); // brings back to home directory
        }

        if (target == nullptr) {
            cerr << "cd: HOME not set" << endl;
        } else if (chdir(target) != 0) {  // 0 means success -1 is fail
            cerr << "cd: " << target << ": " << strerror(errno) << endl; // prints the reason for the failure
        }
        return true; // returns true if the cd was used
    }

    return false;   // needs a fork
}

void Shell::runExecvp(const vector<string>& tokens) {
    vector<char*> argv;   
    int status = 0;
    for (const string& token : tokens) { 
        argv.push_back(const_cast<char*>(token.c_str())); //  converts to c type string 
    }
    argv.push_back(nullptr);    // required null operator for execvp

    pid_t pid = fork();

    if (pid < 0) {
        cerr << "fork: " << strerror(errno) << "\n"; //
        return;
    } else if (pid == 0) {
        execvp(argv[0], argv.data()); // if it succeed it starts a new program

        cerr << argv[0] << ": " << strerror(errno) << "\n"; // only runs if execvp returns; return = fail
        _exit(1);   // use _exit in the child 1 for general error
    } else {
        waitpid(pid, &status, WUNTRACED);
    }
}

void Shell::loop()
{
    while (true) {
        cout << "> " << flush;   // flush so prompt appears before getline blocks


        string line;
        if (!getline(cin, line)) { // works unless ctrl d is pressed
            cout << endl;
            break;
        }

        vector<string> tokens = splitLine(line);

        if (tokens.empty()) {
            continue;   // blank line — show the prompt again
        }
        if (tokens[0] == "quit"){
            break;
        }

        if (!runCd(tokens)) {
            runExecvp(tokens);
        }
    }
}

int main()
{
    Shell shell;
    shell.loop();
    return 0;
}
