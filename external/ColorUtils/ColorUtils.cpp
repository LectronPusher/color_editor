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

// given two colors, returns the distance between them in the CIELAB color space
// the distance ranges from 0 to 100
double getColorDeltaE(rgbColor rgb1, rgbColor rgb2) {
	CIELABColorSpace lab1 = xyzToCIELAB(rgbToXyz(rgb1));
	CIELABColorSpace lab2 = xyzToCIELAB(rgbToXyz(rgb2));

	double dl = lab2.l - lab1.l;
	double da = lab2.a - lab1.a;
	double db = lab2.b - lab1.b;

	return sqrtf(dl * dl + da * da + db * db);
}

static double gammaInverse(double u) {
	if (u > 0.04045)
		return powf(((u + 0.055) / 1.055), 2.4);
	else
		return u / 12.92;
}

// converts from rgb to xyz using the standard sRGB specification
// https://en.wikipedia.org/wiki/SRGB#The_reverse_transformation
xyzColor rgbToXyz(rgbColor rgb) {
	double r = gammaInverse(rgb.r / 255.0);
	double g = gammaInverse(rgb.g / 255.0);
	double b = gammaInverse(rgb.b / 255.0);

	// Calibrated for D65 illumination with observer at 2°
	double x = 100 * (r * 0.4124 + g * 0.3576 + b * 0.1805);
	double y = 100 * (r * 0.2126 + g * 0.7152 + b * 0.0722);
	double z = 100 * (r * 0.0193 + g * 0.1192 + b * 0.9505);

	return xyzColor(x, y, z);
}

// https://en.wikipedia.org/wiki/CIELAB_color_space#Forward_transformation
static double f(double t) {
	if (t > 0.008856)
		return powf(t, 1.0 / 3.0);
	else
		return (7.787 * t) + (16.0 / 116.0);
}

// converts from xyz to CIELAB using the CIE76 specification
CIELABColorSpace xyzToCIELAB(xyzColor xyz) {
	// Calibrated for D65 illumination with observer at 2°
	double x = f(xyz.x / 95.047);
	double y = f(xyz.y / 100.0);
	double z = f(xyz.z / 108.883);

	double l = (116 * y) - 16;
	double a = 500 * (x - y);
	double b = 200 * (y - z);

	return CIELABColorSpace(l, a, b);
}

} // ColorUtils
