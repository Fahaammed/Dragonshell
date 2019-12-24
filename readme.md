DRAGONSHELL is a program which imitates the linush command line and runs the given commands. It handles basic commands(ls, cd, a2path, background running using '&' etc) on it's own and can run other commands if the path of the command is given. In this design I used multiple processes working simultaneously to handle commands.

i) Design Choices
     the design is very similar to that of the shell. It asks user for inputs in a while loop unless ctrl+D is not pressed. It is very modular as each pattern match leads to a diffrent function call.

ii) System calls used
    in main():
        sigemptyset() to handle interrupt handler
        sigaction() to handle interrupt handler
        getline() reads the input from the user
        string.size() to get the size of the command string
        find() find the "&" and ">" charecters in command
        dup(1) to get the standard I/O stream
        open() to open a file in the given pathname
        erase() to shorten the strings to feed it into the command_handler
        fork() to create child processes
        dup2() to change the standard I/O stream to specified file/path
        getpid() to get the pid of the child/parent process
        wait(NULL) makes the parent wait till the child finishes
        substr() to create a substring to find the exact command
        close() to close a file pointer

    in sigint_handler() and sigtstp_handler()
        getpid() to get the pid of the child/parent process
        kill() to kill a child process
    
    in pwd()
        getcwd() gets the current directory and puts it in char *dir

    in cd()
        chdir() sets the current directory according to the arguments
    
    in external_programs()
        fork() to create child processes
        size() to get the size of the command string
        string.back() to get the last char of the string
        execve() to execute a program specified by the path
        string.c_str() to convert a string to const char*
        wait(NULL) makes the parent wait till the child finishes

    in do_dup()
        open() to open a file in the given pathname
        dup2() to change the standard I/O stream to specified file/path
        close() to close a file pointer

    in exit_function()
        kill() to kill a child process
        _exit(0) to exit the program

    in commandHandler()
        strcmp() to compare strings in pattern matching
        string.size() to get the size of the command string
        string.c_str() to convert a string to const char*
        find() find the position of "$PATH" string and ":" charecter in command
        string.append() to append to path_string the new paths
        vector<string>.push_back() to push new paths into the vector string of path
        vector<string>.clear() to clear the vector string of paths if a2path has the                        argument a path


iii) tests

    * using valgring tools to detect memory leaks
    * used the bult in commands with diffrent arguments
    * used different external programs
    * testing against bash outputs
    * diffrent and unusual edge cases


iv) sources

    * used both lab1 and lab2 slides heavily.
    * http://www.cplusplus.com/doc/tutorial/functions/ for looking up differrent        functions
    * stackoverflow for diffrent reasons
        - how to convert between string, char* , const char * 
        - how to manipulate vector strings
        - diffrent errors caused by design choices

v) known problems
    * Does not implement piping
    * some background processes dont work as needs a ctr+D to get back to the main process. the fork in background process handling returns the pid of the parent and that pid is not killed.
