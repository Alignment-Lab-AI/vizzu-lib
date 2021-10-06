#include "drawdiagram.h"

#include "base/text/smartstring.h"
#include "chart/rendering/drawbackground.h"
#include "chart/rendering/drawaxes.h"

#include "drawitem.h"

using namespace Geom;
using namespace Vizzu;
using namespace Vizzu::Base;
using namespace Vizzu::Draw;
using namespace Vizzu::Diag;

drawDiagram::drawDiagram(const Geom::Rect &rect,
    const Diag::Diagram &diagram,
    Gfx::ICanvas &canvas,
    const DrawOptions &drawOptions,
    const Styles::Chart &style,
	const Events::Draw &events) :
	DrawingContext(rect, diagram, canvas, drawOptions, style, events)
{
	guides.init(diagram.axises, options);

	if (!drawOptions.onlyEssentials())
	{
		drawBackground(rect, canvas, style.plot, events.plot.background);

		drawAxes(*this, guides).drawBase();
	}

	auto clip = style.plot.overflow == Styles::Overflow::hidden;

	if (clip)
	{
		canvas.save();
		
		std::array<Geom::Point, 4> points =
		{
			Geom::Point(0.0, 0.0), 
			Geom::Point(0.0, 1.0), 
			Geom::Point(1.0, 1.0), 
			Geom::Point(1.0, 0.0) 
		};
		painter.drawPolygon(points, true);
	}

	if (!drawOptions.onlyEssentials())
	{
		drawMarkerGuides();
	}

	drawMarkers();

	if (clip) canvas.restore();

	if (!drawOptions.onlyEssentials())
	{
		drawMarkerLabels();

		drawAxes(*this, guides).drawLabels();
	}
}

void drawDiagram::drawMarkerGuides()
{
	auto &style = diagram.getStyle().plot.marker.guides;

	if (!style.color->isTransparent()
		&& *style.lineWidth > 0
		&& (double)diagram.anyAxisSet > 0
		&& drawItem::mayDrawLines(guides))
	{
		canvas.setLineWidth(*style.lineWidth);

		auto origo = diagram.axises.origo();

		for (const auto &marker : diagram.getMarkers())
			drawItem(marker, *this).drawLines(guides, style, origo);

		canvas.setLineWidth(0);
	}
}

void drawDiagram::drawMarkers()
{
	for (const auto &marker : diagram.getMarkers())
		drawItem(marker, *this).draw();
}

void drawDiagram::drawMarkerLabels()
{
	for (const auto &marker : diagram.getMarkers())
		drawItem(marker, *this).drawLabel();
}
