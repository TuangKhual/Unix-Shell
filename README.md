# Unix-Shell
My first Unix shell with basic functions built in C++
a simple shell and supports process creation, command execution and basic built in commands

# Features
- Execute commands useing 'fork()' and 'execvp()'
- Handles 'cd' command using 'chdir()'
- Process management using 'waitpid()'
- Error handling for failures and invalid commands

To run it you just:
```
g++ -Wall -o shell shell.cpp
./shell
```

