//
//  ColorUtils.hpp
//  RGB-ColorDifference
//
//  Created by Mohamed Shahawy on 4/22/16.
//  Copyright © 2016 Mohamed Shahawy. All rights reserved.
//

#ifndef ColorUtils_hpp
#define ColorUtils_hpp

#include <cstdint>
#include <QRgb>

namespace ColorUtils {

struct CIELABColorSpace {
	double l, a, b;

	CIELABColorSpace() {}
	CIELABColorSpace(double l, double a, double b)
		: l(l), a(a), b(b) {}
};

struct xyzColor {
	double x, y, z;

	xyzColor() {}
	xyzColor(double x, double y, double z)
		: x(x), y(y), z(z) {}
};

struct rgbColor {
	uint16_t r, g, b;
	double rF, gF, bF;

	rgbColor() {}
	rgbColor(int r, int g, int b)
		: r(r), g(g), b(b) {
		rF = r / 255.0;
		gF = g / 255.0;
		bF = b / 255.0;
	}
	rgbColor(double rF, double gF, double bF)
		: rF(rF), gF(gF), bF(bF) {
		r = rF * 255.0;
		g = gF * 255.0;
		b = bF * 255.0;
	}
	rgbColor(unsigned int r, unsigned int g, unsigned int b)
		: rgbColor((int)r, (int)g, (int)b) {}
	rgbColor(float r, float g, float b)
	: rgbColor((double)r, (double)g, (double)b) {}
	
	rgbColor(const QRgb &rgb)
	: rgbColor(qRed(rgb), qGreen(rgb), qBlue(rgb)) {}
};

double getColorDeltaE(rgbColor rgb1, rgbColor rgb2);
xyzColor rgbToXyz(rgbColor rgb);
CIELABColorSpace xyzToCIELAB(xyzColor xyz);

} // ColorUtils

#endif // ColorUtils_hpp
