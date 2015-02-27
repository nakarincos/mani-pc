//============================================================================
// Name        : mani-pc.cpp
// Author      : Nakarin
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cmath>
#include <cstdio>
#include <dynamixel.h>
#include "inverse_kinematic.cpp"
#include "tree_point.cpp"
using namespace std;
// Control table address
#define P_GOAL_POSITION_L	30
#define P_GOAL_POSITION_H	31
#define P_SPEED_L	32
#define P_PRESENT_POSITION_L	36
#define P_PRESENT_POSITION_H	37
#define P_MOVING		46

// Defulat setting
#define DEFAULT_BAUDNUM		1 // 1Mbps
#define DEFAULT_ID		0
float a(float input);
float b(float input);
void slowdown(int id, int pos);
void check_moving(int id, int pos);
void resetId2();
void resetId3();
void resetId4();

int main() {

	float targetX, targetY;
	float angle;
	int baudnum = 1;
	int deviceIndex = 0;
	// ##############################################
	// ##        Initial Inverse Kinematic         ##
	// ##############################################
	if( dxl_initialize(deviceIndex, baudnum) == 0 )
	{
		printf( "Failed to open USB2Dynamixel!\n" );
		printf( "Press Enter key to terminate...\n" );
		getchar();
		return 0;
	}
	else
		printf( "Succeed to open USB2Dynamixel!\n" );
	while(1){
		cout << "ANGLE: ";
		cin >> angle;
		if(angle == 0){
			dxl_terminate();
			printf( "Press Enter key to terminate...\n" );
			break;
		}
		tree_point_calculation(angle);
		
		targetX = getTreex_far();
		targetY = getTreey_far();
		setTarget(targetX, targetY);
		findAngle();
		float id1 = getAnswerID1();
		float id2 = getAnswerID2();
		// float id3 = getAnswerID3();

		/////////////////////////
		resetId4();
		resetId3();
		resetId2();
		float tmp = a(id1);
		cout << id1 << " " << tmp << endl;
		float tmp2 = b(id2);
		cout << id2 << " " << tmp2 << endl;
		slowdown(1, tmp);
		slowdown(2, tmp2);
		////////////////////////
	}
	return 0;
}

float a(float input){
	if(input >= 0 && input <= 60){
		return (60-input) *1024 / 300;
	}
	else{
		return ((input*-1)+60) * 1024 / 300;
	}
}
float b(float input){
	if(input >= 0){
		return (150+(180-input)) * 1024 / 300;
	}
	else{
		return ((input*-1)-30) * 1024 / 300;
	}
}
void resetId2(){
	slowdown(2, 512);
}
void resetId3(){
	slowdown(2, 512);
}
void resetId4(){
	slowdown(4, 512);
}
void slowdown(int id, int pos){
	int curr = dxl_read_word( id, P_PRESENT_POSITION_L );
	if(abs(curr-pos) > 100){
		if(curr > pos){
			dxl_write_word( id, P_SPEED_L, 10);
			dxl_write_word( id, P_GOAL_POSITION_L, curr-50);
			check_moving(id, curr-50);
			dxl_write_word( id, P_SPEED_L, 25);
			dxl_write_word( id, P_GOAL_POSITION_L, pos+50);
			check_moving(id, pos+50);
			dxl_write_word( id, P_SPEED_L, 10);
			dxl_write_word( id, P_GOAL_POSITION_L, pos);
			check_moving(id, pos);
		}
		else{
			dxl_write_word( id, P_SPEED_L, 10);
			dxl_write_word( id, P_GOAL_POSITION_L, curr+50);
			check_moving(id, curr+50);
			dxl_write_word( id, P_SPEED_L, 25);
			dxl_write_word( id, P_GOAL_POSITION_L, pos-50);
			check_moving(id, pos-50);
			dxl_write_word( id, P_SPEED_L, 10);
			dxl_write_word( id, P_GOAL_POSITION_L, pos);
			check_moving(id, pos);
		}
	}
	else{
		dxl_write_word( id, P_SPEED_L, 25);
		dxl_write_word( id, P_GOAL_POSITION_L, pos);
		check_moving(id, pos);
	}
	
}

void check_moving(int id, int pos){
	int curr_pos[10] = {dxl_read_word( id, P_PRESENT_POSITION_L )};
	int carry = 0;
	int carry_tmp = 0;
	while(1){
		int j;
		int curr = dxl_read_word( id, P_PRESENT_POSITION_L );
		int count = 0;
		for(j = 0 ; j < 10 ; j++){
			// printf("curr_pos %d : %d\n", j, curr_pos[j]);
			if(curr_pos[j] == curr)
				count++;
		}
		if(count == 10){
			if(abs(curr-pos) <= 3)
				break;
			// int tmp = pos - curr;
			// // cout << "tmp : " << tmp << endl;
			// dxl_write_word( id, P_GOAL_POSITION_L, curr+tmp);
			// carry_tmp++;
			// if(carry_tmp >= 3){
			// 	carry_tmp = 0;
			// 	carry++;
			// 	dxl_write_word( id, P_GOAL_POSITION_L, curr+tmp+carry);
			// }
		}
		else{
			for(j = 0 ; j < 10 ; j++){
				curr_pos[j] = dxl_read_word( id, P_PRESENT_POSITION_L );
			}
		}
		// printf(">> %d\n", curr);
		if(curr == pos)
			break;
	}
}
