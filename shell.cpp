#include <stdio.h>
#include <iostream>
#include <string.h>
#include <dirent.h>
#include <algorithm>

#define HOME_DIR "/home/jeremy/CS415/ShellLab"

using namespace std;

// METHODS
void parse(char* [], int); //parse command
void echo(char* [], int); //echo
void ls(); //ls

int main(){
  // VARIABLES
  char input[128];
  char *tok;
  char *split_input[128];
  int tokenCount;

  // INPUT LOOP
  while (true){
    tokenCount = 0;
    cout << "> ";
    //get user input, store into "input"
    fgets(input, 128, stdin);
    //check if input is "aexit" -- if so, break loop
    if (strcmp(input,"aexit\n") == 0){
      break;
    }
    //split input into token array
    tok = strtok(input, " ");
    while (tok != NULL){
      split_input[tokenCount++] = tok;
      tok = strtok(NULL, " ");
    }
    //split_input[tokenCount] = NULL;
    //parse
    parse(split_input, tokenCount);
  }
  return 0;
}

//parse commands using if statements
void parse(char* c[], int count){
  //if input is echo
  if (strcmp(c[0], "echo\n") == 0 or strcmp(c[0], "echo") == 0 ){
    //printf("%d\n", strcmp(c[0], "echo"));
    echo(c, count);
  }
  else if (strcmp(c[0], "ls\n") == 0 or strcmp(c[0], "ls") == 0){
    //printf("%d\n", strcmp(c[0], "ls\n"));
    ls();
  }
  else {
    cout << "Invalid Command" << endl;
  }
}

//echo function
void echo(char * inp[], int count){
  int counter = 1;
  while (counter < count){
    if (counter == count-1){
      cout << inp[counter++];
    }
    else {
      cout << inp[counter++] << " ";
    }
  }
}

//ls function
void ls(){
  //create pointers for directory
  struct dirent *pointer;
  DIR *dir_path;
  //open directory
  dir_path = opendir(HOME_DIR);
  //read each file from directory
  pointer = readdir(dir_path);
  while (pointer != NULL){
    cout << pointer->d_name << endl;
    pointer = readdir(dir_path);
  }
  closedir(dir_path);
}
