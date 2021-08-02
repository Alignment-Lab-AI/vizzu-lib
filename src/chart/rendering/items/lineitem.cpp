#include "lineitem.h"

using namespace Vizzu;
using namespace Vizzu::Draw;

LineItem::LineItem(const Diag::Marker &marker,
    const Diag::Options &options,
    const Styles::Chart &style,
    const CoordinateSystem &coordSys,
    const Diag::Diagram::Markers &markers,
    size_t lineIndex) :
    ConnectingDrawItem(marker,
        options,
        markers,
        lineIndex,
        Diag::ShapeType::Line)
{
	minWidth = *style.data.lineMinWidth;
	width = *style.data.lineWidth;
	maxWidth = *style.data.lineMaxWidth;

	linear = true;
	center = Math::interpolate(
		marker.position.yComp(),
	    marker.position.xComp(),
	    (double)options.horizontal.get());

	auto spacing = marker.spacing * marker.size / 2;
	auto pos = marker.position - spacing;

	if ((double)labelEnabled > 0.0)
	{
		lineWidth[1] = getWidth(marker.sizeFactor);

		points[2] = pos;

		points[1] = pos - ((double)options.horizontal.get() > 0.5
			? marker.size.yComp() : marker.size.xComp());

		const auto *prev = getPrev(marker, markers, lineIndex);
		if (prev)
		{
			auto prevSpacing = prev->spacing * prev->size / 2;
			auto prevPos = prev->position - prevSpacing;

			lineWidth[0] = getWidth(prev->sizeFactor);

			points[3] = prevPos;

			points[0] = prevPos - ((double)options.horizontal.get() > 0.5
				? prev->size.yComp() : prev->size.xComp());

			center = pos;
		}
		else
		{
			center = points[3] = pos;
			points[0] = points[1];
			lineWidth[0] = lineWidth[1];
		}
	}
	else
	{
		center = Geom::Point(pos.x, 0);
	}
	dataRect.pos = points[2];
	dataRect.size = Geom::Size();
	radius = lineWidth[1] * coordSys.getRect().size.minSize();

}

bool LineItem::bounds(const Geom::Point &)
{
	if ((double)enabled == 0) return false;
	return false; // todo: missing
//	return VerticalTrapezoid<Linear>(getLine()).contains(p);
}

double LineItem::getWidth(double sizeFactor) const
{
	return std::min(std::max(minWidth, width * sizeFactor), maxWidth);
}
