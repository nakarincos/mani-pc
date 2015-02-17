/*
 * forward_kinematic.hpp
 *
 *  Created on: Feb 14, 2015
 *      Author: Nakarin
 */

#ifndef FORWARD_KINEMATIC_HPP_
#define FORWARD_KINEMATIC_HPP_


void input_alpha();
void input_a();
void input_d();
void input_theta(int input, int i);
void reference_frame_calculator(int index);
void cross_matrix(float a[][4], float b[][4]);
void goal_calculator(int from, int end);
float getX();
float getY();



#endif /* FORWARD_KINEMATIC_HPP_ */
