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
  wait(NULL);
  return;
}

void cd(string ch){
  if(chdir(ch.c_str()) == -1) {
    cout << "the directory" << ch << "does not exist" << endl;
  }
  return;
}

int main(int argc, char **argv) {
  // print the string prompt without a newline, before beginning to read
  // tokenize the input, run the command(s), and print the result
  // do this in a loop
  //char input[1024];
  string path_string = "/bin/:/usr/bin/";
  
  while(1){
    string str;
    vector <string> commands_str;
    cout << "dragonshell > ";
    getline(cin, str);
    //cout << str << endl;
    //char *delim = ' ';

    commands_str = tokenize(str, ";");
    //for (int i =0; i< commands_str.size(); ++i) {
      //cout << ' ' << commands_str[i] << endl;
    //}
    for (int i =0; i< commands_str.size(); ++i) {
      //cout << ' ' << commands_str[i] << endl;
      vector <string> command;
      command = tokenize(commands_str[i], " ");
      if (strcmp(command[0].c_str(), "cd")==0){
        //cout << command[0] << " " << command[1] << endl;
        cd(command[1]);
      }
      if (strcmp(command[0].c_str(), "pwd")==0) {
        pwd();
      }

      if(strcmp(command[0].c_str(), "$PATH")==0) {
        cout << "Current PATH: " << path_string << endl;
      }
      if(strcmp(command[0].c_str(), "a2path")==0) {
        if(command[1].find("$PATH") == 0) {
          int pos = command[1].find(":");
          string path_substr = command[1].substr(pos);
          path_string.append(path_substr);
        }
        else{
          path_string = command[1];
          //cout << "$PATH changed" << endl;
        }
      }
      
    }
    
  }
  
  return 0;
}