//
//  ColorUtils.cpp
//  RGB-ColorDifference
//
//  Created by Mohamed Shahawy on 4/22/16.
//  Copyright © 2016 Mohamed Shahawy. All rights reserved.
//

#include "ColorUtils.hpp"
#include <cmath>

namespace ColorUtils {

double getColorDeltaE(rgbColor rgb1, rgbColor rgb2) {
	CIELABColor lab1 = rgb1.toXyz().toCIELAB();
	CIELABColor lab2 = rgb2.toXyz().toCIELAB();
	
	double dl = lab1._l - lab2._l;
	double da = lab1._a - lab2._a;
	double db = lab1._b - lab2._b;
	
	return sqrtf(dl * dl + da * da + db * db);
}

// https://en.wikipedia.org/wiki/SRGB#The_reverse_transformation
static double gammaInverse(double u) {
	if (u > 0.04045)
		return powf(((u + 0.055) / 1.055), 2.4);
	else
		return u / 12.92;
}

xyzColor rgbColor::toXyz() {
	double r = gammaInverse(_rF);
	double g = gammaInverse(_gF);
	double b = gammaInverse(_bF);
	
	// Calibrated for D65 illumination
	double x = 100 * (r * 0.4124 + g * 0.3576 + b * 0.1805);
	double y = 100 * (r * 0.2126 + g * 0.7152 + b * 0.0722);
	double z = 100 * (r * 0.0193 + g * 0.1192 + b * 0.9505);
	
	return {x, y, z};
}

// https://en.wikipedia.org/wiki/CIELAB_color_space#Forward_transformation
static double f(double t) {
	if (t > 0.008856)
		return powf(t, 1.0 / 3.0);
	else
		return (7.787 * t) + (16.0 / 116.0);
}

CIELABColor xyzColor::toCIELAB() {
	// Calibrated for D65 illumination
	double x = f(_x / 95.047);
	double y = f(_y / 100.0);
	double z = f(_z / 108.883);
	
	double l = (116 * y) - 16;
	double a = 500 * (x - y);
	double b = 200 * (y - z);
	
	return {l, a, b};
}

}; // ColorUtils
