//
// Created by Vlad on 29.11.2022.
//

#define _USE_MATH_DEFINES

#include "trigonometry.h"
#include <math.h>



float math::deg_to_rad(float angle)
{
	return (angle / 180.f) * (float)M_PI;
}
float math::rad_to_deg(float angle)
{
	return angle / (float)M_PI * 180.f;
}

