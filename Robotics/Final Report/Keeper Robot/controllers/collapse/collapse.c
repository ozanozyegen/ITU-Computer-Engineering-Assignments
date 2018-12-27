/*
 * Description:  Collapse simulation
 * Sema Karaka�
 */

#include <webots/robot.h>
#include <webots/utils/motion.h>
#include <stdio.h>
#include <string.h>
static void next_motion(WbMotionRef nextMotion)
{
  int time_step = wb_robot_get_basic_time_step();
  wbu_motion_play(nextMotion);
    // wait for termination of motion
    while (! wbu_motion_is_over(nextMotion))
      wb_robot_step(time_step);
}

static void collapse(char *direction)
{
  char *left="left";
  char *right="right";
  char *front="front";
  static WbMotionRef collapse_front;
  collapse_front= wbu_motion_new("../../motions/Collapse.motion");

  if(strcmp(direction,left) == 0)
  {
     // load motion
    static WbMotionRef turn_left_40;
    turn_left_40= wbu_motion_new("../../motions/TurnLeft40.motion");
    
    next_motion(turn_left_40);
    next_motion(turn_left_40);
    wb_robot_step(100);
    next_motion(collapse_front);
  }
  else if(strcmp(direction,right) == 0)
  {
     // load motion
    static WbMotionRef turn_right_40;
    turn_right_40= wbu_motion_new("../../motions/TurnRight40.motion");
    
    next_motion(turn_right_40);
    next_motion(turn_right_40);
    wb_robot_step(100);
    next_motion(collapse_front);
  }
  else if(strcmp(direction,front) == 0)
  {  
    next_motion(collapse_front);
  }
}

int main(int argc, char *argv[]) {

  wb_robot_init();

  printf("Collapse controller of the Nao robot started...\n");
  
  //collapse("left");
  collapse("right");
  //collapse("front");
    

  wb_robot_cleanup();

  return 0;
}
