#include <stdio.h>

class StateMchine {
public:
  int StateMachine(){

  }
  void AEBState(){
    if(AEB_flag && !recovery_flag){
    // AEB function
    normal_public = false;
    if(speed < 0.02){
      recovery_flag = true;
    }

    }
  }
  void recoveryState(){
    if (recovery_flag && !stop_flag){
      // Recovery function
      if(min_dist > 0.2){
        stop_flag = true;
      }
    }
  }
  void stopState() {
    if (stop_flag && !normal_public){
      // stop func
      if (speed < 0.0){
        AEB_engaged = true;
      }
    }
  }
  void Init_state(){
    if(AEB_flag && recovery_flag && stop_flag && AEB_engaged){
      normal_public = true;
      AEB_flag = false;
      recovery_flag = false;
      stop_flag = false;
      AEB_engaged = false;
    }
  }
  void normalPub(){
    if(normal_public){
        // normal public
    }

  }
private:
    bool AEB_flag = false;
    bool recovery_flag = false;
    bool stop_flag = false;
    bool AEB_engaged = false;
    bool normal_public = false;
    float speed = 0.0;
    float min_dist = 0.0;
    char msg;
};