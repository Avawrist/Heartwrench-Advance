#ifndef MATH_H
#define MATH_H

// Butano
#include "bn_fixed.h"
#include "bn_fixed_point.h"

// My Libs
#include "utility.h"

inline bn::fixed lerp(bn::fixed a, bn::fixed b, bn::fixed t)
{
	return a + (t * (b - a));
}

inline bn::fixed clamp(bn::fixed min, bn::fixed max, bn::fixed target)
{
	if(target < min) {return min;}
	if(target > max) {return max;}
	return target;
}

inline int32 clamp(int32 min, int32 max, int32 target)
{
	if(target < min) {return min;}
	if(target > max) {return max;}
	return target;
}

inline bn::fixed min(bn::fixed a, bn::fixed b)
{
	if(b < a) {return b;}

	return a;
}

inline bn::fixed max(bn::fixed a, bn::fixed b)
{
	if(b > a) {return b;}

	return a;
}

#endif
