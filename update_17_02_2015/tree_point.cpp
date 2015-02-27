#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
#define PI (3.14159265358979323846)
float INPUT_KINEMATIC_TABLE_TREE[4][4];
float REF_KINEMATIC_TABLE_TREE[4][4][4];
float GOAL_KINEMATIC_TABLE_TREE[4][4];


void reference_frame_calculator_tree(int index){
	float tmp;
	REF_KINEMATIC_TABLE_TREE[index][0][0] = round(cos(INPUT_KINEMATIC_TABLE_TREE[index][3]*PI/180)*10000) / 10000;
	tmp = sin(INPUT_KINEMATIC_TABLE_TREE[index][3]*PI/180);
	REF_KINEMATIC_TABLE_TREE[index][0][1] = (tmp == 0 ? 0 : round((-1)*tmp*10000) / 10000);
	//REF_KINEMATIC_TABLE_TREE[index][0][1] = round((-1)*sin(INPUT_KINEMATIC_TABLE_TREE[index][3]*PI/180)*10000) / 10000;
	REF_KINEMATIC_TABLE_TREE[index][0][2] = 0;
	REF_KINEMATIC_TABLE_TREE[index][0][3] = round(INPUT_KINEMATIC_TABLE_TREE[index][1]*10000) / 10000;

	REF_KINEMATIC_TABLE_TREE[index][1][0] = round(sin(INPUT_KINEMATIC_TABLE_TREE[index][3]*PI/180) * cos(INPUT_KINEMATIC_TABLE_TREE[index][0]*PI/180)*10000) / 10000;
	REF_KINEMATIC_TABLE_TREE[index][1][1] = round(cos(INPUT_KINEMATIC_TABLE_TREE[index][3]*PI/180) * cos(INPUT_KINEMATIC_TABLE_TREE[index][0]*PI/180)*10000) / 10000;
	tmp = sin(INPUT_KINEMATIC_TABLE_TREE[index][0]*PI/180);
	REF_KINEMATIC_TABLE_TREE[index][1][2] = (tmp == 0 ? 0 : round((-1)*tmp*10000) / 10000);
	// REF_KINEMATIC_TABLE_TREE[index][1][2] = round((-1)*sin(INPUT_KINEMATIC_TABLE_TREE[index][0]*PI/180)*10000) / 10000;
	tmp = sin(INPUT_KINEMATIC_TABLE_TREE[index][0]*PI/180)*INPUT_KINEMATIC_TABLE_TREE[index][2];
	REF_KINEMATIC_TABLE_TREE[index][1][3] = (tmp == 0 ? 0 : round((-1)*tmp*10000) / 10000);
	// REF_KINEMATIC_TABLE_TREE[index][1][3] = round((-1)*(sin(INPUT_KINEMATIC_TABLE_TREE[index][0]*PI/180)*INPUT_KINEMATIC_TABLE_TREE[index][2]) *10000) / 10000;

	REF_KINEMATIC_TABLE_TREE[index][2][0] = round(sin(INPUT_KINEMATIC_TABLE_TREE[index][3]*PI/180) * sin(INPUT_KINEMATIC_TABLE_TREE[index][0]*PI/180)*10000) / 10000;
	REF_KINEMATIC_TABLE_TREE[index][2][1] = round(cos(INPUT_KINEMATIC_TABLE_TREE[index][3]*PI/180) * sin(INPUT_KINEMATIC_TABLE_TREE[index][0]*PI/180)*10000) / 10000;
	REF_KINEMATIC_TABLE_TREE[index][2][2] = round(cos(INPUT_KINEMATIC_TABLE_TREE[index][0]*PI/180)*10000) / 10000;
	REF_KINEMATIC_TABLE_TREE[index][2][3] = round((cos(INPUT_KINEMATIC_TABLE_TREE[index][0]*PI/180)*INPUT_KINEMATIC_TABLE_TREE[index][2])*10000) / 10000;

	REF_KINEMATIC_TABLE_TREE[index][3][0] = 0;
	REF_KINEMATIC_TABLE_TREE[index][3][1] = 0;
	REF_KINEMATIC_TABLE_TREE[index][3][2] = 0;
	REF_KINEMATIC_TABLE_TREE[index][3][3] = 1;


}
void cross_matrix_tree(float a[][4], float b[][4]){
	float tmp_array[4][4];
	float sum = 0;
	for(int count = 0 ; count < 4 ; count++){
		
		for(int i = 0 ; i < 4 ; i++){
			for(int j = 0 ; j < 4 ; j++){
				sum += a[count][j] * b[j][i];
			}
			tmp_array[count][i] = sum;
			sum = 0;
		}
	}
	for(int i = 0 ; i < 4 ; i++){
		for(int j = 0 ; j < 4 ; j++){
			GOAL_KINEMATIC_TABLE_TREE[i][j] = tmp_array[i][j];
		}
	}
}

void goal_calculator_tree(int from, int end){
	from--;	
	end--;
	for(int i = 0 ; i < 4 ; i++){
		for(int j = 0 ; j < 4 ; j++){
			GOAL_KINEMATIC_TABLE_TREE[i][j] = REF_KINEMATIC_TABLE_TREE[from][i][j];
		}
	}
	if(from == end)
		return;
	for(int i = from+1 ; i <= end ; i++){
		cross_matrix_tree(GOAL_KINEMATIC_TABLE_TREE, REF_KINEMATIC_TABLE_TREE[i]);
	}

}
float x_far, y_far, x_near, y_near;

void tree_point_calculation(float input){
	for(int i = 0 ; i < 4 ; i++){
		INPUT_KINEMATIC_TABLE_TREE[i][0] = 0;
		INPUT_KINEMATIC_TABLE_TREE[i][2] = 0;
	}
	INPUT_KINEMATIC_TABLE_TREE[0][1] = 0;
	INPUT_KINEMATIC_TABLE_TREE[1][1] = 50+(40/PI);
	INPUT_KINEMATIC_TABLE_TREE[2][1] = 40/PI;
	INPUT_KINEMATIC_TABLE_TREE[3][1] = 50;
	// initial theta
	INPUT_KINEMATIC_TABLE_TREE[0][3] = 90;
	INPUT_KINEMATIC_TABLE_TREE[1][3] = input;
	INPUT_KINEMATIC_TABLE_TREE[2][3] = 0;
	INPUT_KINEMATIC_TABLE_TREE[3][3] = 0;
	for(int i = 0 ; i < 4 ; i++){
		reference_frame_calculator_tree(i);
	}
	goal_calculator_tree(1,4);
	x_far = GOAL_KINEMATIC_TABLE_TREE[0][3];
	y_far = GOAL_KINEMATIC_TABLE_TREE[1][3];
  	goal_calculator_tree(1,3);
  	x_near = GOAL_KINEMATIC_TABLE_TREE[0][3];
	y_near = GOAL_KINEMATIC_TABLE_TREE[1][3];
}
float getTreex_far(){
	return x_far;
}
float getTreey_far(){
	return y_far;
}
float getTreex_near(){
	return x_near;
}
float getTreey_near(){
	return y_near;
}