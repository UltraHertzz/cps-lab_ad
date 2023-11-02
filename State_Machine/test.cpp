#include <stdio.h>
#include <iostream>

bool flag = false;

int main(){
  for (int i=0;i<3;i++){
    if (!flag){
      printf("function implemented!\n");
      flag = true;
    }
  }
  return 0;
}
