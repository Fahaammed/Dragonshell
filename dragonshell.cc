#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>
#define PATH_MAX  4096
#include <sys/wait.h>

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


void pwd() {
  pid_t pid = fork();
  if (pid == 0){
    //cout << "went into pid=0" << endl;
    char *dir;
    getcwd(dir, PATH_MAX);
    if(dir != NULL) {
      cout << dir << endl;
    } 
    else cout << "failed to get directory" << endl;
    return;
  }
  if (pid < 0){
    cout << "fork failed" << endl;
    return;
  }
  if (pid > 0) {
    wait(NULL);
    return;
  }
  return;
}

void cd(string ch){
  if(chdir(ch.c_str()) == -1) {
    cout << "dragonshell: No such file or directory" << endl;
  }
  return;
}

void external_programs(vector <string> full_command, vector<string> path_vect,string first_arg) {
  pid_t pid = fork();
  char *arr1[full_command.size()+1];
  for(int i = 0; i < full_command.size() ; i++ ){
    arr1 [i] = (char*)full_command[i].c_str();
  }
  arr1[full_command.size()] = NULL;

  if (pid == 0){
    //char *cstr1 = const_cast<char*>(first_arg.c_str());
    //char *cstr2 = const_cast<char*>(second_arg.c_str());
    
    char *envp[] = {NULL};
    int i = 0;

    while (i < path_vect.size()){
      string new_path = string(path_vect[i]) + first_arg;
      i++;
      if (execve(new_path.c_str(), arr1, envp) ==-1){ 
        cout << "End on execve no " << i << endl;
      }
      else{
        cout << "ELSE" <<endl;
        break;
      } 
    }
    if (i == path_vect.size()) {
      cout << "dragonshell: command not found" << endl;
    } 
  }
  else if (pid == -1){
    cout << "fork failed" << endl;
    return;
  }
  else{
    wait(NULL);
    return;
  }  
  
}

void background_process(vector <string> full_command, vector<string> path_vect,string first_arg, bool background){
  

}
  


int main(int argc, char **argv) {
  // print the string prompt without a newline, before beginning to read
  // tokenize the input, run the command(s), and print the result
  // do this in a loop
  //char input[1024];
  string path_string = "/bin/:/usr/bin/";

  vector <string> path_vector;
  path_vector.push_back("");
  path_vector.push_back("/bin/");
  path_vector.push_back("/usr/bin/");


  while(1){
    string str;
    vector <string> commands_str;
    cout << "dragonshell > ";
    getline(cin, str);

    commands_str = tokenize(str, ";");
    for (int i =0; i< commands_str.size(); ++i) {
      vector <string> command;
      command = tokenize(commands_str[i], " ");
      if (strcmp(command[0].c_str(), "cd")==0){
        if (command.size() > 1) {
          cd(command[1]);
        }
        else {
          cout << "expected argument to \"cd\"" << endl;
        }
        
      }
      if (strcmp(command[0].c_str(), "pwd")==0) {
        pwd();
      }

      if(strcmp(command[0].c_str(), "$PATH")==0) {
        cout << "Current PATH: " << path_string << endl;
      }
      if(strcmp(command[0].c_str(), "a2path")==0) {
        if (command.size() > 1) {
          if(command[1].find("$PATH") == 0) {
            int pos = command[1].find(":");
            string path_substr = command[1].substr(pos);
            path_string.append(path_substr);
            path_substr = command[1].substr(pos+1);
            path_vector.push_back(path_substr);
          }
          else{
            path_string = command[1];
          }
        }
      }
      if(command[0].find("/") == 0) {
        int pos = command[0].find("/");
        if (pos == 0) {
          external_programs(command,path_vector, command[0]);
        }

      }
      if(strcmp(command[0].c_str(), "exit")== 0){
        cout << "Exiting" << endl;
        _exit(3);
      }

      if(commands_str[i].find("&") != -1){
        int pos = commands_str[i].find("&");
        int end_commandPos = commands_str[i].size();
        if((pos+1) == end_commandPos) {
          background_process (command,path_vector, command[0], true)
          cout << "background process found " << endl;
        }
      }
    
      
    }
    
  }
  
  return 0;
}