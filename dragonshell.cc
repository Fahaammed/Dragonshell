#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>
#define PATH_MAX  4096
#include <sys/wait.h>
#include <fcntl.h>
pid_t childPid;                                                                           // childpid1
pid_t childPid2;                                                                          // childpid2
bool background;                                                                          // boolean background
bool redirect;                                                                            // boolean redirect
pid_t mainPid = getpid();                                                                 // mainPid = the shell pid


using namespace std;

std::string path_string = "/bin/:/usr/bin/";                                               // path_string = defined paths

std::vector<std::string> path_vector;                                                      // path_vector will contain the individual paths as strings


/**
 * @brief Tokenize a string 
 * 
 * @param str - The string to tokenize
 * @param delim - The string containing delimiter character(s)
 * @return std::vector<std::string> - The list of tokenized strings. Can be empty
 */
using namespace std;                                                                    
vector<string> tokenize(const string &str, const char *delim) {                        
  char* cstr = new char[str.size() + 1];                                                
  std::strcpy(cstr, str.c_str());                                                       

  char* tokenized_string = strtok(cstr, delim);                                         

  vector<string> tokens;                                                                
  while (tokenized_string != NULL)                                                      
  {
    tokens.push_back(string(tokenized_string));                                         
    tokenized_string = strtok(NULL, delim);                                             
  }
  delete[] cstr;                                                                        
  return tokens;                                                                        
}


void pwd() {                                                                            // PWD execution
    char dir[PATH_MAX];                                                                 // creates a char *dir
    getcwd(dir, PATH_MAX);                                                              // getcwd gets the path of current directory and puts in into char *dir
    if(dir != NULL) {                                                                   // if( directory exist)
      cout << dir << endl;                                                              // print directory
    } 
    else cout << "failed to get directory" << endl;                                     // error checking for getcwd
    return;
}

void cd(string ch){                                                                     // CD execution
  if(chdir(ch.c_str()) == -1) {                                                         // chdir takes the directory passed by the arguments and goes in there 
    cout << "dragonshell: No such file or directory" << endl;                           // if chdir fails then prints error
  }
  return;
}
// external program execution
void external_programs(vector <string> full_command, vector<string> path_vect,string first_arg) { 
  pid_t pid = fork();                                                                   // fork to create a child
  char *arr1[full_command.size()+1];                                                    // declare a char **arr1
  for(unsigned int i = 0; i < full_command.size() ; i++ ){                              // convert the command string into the char **arr1
    arr1 [i] = (char*)full_command[i].c_str();
  }
  arr1[full_command.size()] = NULL;                                                     // insert NULL at the end of **arr1

  if (pid == 0){                                                                        // if child process
    char *envp[] = {NULL};                                                              // declare char *envp = {NULL}
    unsigned int i = 0;                                                                 
    string new_path;                                                                    
    while (i < path_vect.size()){                                                       // while index less than number of paths
      
      if(path_vect[i].back() != '/' && i != 0){                                         // if the last ch of path_vect[i] is not "/" and index != 0
        new_path = string(path_vect[i]) + '/' + first_arg;                              // new_path = path_vector + / + path given by the command
      }
      else {
        new_path = string(path_vect[i]) + first_arg;                                    // else new_path = path_vector + path given by the command
      }
      
      
      if (execve(new_path.c_str(), arr1, envp) ==-1){                                   // execute the command
        i++;                                                                            // increment the index if execve fails
      }
      else{                                                                             
        cout << "ELSE" <<endl;                                                          // else break
        break;
      } 
    }
    if (i == path_vect.size()) {                                                        // if ( index = number of paths)
      cout << "dragonshell: command not found" << endl;                                 // the command is not found
    } 
  }
  else if (pid == -1){                                                                  // fork fail
    cout << "fork failed" << endl;                                                      
    return;
  }
  else if (pid > 0) {                                                                   // parent process
    wait(NULL);                                                                         // wait for the child to finish
    childPid = pid;                                                                     // set childPid
    return;
  }
  else{                                                                                 // wait
    wait(NULL);
    return;
  }  
  
}

