/*
 * Description:  Default controller of the Nao robot
 */

#include <webots/robot.h>
#include <webots/utils/motion.h>
#include <stdio.h>
static void next_motion(WbMotionRef nextMotion)
{
  int time_step = wb_robot_get_basic_time_step();
  wbu_motion_play(nextMotion);
    // wait for termination of motion
    while (! wbu_motion_is_over(nextMotion))
      wb_robot_step(time_step);
}
static void wait_n_seconds(int n)
{
  int time_step = wb_robot_get_basic_time_step();
  int pause;
  for (pause = n*1000; pause > 0; pause -= time_step)
    wb_robot_step(time_step);
}
int main(int argc, char *argv[]) {

  wb_robot_init();

  int time_step = wb_robot_get_basic_time_step();

  printf("Default (hello!) controller of the Nao robot started...\n");
  
  // load motion
  static WbMotionRef hand_wave, forwards, backwards, side_step_left, side_step_right, turn_left_60, turn_right_60,turn_left_40,turn_right_40,turn_left_180;
  hand_wave = wbu_motion_new("../../motions/HandWave.motion");
  forwards = wbu_motion_new("../../motions/Forwards50.motion");
  backwards = wbu_motion_new("../../motions/Backwards.motion");
  side_step_left = wbu_motion_new("../../motions/SideStepLeft.motion");
  side_step_right = wbu_motion_new("../../motions/SideStepRight.motion");
  turn_left_60 = wbu_motion_new("../../motions/TurnLeft60.motion");
  turn_left_40 = wbu_motion_new("../../motions/TurnLeft40.motion");
  turn_left_180 = wbu_motion_new("../../motions/TurnLeft180.motion");
  turn_right_40 = wbu_motion_new("../../motions/TurnRight40.motion");
  turn_right_60 = wbu_motion_new("../../motions/TurnRight60.motion");
  
  
  while (1) {
  
    //next_motion(hand_wave);
    //next_motion(turn_left_180);
    next_motion(turn_left_40);
    //next_motion(turn_left_40);
    //next_motion(forwards);
    //next_motion(backwards);
    
    
  }

  wb_robot_cleanup();

  return 0;
}
