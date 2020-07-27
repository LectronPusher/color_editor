//
//  ColorUtils.hpp
//  RGB-ColorDifference
//
//  Created by Mohamed Shahawy on 4/22/16.
//  Copyright © 2016 Mohamed Shahawy. All rights reserved.
//

#ifndef ColorUtils_hpp
#define ColorUtils_hpp

#include <QRgb>

namespace ColorUtils {

struct CIELABColor {
	double _l, _a, _b;
	
	CIELABColor() {}
	CIELABColor(double l, double a, double b)
	: _l(l), _a(a), _b(b) {}
};

struct xyzColor {
	double _x, _y, _z;
	
	xyzColor() {}
	xyzColor(double x, double y, double z)
	: _x(x), _y(y), _z(z) {}
	
	CIELABColor toCIELAB();
};

struct rgbColor {
	uint16_t _r, _g, _b;
	double _rF, _gF, _bF;
	
	rgbColor() {}
	rgbColor(int r, int g, int b)
	: _r(r), _g(g), _b(b) {
		_rF = _r / 255.0;
		_gF = _g / 255.0;
		_bF = _b / 255.0;
	}
	rgbColor(double r, double g, double b)
	: _rF(r), _gF(g), _bF(b) {
		_r = _rF * 255.0;
		_g = _gF * 255.0;
		_b = _bF * 255.0;
	}
	rgbColor(QRgb rgb)
	: rgbColor(qRed(rgb), qGreen(rgb), qBlue(rgb)) {
	}
	
	xyzColor toXyz();
};

double getColorDeltaE(rgbColor rgb1, rgbColor rgb2);

}; // ColorUtils

#endif // ColorUtils_hpp
