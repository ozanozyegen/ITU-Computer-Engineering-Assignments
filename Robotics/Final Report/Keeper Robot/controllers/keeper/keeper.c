#include <webots/robot.h> 
#include <webots/receiver.h> 
#include <stdio.h> 
#include <math.h>
#include <webots/utils/motion.h>

 #define TIME_STEP 64

WbDeviceTag receiver;
double ballx, bally, ball_angle, ball_speed;

//Written by Sercan Bayindir
static void refresh_ball_data(){
  while (wb_receiver_get_queue_length(receiver) > 0) {
      const double * data = wb_receiver_get_data(receiver);
      wb_receiver_next_packet(receiver);

      ballx = data[0];
      bally = data[1];
      ball_angle = data[2];
      ball_speed = data[3];

  }
}

//Written by Ozan Ozyegen
static void next_motion(WbMotionRef nextMotion) {
  int time_step = wb_robot_get_basic_time_step();
  wbu_motion_play(nextMotion);
  // wait for termination of motion
  while (!wbu_motion_is_over(nextMotion))
    wb_robot_step(time_step);
}

//Created by Faruk Yazici

int main(int argc, char * * argv) {
  wb_robot_init();

  receiver = wb_robot_get_device("receiver");
  wb_receiver_enable(receiver, TIME_STEP);

  double guessed = 999;
  double guessed_time = 999;
  bool fixed = false;
  double last_angle = 999;

  
  while (wb_robot_step(TIME_STEP) != -1) {

    refresh_ball_data();
    printf("\nBall position (x,y): (%.2f,%.2f)\n", ballx, bally);
    printf("Ball movement direction: %.2f\n", ball_angle);
    printf("Ball speed: %.2f\n", ball_speed);
    //Initial faulty values are ignored
    if (ball_angle == last_angle && ball_speed > 0 && ball_speed < 5) fixed = true;
    last_angle = ball_angle;

    if (guessed == 999 && fixed) {
      guessed = bally - ballx * tan(ball_angle);
      guessed_time = fabs(ballx / cos(ball_angle) / ball_speed);
    }

    if (fixed) {
      printf("Guessed pass y: %.2f\n", guessed);
      printf("Guessed pass time: %.2f secs\n", guessed_time);

      static WbMotionRef side_step_left, side_step_right,
      turn_left_40, turn_right_40, collapse, wave_hand;
      static WbMotionRef forwards20, forwards30, forwards40, forwards50;

      side_step_left = wbu_motion_new("../../motions/SideStepLeft.motion");
      side_step_right = wbu_motion_new("../../motions/SideStepRight.motion");
      turn_left_40 = wbu_motion_new("../../motions/TurnLeft40.motion");
      turn_right_40 = wbu_motion_new("../../motions/TurnRight40.motion");
      collapse= wbu_motion_new("../../motions/Collapse.motion"); //Edited by Sema Karakas
      forwards20 = wbu_motion_new("../../motions/Forwards20.motion"); //Edited by Ozan Ozyegen
      forwards30 = wbu_motion_new("../../motions/Forwards30.motion"); //Edited by Ozan Ozyegen
      forwards40 = wbu_motion_new("../../motions/Forwards40.motion"); //Edited by Ozan Ozyegen
      forwards50 = wbu_motion_new("../../motions/Forwards50.motion"); //Edited by Ozan Ozyegen
      wave_hand = wbu_motion_new("../../motions/HandWave.motion");
      
      /*
       *  CustomRobotstadiumGoal is edited by Faruk Yazici
      */
      
      //Decision Algorithm and Implementations coworking by Faruk, Ozan and Sema
      
      
      //kidding me?
      if(fabs(guessed)<0.18){
        next_motion(wave_hand);
        next_motion(wave_hand);
        next_motion(wave_hand);
        goto end;
      }
      //should I walk sideways
      if (fabs(guessed) < 0.27) {
        printf("walking sideways\n");
        while (fabs(bally - ballx*tan(ball_angle)) > 0.18){
          printf("sides: %.2f\n", bally - ballx*tan(ba ll_angle));
          refresh_ball_data();
          next_motion(guessed > 0 ? side_step_left:side_step_right);
        }
        printf("finished movement\n");
        goto end;
      }
      //should I turn and walk
      else if (fabs(guessed) < 0.5) {
        printf("Turning and walking\n");
        next_motion(guessed > 0? turn_left_40:turn_right_40);
        next_motion(guessed > 0? turn_left_40:turn_right_40);
        
        if(fabs(guessed) < 0.27){
          printf("2 steps\n");
          next_motion(forwards20);
        }
        else if(fabs(guessed) < 0.34){
          printf("3 steps\n");
          next_motion(forwards30);
        }
        else if(fabs(guessed) < 0.37){
          printf("4 steps\n");
          next_motion(forwards40);
        }
        else if(fabs(guessed) < 0.5){
          printf("5 steps\n");
          next_motion(forwards50);
        }
        
        next_motion(guessed < 0? turn_left_40:turn_right_40);
        next_motion(guessed < 0? turn_left_40:turn_right_40);
        
        goto end;
      }
      //should I walk and collapse
      else {
        printf("Turning and walking and COLLAPSE\n");
        next_motion(guessed > 0? turn_left_40:turn_right_40);
        next_motion(guessed > 0? turn_left_40:turn_right_40);
        
        next_motion(forwards50);
        next_motion(collapse);
        goto end;

      }
    }

  };
  
  end:
  wb_robot_cleanup();
  return 0;
}

