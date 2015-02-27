#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>

#include "forward_kinematic.cpp"
using namespace std;
#define PI (3.14159265358979323846)
typedef struct Range{
	float a,b;
};
float goalX, goalY;
float answer_Angle[3] = {0};
Range limit_angle[3];

void setTarget(float x, float y){
	goalX = x;
	goalY = y;
	if(goalX >= 0){
		limit_angle[0].a = -90;
		limit_angle[0].b = 30;
		limit_angle[1].a = 30;
		limit_angle[1].b = 180;
		limit_angle[2].a = 0;
		limit_angle[2].b = 150;
	}
	else{
		limit_angle[0].a = -210;
		limit_angle[0].b = -90;
		limit_angle[1].a = -180;
		limit_angle[1].b = -30;
		limit_angle[2].a = -150;
		limit_angle[2].b = 0;
	}
}
void findAngle(){
	int i = limit_angle[0].a;
    int j = limit_angle[1].a;
    int k = limit_angle[2].a;
	int counteri = 10;
	int counterj = 10;
	int is_ok = 0;
	input_alpha();
	input_a();
	input_d();
	float x,y;
	float diff = 100.0, tmp;
	Range answer;

	cout << "=====================================\n";
	cout << "             Point Moving            \n";
	cout << "=====================================\n";
	///////////////////////////////////////////////////////////////
	while(i <= limit_angle[0].b){
		j = limit_angle[1].a;
		while(j <= limit_angle[1].b){
				input_theta(i, 0);
				input_theta(j, 1);
				input_theta(0, 2);
				for(int k = 0 ; k < 3 ; k++){
					reference_frame_calculator(k);
				}
				goal_calculator(1,3);
				x = getX();
				y = getY();
				if(x <= goalX+3 && x >= goalX-3){
					if(y <= goalY+3 && y >= goalY-3){
						tmp = abs(x-goalX) + abs(y-goalY);
						if(diff > tmp){
							diff = tmp;
							answer.a = x;
							answer.b = y;
							answer_Angle[0] = i;
							answer_Angle[1] = j;
						}
					}
				}

			if(x <= goalX+5 && x >= goalX-5){
				counterj = 1;
			}
			else{
				counterj = 10;
			}
			j += counterj;
		}
		if((x <= goalX+5 && x >= goalX-5) || i >= limit_angle[0].b - 10){
			counteri = 1;
			is_ok = 1;
		}
		else{
			if(!is_ok)
				counteri = 10;
			else
				counteri = limit_angle[0].b;
		}
		i += counteri;
	}

	cout << "=====   ANSWER   =====\n";
	cout << "DIFF: " << diff <<"\n";
	cout << "X: " << answer.a << " , Y: " << answer.b << "\n";
	cout << "ID1: " << answer_Angle[0] << " , ID2: " << answer_Angle[1] << "\n";
	// while(k <= limit_angle[2].b){
		
	// 	k++;
	// }

}
float getAnswerID1(){
	return answer_Angle[0];
}
float getAnswerID2(){
	return answer_Angle[1];
}
float getAnswerID3(){
	return answer_Angle[2];
}
