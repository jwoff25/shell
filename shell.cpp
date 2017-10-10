#include <stdio.h>
#include <iostream>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <bits/stdc++.h>
#include <algorithm>

using namespace std;

// GLOBAL VARIABLES
const char *HOME_DIR = getenv("HOME"); //unreliable?
char CUR_DIR[128];

// METHODS
void parse(char* [], int); //parse commands
void echo(char* [], int); //echo
void ls(); //list
void cd(char []); //change directory
void mkdir(char []); //make directory
void remdir(char []); //remove directory

int main(){
  // VARIABLES
  char input[128];
  char *tok;
  char *split_input[128];
  int tokenCount;
  streambuf* coutbuf = cout.rdbuf();
  ofstream out;

  //set current directory
  getcwd(CUR_DIR, 128);

  // INPUT LOOP
  while (true){
    //chdir(CUR_DIR); //change working directory?
    cout.rdbuf(coutbuf); //reset cout buffer to terminal (in case cout is changed)
    tokenCount = 0; //counter for input
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
    //write to file if "> FILENAME" is included
    if (strcmp(split_input[tokenCount - 2], ">") == 0){
      size_t inp_size = strlen(split_input[tokenCount-1]);
      split_input[tokenCount-1][inp_size-1] = '\0';
      out.open(split_input[tokenCount-1]);
      cout.rdbuf(out.rdbuf()); //change cout to file
      split_input[tokenCount - 2] = '\0';
      split_input[tokenCount - 1] = '\0';
    }
    //parse
    parse(split_input, tokenCount);

    //close buffer ()
    out.close();
  }
  return 0;
}

//parse commands using if statements
void parse(char* c[], int count){
  if (strcmp(c[0], "echo\n") == 0 or strcmp(c[0], "echo") == 0 ){
    echo(c, count);
  }
  else if (strcmp(c[0], "ls\n") == 0 or strcmp(c[0], "ls") == 0){
    ls();
  }
  else if (strcmp(c[0], "cd\n") == 0 or strcmp(c[0], "cd") == 0){
    cd(c[1]);
  }
  else if (strcmp(c[0], "mkdir\n") == 0 or strcmp(c[0], "mkdir") == 0){
    mkdir(c[1]);
  }
  else if (strcmp(c[0], "rmdir\n") == 0 or strcmp(c[0], "rmdir") == 0){
    remdir(c[1]);
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
      cout.flush();
    }
    else {
      cout << inp[counter++] << " ";
      cout.flush();
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
      cout << pointer->d_name << " "; //print out directories
    }
    pointer = readdir(dir_path);
  }
  cout << endl;
  closedir(dir_path);
}

//cd function -- easier way to do it?
void cd (char inp[]){
  if (inp == NULL or strcmp(inp,"\n") == 0){
    memcpy(CUR_DIR, HOME_DIR, 128);
  }
  //for cd .. command
  else if (strcmp(inp, "..\n") == 0 or strcmp(inp, "..") == 0){
    if (CUR_DIR[0] != '\0'){
      char temp[128];
      memcpy(temp, CUR_DIR, 128);
      size_t dir_len = strlen(temp);
      while (temp[dir_len] != '/'){
        temp[dir_len--] = '\0';
      }
      temp[dir_len] = '\0';
      memcpy(CUR_DIR, temp, 128);
    }
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
    //start with '/'
    temp[cur_dir_len++] = '/';
    //loop through both arrays, append input to temp
    int counter = 0;
    while (counter < inp_len){
      temp[cur_dir_len++] = inp[counter++];
    }
    //check if directory exists
    if (temp[cur_dir_len-1] == '\n'){
      temp[cur_dir_len-1] = '\0'; //get rid of any pesky newlines
    }
    if (inp[inp_len-1] == '\n'){
      inp[inp_len-1] = '\0'; //get rid of newline for input
    }
    d = opendir(temp);
    if (d) {
      memcpy(CUR_DIR, temp, 128);
    }
    else{
      //check if input is full directory
      d = opendir(inp);
      if (d){
        memcpy(CUR_DIR, inp, 128);
      }
      else {
        cout << "No such file or directory." << endl;
      }
    }
  }
  //cout << CUR_DIR << endl;
}

//mkdir function
void mkdir(char inp[]){
  if (inp == NULL or strcmp(inp,"\n") == 0){
    cout << "Missing directory name." << endl;
  }
  else {
    //printf("%s", inp);
    //get size of both arrays, append
    size_t cur_dir_len = strlen(CUR_DIR);
    size_t inp_len = strlen(inp);
    char temp[128];
    //copy both arrays into temp -- this is like way shorter
    memcpy(temp, CUR_DIR, 128);
    temp[cur_dir_len++] = '/';
    memcpy(temp + cur_dir_len, inp, inp_len);
    if (temp[strlen(temp)-1] == '\n'){
      temp[strlen(temp)-1] = '\0'; //get rid of newline
    }
    if (inp[inp_len-1] == '\n'){
      inp[inp_len-1] = '\0'; //get rid of newline for input
    }
    //make directory
    int status = mkdir(temp, 0700);
    //check for error status
    if (status == -1){
      //check if input is full directory
      status = mkdir(inp, 0700);
      if (status == -1){
        cout << "Error creating directory." << endl;
      }
      //cout << "Directory exists." << endl;
    }
  }
}

//rmdir function
void remdir(char inp[]){
  if (inp == NULL or strcmp(inp,"\n") == 0){
    cout << "Missing directory name." << endl;
  }
  else {
    //get size of both arrays, append
    size_t cur_dir_len = strlen(CUR_DIR);
    size_t inp_len = strlen(inp);
    char temp[128];
    //copy both arrays into temp
    memcpy(temp, CUR_DIR, cur_dir_len);
    temp[cur_dir_len++] = '/';
    memcpy(temp + cur_dir_len, inp, inp_len);
    if (temp[strlen(temp)-1] == '\n'){
      temp[strlen(temp)-1] = '\0'; //get rid of newline
    }
    if (inp[inp_len-1] == '\n'){
      inp[inp_len-1] = '\0'; //get rid of newline for input
    }
    //remove directory
    int status = rmdir(temp);
    //check for error status
    if (status == -1){
      //check if inp is full directory
      status = rmdir(inp);
      if (status == -1){
        cout << "Error removing directory." << endl;
      }
    }
  }
}
