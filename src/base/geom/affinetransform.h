#ifndef GEOM_AFFINETRANSFORM
#define GEOM_AFFINETRANSFORM

#include <array>

#include "point.h"

namespace Geom
{

struct Size;
struct Rect;
class Circle;
struct Line;
class Polygon;

class AffineTransform
{
public:
	typedef std::array<double, 3> Row;
	typedef std::array<Row, 2> Matrix;

	AffineTransform();
	AffineTransform(const Matrix &m) : m(m){};
	AffineTransform(double m00,
	    double m01,
	    double m02,
	    double m10,
	    double m11,
	    double m12);
	AffineTransform(Geom::Point offset,
	    double scale = 1.0,
	    double angle = 0.0);

	const Matrix &getMatrix() const { return m; }

	AffineTransform inverse() const;
	bool transforms() const;
	void shift(const Geom::Point &offset);

	AffineTransform operator*(const AffineTransform &other) const;
	bool operator==(const AffineTransform &other) const = default;

	Geom::Point operator()(const Geom::Point &original) const;
	Geom::Line operator()(const Geom::Line &original) const;
	Geom::Polygon operator()(const Geom::Polygon &original) const;

private:
	Matrix m;
};

}

#endif
