#ifndef BUBBLECHART_H
#define BUBBLECHART_H

#include <vector>

#include "base/geom/circle.h"
#include "base/geom/point.h"

#include "specmarker.h"

namespace Vizzu::Charts
{

class BubbleChart
{
public:
	using Markers = std::vector<SpecMarker>;

	Markers markers;

	explicit BubbleChart(const std::vector<double> &circleAreas,
	    const SpecMarker *parent = nullptr);

private:
	void generate();

	void normalize(const Geom::Rect &rect);

	[[nodiscard]] std::optional<Geom::Circle> getTouchingCircle(
	    const SpecMarker &act,
	    size_t firstIdx,
	    size_t lastIdx) const;
};

}

#endif
