#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>
#define PATH_MAX  4096
#include <sys/wait.h>
#include <fcntl.h>

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
    char *envp[] = {NULL};
    int i = 0;
    string new_path;
    while (i < path_vect.size()){
      
      if(path_vect[i].back() != '/' && i != 0){
        new_path = string(path_vect[i]) + '/' + first_arg;
      }
      else {
        new_path = string(path_vect[i]) + first_arg;
      }
      
      
      if (execve(new_path.c_str(), arr1, envp) ==-1){ 
        //cout << "End on execve no " << i << endl;
        i++;
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

  return;
}
void do_dup(string fileName){
  int file_desc = open(fileName.c_str(), O_CREAT | O_WRONLY, 0666);
  if(file_desc < 0){
    printf("Error opening the file\n");
  }
  cout << "file open successfull"<< endl;
  dup2(file_desc, 1);
  //printf("Tester\n");
  close(file_desc);
  return;
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
      if(commands_str[i].find(">") != -1){
        int pos = commands_str[i].find(">");
        string file_name = commands_str[i].substr(pos+2);
        cout << "position of > charecter "<< pos << " filename: "<< file_name << endl;
        do_dup(file_name);
        string redirected_command = commands_str[i].erase(pos-1, file_name.size()+3);
        cout << "new command: " << redirected_command << endl;
      }
      command = tokenize(commands_str[i], " ");
      if (strcmp(command[0].c_str(), "cd")==0){
        if (command.size() > 1) {
          cd(command[1]);
        }
        else {
          cout << "expected argument to \"cd\"" << endl;
        }
        
      }
      else if (strcmp(command[0].c_str(), "pwd")==0) {
        pwd();
      }

      else if(strcmp(command[0].c_str(), "$PATH")==0) {
        cout << "Current PATH: " << path_string << endl;
      }
      else if(strcmp(command[0].c_str(), "a2path")==0) {
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
      else if(strcmp(command[0].c_str(), "exit")== 0){
        cout << "Exiting" << endl;
        _exit(3);
      }
      

      else {
        external_programs(command,path_vector, command[0]);
      }
    }
  }
  return 0;
}