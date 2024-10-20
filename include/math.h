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

#endif
