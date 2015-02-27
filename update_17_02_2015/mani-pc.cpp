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
#include "inverse_kinematic.hpp"
using namespace std;


int main() {

	float targetX, targetY;

	// ##############################################
	// ##        Initial Inverse Kinematic         ##
	// ##############################################
	while(1){
		cout << "X: ";
		cin >> targetX;
		cout << "Y: ";
		cin >> targetY;
		setTarget(targetX, targetY);
		findAngle();
	}
	return 0;
}