void do_dup(string fileName){                                                           // function for doing dup2()
  int file_desc = open(fileName.c_str(), O_CREAT | O_WRONLY, 0666);                     // file_desc = file name to put the outputs
  if(file_desc < 0){                                                                    // open() error
    cout << "Error opening the file "<< endl;                                           
  }
  dup2(file_desc, 1);                                                                   // change the standard I/O stream to file_desc
  close(file_desc);                                                                     // close file_desc
  return;
}
void exit_function(){
  cout << "\nExiting" << endl;                                                            // prints exit message
    if (childPid > 0){                                                                  // kills child processes
      kill(childPid, SIGTERM);                                                          // kills
    }
    _exit(0);                                                                           // exits
}

void commandHandler(string commandstr){                                                 // the function that handles all the commands
          
  vector <string> command;                                                              // vector string command will have the command tokenized command by " "
  command = tokenize(commandstr, " ");                                                  
  if (strcmp(command[0].c_str(), "cd")==0){                                             // pattern matching with cd command
    if (command.size() > 1) {                                                           // if cd has the proper arguments
      cd(command[1]);                                                                   // callind the cd function to execute the cd command
    }
    else {
      cout << "expected argument to \"cd\"" << endl;                                    // else cd does not have proper arguments
    }
        
  }
  else if (strcmp(command[0].c_str(), "pwd")==0) {                                      // pattern matching with pwd command
    pwd();                                                                              // calls for pwd execution
  }

  else if(strcmp(command[0].c_str(), "$PATH")==0) {                                     // pattern matching with $PATH command
    cout << "Current PATH: " << path_string << endl;                                    // prints out the path_str
  }
  else if(strcmp(command[0].c_str(), "a2path")==0) {                                    // pattern matching with a2path command
    if (command.size() > 1) {                                                           // checks to see if a2path has arguments
      if(command[1].find("$PATH") == 0) {                                               // if the argument contains "$PATH" at the beginning
        int pos = command[1].find(":");                                                 // finds the position of ":"
        string path_substr = command[1].substr(pos);                                    // creates a substring from the command string starting at that position ending at the end
        path_string.append(path_substr);                                                // appends that substring including the colon to path_string
        path_substr = command[1].substr(pos+1);                                         // finds the substring without the colon
        path_vector.push_back(path_substr);                                             // pushes that path _substring into the path vector
      }
      else{
        path_string = command[1];
        path_vector.clear();
        path_vector.push_back(path_string);                                             // if the argument is a path then change the initialize path_string to that string
      }
    }
  }
  else if(strcmp(command[0].c_str(), "exit")== 0){                                      // pattern matching exit
    exit_function();
                                                                                        // calling exit function
  }
  
  else {                                                                                //
    external_programs(command,path_vector, command[0]);                                 // calls externalprogram to execute
  }
  return;
}



void sigint_handler(int signal){                                                        // handles the SIGINT interrupt
  if(getpid() != mainPid){
    kill(childPid,SIGINT);                                                              // kill the child process
  }
}


void sigtstp_handler(int signal){                                                       // handles the SIGTSTP interrupt
  if(getpid() != mainPid){
    kill(childPid,SIGTSTP);
  }
}
  
