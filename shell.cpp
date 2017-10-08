#include <stdio.h>
#include <iostream>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <algorithm>

using namespace std;

// GLOBAL VARIABLES
const char *HOME_DIR = getenv("HOME"); //unreliable?
char CUR_DIR[128];

// METHODS
void parse(char* [], int); //parse commands
void echo(char* [], int); //echo implementation
void ls(); //ls implementation
void cd(char []); //cd implementation

int main(){
  // VARIABLES
  char input[128];
  char *tok;
  char *split_input[128];
  int tokenCount;

  //set current directory
  getcwd(CUR_DIR, 128);

  // INPUT LOOP
  while (true){
    tokenCount = 0;
    cout << "[" << CUR_DIR << "]$ ";
    //get user input, store into "input"
    fgets(input, 128, stdin);
    //split input into token array
    tok = strtok(input, " ");
    while (tok != NULL){
      split_input[tokenCount++] = tok;
      tok = strtok(NULL, " ");
    }
    split_input[tokenCount] = NULL;
    //check if input is "aexit" -- if so, break loop
    if (strcmp(split_input[0],"aexit\n") == 0 or strcmp(split_input[0],"aexit") == 0){ //with(out) whitespace
      break;
    }
    //parse
    parse(split_input, tokenCount);
  }
  return 0;
}

//parse commands using if statements
void parse(char* c[], int count){
  //if input is echo
  if (strcmp(c[0], "echo\n") == 0 or strcmp(c[0], "echo") == 0 ){
    echo(c, count);
  }
  else if (strcmp(c[0], "ls\n") == 0 or strcmp(c[0], "ls") == 0){
    ls();
  }
  else if (strcmp(c[0], "cd\n") == 0 or strcmp(c[0], "cd") == 0){
    cd(c[1]);
  }
  else {
    cout << "Invalid Command" << endl;
  }
}

//echo function - just loop through rest of array
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

//ls function - using dirent.h
void ls(){
  //create pointers for directory
  struct dirent *pointer;
  DIR *dir_path;
  //open directory
  dir_path = opendir(CUR_DIR);
  //read each file from directory
  pointer = readdir(dir_path);
  while (pointer != NULL){
    if (*pointer->d_name != '.'){
      cout << pointer->d_name << " ";
    }
    pointer = readdir(dir_path);
  }
  cout << endl;
  closedir(dir_path);
}

//cd function -- easier way to do it?
void cd (char inp[]){
  if (inp == NULL or inp == " "){
    memcpy(CUR_DIR, HOME_DIR, 128);
  }
  else { //this is messy
    //for testing DIR later
    DIR *d;
    //get length of current dir and input
    size_t cur_dir_len = strlen(CUR_DIR);
    size_t inp_len = strlen(inp);
    //temp variable to store new directory and copy
    char temp[128];
    memcpy(temp, CUR_DIR, 128);
    //start with /
    temp[cur_dir_len++] = '/';
    //loop through both arrays, append input to temp
    int counter = 0;
    while (counter < inp_len){
      if (inp[counter] != ' ' or inp[counter != '\n']){
        temp[cur_dir_len++] = inp[counter++];
      }
    }
    //check if directory exists
    // d = opendir(temp);
    // if (d) {
    //   memcpy(CUR_DIR, temp, 128);
    // }
    // else{
    //   cout << "Invalid directory." << endl;
    //   return;
    // }
  }
  cout << CUR_DIR << endl;
}
