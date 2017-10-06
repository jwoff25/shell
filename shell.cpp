#include <stdio.h>
#include <iostream>
#include <string.h>

#define DIR "/home/jeremy/"

using namespace std;

void parse(char* [], int);

void echo(char* [], int);

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
    split_input[tokenCount] = NULL;
    //parse
    parse(split_input, tokenCount);
  }
  return 0;
}

//parse commands using if statements
void parse(char* c[], int count){
  //if input is echo
  if (strcmp(c[0], "echo\n")){
    echo(c, count);
  }
  //else if (str)
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
