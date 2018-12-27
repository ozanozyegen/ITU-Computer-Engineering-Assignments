#include <stdio.h>
#include <webots/robot.h>
#include <webots/supervisor.h>
#include <math.h>
#define STEP 100


//Created by Sercan Bayindir


int main(int argc, char *argv[]){
	WbNodeRef robotNode, ballNode;
	WbFieldRef field;
	
	double *robotPosition, *ballPosition;
	double *robotRotation, angle;
	double oldballx, oldbally, ballx, bally, robotx, roboty;
	double translatedx, translatedy, rotatedx, rotatedy;
	double movAngle, movSpeed;
	double message[4];
	bool hasOld = false;
	
	wb_robot_init();
	WbDeviceTag emitter = wb_robot_get_device("emitter");
	
	
	robotNode = wb_supervisor_node_get_from_def("ROBOT");
	ballNode = wb_supervisor_node_get_from_def("BALL");
	
	printf("Ball locator started...\n");
	
	do{
            	field = wb_supervisor_node_get_field(robotNode, "translation");
		robotPosition = wb_supervisor_field_get_sf_vec3f(field);
		field = wb_supervisor_node_get_field(ballNode, "translation");
		ballPosition = wb_supervisor_field_get_sf_vec3f(field);
		robotRotation = wb_supervisor_node_get_orientation(robotNode);
		angle = atan2(robotRotation[0], robotRotation[6]);
		
		ballx = ballPosition[2];
		bally = ballPosition[0];
		
		robotx = robotPosition[2];
		roboty = robotPosition[0];
		//relative pos
		translatedx = ballx - robotx;
		translatedy = bally - roboty;
		rotatedx = cos(-angle)*translatedx - sin(-angle)*translatedy;
		rotatedy = sin(-angle)*translatedx + cos(-angle)*translatedy;
		//movement angle
		if(hasOld){
            		movAngle = atan2(bally - oldbally, ballx - oldballx);
            		movAngle -= angle;
		}
		
		
		
		movSpeed = fabs(sqrt(pow(ballx - oldballx,2) + pow(bally - oldbally, 2)))*10;
		oldballx = ballx;
		oldbally = bally;
		hasOld = true;
		
		/*
		printf("Robot at (%.2f,%.2f)\n", robotx, roboty);
		printf("Ball at (%.2f,%.2f)\n", ballx, bally);
		printf("Robot rotation: (%.2f)\n", angle );
		printf("Relative ball pos: (%.2f,%.2f)\n ", rotatedx, rotatedy);
		if(hasOld){
              		printf("Sent moveSpeed: %f\n", movSpeed);
                		printf("Relative ball movement angle: (%.2f)\n\n ", movAngle);
		}*/
		
		message[0] = rotatedx;
		message[1] = rotatedy;
		message[2] = movAngle;
		message[3] = movSpeed;
		if(hasOld)wb_emitter_send(emitter, message, 4*sizeof(double));
		
	} while(wb_robot_step(STEP)!=-1);
	wb_robot_cleanup();
	return 0;
}