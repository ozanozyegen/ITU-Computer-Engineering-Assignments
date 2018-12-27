#include <stdio.h>
#include <webots/robot.h>
#include <webots/supervisor.h>
#include <string.h>
#include <webots/emitter.h>
#include <math.h>

#define GOAL_WIDTH 160
#define SHOOT_WIDTH 1000
#define GOAL_DISTANCE 10
#define BALL_HEIGHT 0.03
#define RAMP_SPACE 2.6


//Created by Furkan Aksin


int main(int argc, char *argv[]) {
  srand (time(NULL));
  
  
  wb_robot_init();
  printf("Initialized.\n");
  
  WbNodeRef ball, ramp;
  WbFieldRef ball_translation,ramp_translation,ramp_rotation;
  double t[3],k[3];
  int j;
  double kale[3], midkale;
  kale[0] = GOAL_DISTANCE;
  kale[1] = 0.11;
  kale[2]= 0;
  
  double newrampangle[4];

  
  
  ball = wb_supervisor_node_get_from_def("BALL");
  if(ball == NULL){
    printf("top yok\n");
    goto son;
  }
  printf("Got ball.\n");
  
  ramp = wb_supervisor_node_get_from_def("ramp");
  if(ramp == NULL){
    printf("ramp yok\n");
    goto son;
  }
  printf("Got ramp.\n");
  
  
  midkale = kale[2];
  kale[2] += (double)(rand()%GOAL_WIDTH)/100 - (double)GOAL_WIDTH/200;
  //kale[2] = 0.3;
  k[0] = kale[0] - GOAL_DISTANCE;
  k[2] = kale[2] - (double)(rand()%SHOOT_WIDTH)/100 + SHOOT_WIDTH/200;
  ramp_translation = wb_supervisor_node_get_field(ramp,"translation");
  if(ramp_translation == NULL){
    printf("ramp_translation yok\n");
    goto son;
  }
  printf("Got ramp_translation.\n");
  wb_supervisor_field_set_sf_vec3f(ramp_translation,k);
  
  
  double angle = atan2(k[2] - kale[2], k[0] - kale[0]);
  printf("Angle : %f\n", angle);
  newrampangle[0] = 0;
  newrampangle[1] = 1;
  newrampangle[2] = 0;
  newrampangle[3] = -angle;
  ramp_rotation = wb_supervisor_node_get_field(ramp,"rotation");
  if(ramp_rotation == NULL){
    printf("ramp_rotation yok\n");
    goto son;
  }
  printf("Got ramp_rotation.\n");
  wb_supervisor_field_set_sf_rotation(ramp_rotation,newrampangle);
  printf("Shoot at: %f\n", (kale[2] - midkale)/(GOAL_WIDTH/200));
  
  
  t[1] = BALL_HEIGHT;
  t[0] = k[0] + fabs(RAMP_SPACE*cos(angle));
  t[2] = k[2] - RAMP_SPACE*sin(angle);
  ball_translation = wb_supervisor_node_get_field(ball,"translation");
  if(ball_translation == NULL){
    printf("ball_translation yok\n");
    goto son;
  }
  printf("Got ball_translation.\n");
  wb_supervisor_field_set_sf_vec3f(ball_translation,t);
  printf("Ball position: %.2f %.2f %.2f\n",t[0],t[1],t[2]);
  printf("Ramp position: %.2f %.2f %.2f\n\n\n",k[0],k[1],k[2]);
  
  son:
  wb_robot_cleanup();
  return 0;
}