int main(int argc, char **argv) {
  // print the string prompt without a newline, before beginning to read
  // tokenize the input, run the command(s), and print the result
  // do this in a loop
  //char input[1024];
  path_vector.push_back("");                                                            
  path_vector.push_back("/bin/");                                                       
  path_vector.push_back("/usr/bin/");
  
  struct sigaction sa;                                                                  // handling the sigint
  sa.sa_flags = SA_RESTART;                                                             // code taken from lab2 slides
  sigemptyset(&sa.sa_mask);
  sa.sa_handler = sigint_handler;
  sigaction(SIGINT, &sa, NULL);

  struct sigaction st;                                                                  // handling the sigtstp
  st.sa_flags = SA_RESTART;                                                             // code taken from lab2 slides
  sigemptyset(&st.sa_mask);
  st.sa_handler = sigtstp_handler;
  sigaction(SIGTSTP, &st, NULL);

  

  cout << "Welcome to DRAGONSHELL!" << endl;                                            // greeting
  while(std::cin){
    
    string str;                                                                         // str = stores the whole user input
    vector <string> commands_str;                                                       // command_str = stores the whole user input tokenized by ";"
    cout << "dragonshell > ";                                                           // prints the prompt
    getline(cin, str);    
    
                                                                                        // gets the user input

    commands_str = tokenize(str, ";");                                                  // tokenize the user input by ";"
    for (unsigned int i =0; i< commands_str.size(); ++i) {                              // for loop that iterates over the individual tokens/commands
      background = false;                                                               // background bool set to false
      redirect = false;                                                                 // redirect bool set to false
      
      
      size_t find_background = commands_str[i].find("&");                               // find_background = position of "&"
      int saved_stdout;                                                                 
      saved_stdout = dup(1);                                                            // saving the standard i/o stream in saved_stdout

      if(find_background > 2 && find_background != string::npos){                       // if ( & is after a command and & exists)
        background = true;                                                              // set background true
        int pos = commands_str[i].find("&");                                            // pos = position of "&"
        string file_name = "/dev/null";                                                 // file_desc = file name to put the outputs in /dev/null
        int file_desc = open(file_name.c_str(), O_CREAT | O_WRONLY, 0666);              // open file
        if(file_desc < 0){                                                              // checking for errors in file.open
          cout << "Error opening the file "<< endl;
        }
        
        

        string redirected_command = commands_str[i].erase(pos-1);                       // redirected_command = reduced command by removing " &"
        commands_str[i] = commands_str[i].erase(pos-1);                                 // changing the command to be reduced_command
        if(redirected_command.find(">") == string::npos){                               // check for edge case if the redirected output is not in the background else it falls through
          pid_t pid2 = fork();                                                          // fork to crate a child process
          if(pid2 == 0) {                                                               // if it is a child process
            dup2(file_desc, 1);                                                         // change std I/O stream to file_desc
            dup2(file_desc, 2);
            close(file_desc);                                                           // close file_desc
            commandHandler(redirected_command);                                         // pass the command to command handler to execute
            dup2(saved_stdout, 1);                                                      // change the I/0 stream back to standard
            dup2(saved_stdout, 2);                                                      
            //cout << "PID " << getpid() << " is running in the background" << endl;      // print the pid of the background process
          }
          else if (pid2 > 0){
            cout << "PID " << pid2  << " is running in the background" << endl; 
            wait(NULL);                                                                 // make the parent process wait to let the child finiish
            childPid2 = pid2;                                                           // put child pid in the global pid to kill it
          }
          else{
            wait(NULL);
          }  
        }
    }

      size_t find_redirecting = commands_str[i].find(">");                              // find_redirecting = position of ">" in the command
      if(find_redirecting > 2 && find_redirecting != string::npos){                     // if( ">" exists after a command and ">" exists)
        redirect = true;                                                                // set redirect to true
        int pos = commands_str[i].find(">");                                            // pos = position of ">" in the command
        string file_name = commands_str[i].substr(pos+2);                               // file_name = substring after "> "
        do_dup(file_name);                                                              // calling do_dup to dup XD
        string redirected_command = commands_str[i].erase(pos-1, file_name.size()+3);   // redirected_command = "> " and filename erased from the command_str
        commandHandler(redirected_command);                                             // calls command_handler to execute the command
        dup2(saved_stdout, 1);                                                          // return to standard I/O stream
        close(saved_stdout);
      }

      // for background process

      else{
        if(background == false && redirect == false)                                    // if both background and redirecting is false execute the the command
        commandHandler(commands_str[i]);
        
      }
    }
  }
  exit_function();
  return 0;
}