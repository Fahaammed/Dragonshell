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
    cout << "went into pid=0" << endl;
    char *dir;
    getcwd(dir, PATH_MAX);
    if(dir != NULL) {
      cout << dir << endl;
    } 
    else cout << "failed to get directory" << endl;
  }
  if (pid < 0){
    wait(NULL);
    cout << "fork failed" << endl;
  }
  return;
}
void cd(){
  
}

int main(int argc, char **argv) {
  // print the string prompt without a newline, before beginning to read
  // tokenize the input, run the command(s), and print the result
  // do this in a loop
  //char input[1024];
  string str;
  vector <string> commands_str;
  while(1){
    cout << "dragonshell > ";
    getline(cin, str);
    //cout << str << endl;
    //char *delim = ' ';

    commands_str = tokenize(str, ";");
    for (int i =0; i< commands_str.size(); ++i) {
      cout << ' ' << commands_str[i];
      cout << '\n';
      vector <string> command;
      command = tokenize(commands_str[i], " ");
      if (strcmp(command[i].c_str(), "pwd")==0) {
        pwd();
      }
      if (strcmp(command[i].c_str(), "cd")==0){
        cout << command[i] << " " << command[i++] << endl;
        cd();
      }
    }
    
  }
  
  return 0;
}