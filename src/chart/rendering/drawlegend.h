#ifndef CHART_RENDERING_DRAWLEGEND_H
#define CHART_RENDERING_DRAWLEGEND_H

#include "chart/rendering/colorbuilder.h"
#include "chart/rendering/drawingcontext.h"

namespace Vizzu::Draw
{

class DrawLegend : public DrawingContext
{
public:
	void draw(Gfx::ICanvas &canvas,
	    const Geom::Rect &legendLayout,
	    Gen::ChannelId channelType,
	    double weight) const;

	const Events::DrawEvents::Legend &events = rootEvents.draw.legend;
	const Styles::Legend &style = rootStyle.legend;

	const ColorBuilder colorBuilder = {
	    rootStyle.plot.marker.lightnessRange(),
	    *rootStyle.plot.marker.colorPalette,
	    *rootStyle.plot.marker.colorGradient};

private:
	struct Info
	{
		Gfx::ICanvas &canvas;
		Geom::Rect contentRect;
		Gen::ChannelId type;
		double weight;
		double itemHeight;
		double titleHeight;
		double markerSize;
		const Gen::MeasureAxis &measure;
		const Gen::DimensionAxis &dimension;
		double measureEnabled = measure.enabled.calculate<double>();
		bool dimensionEnabled = dimension.enabled;
		double measureWeight = weight * measureEnabled;
	};

	void drawTitle(const Info &info) const;

	void drawDimension(const Info &info) const;
	void drawMeasure(const Info &info) const;

	void drawMarker(const Info &info,
	    const Gfx::Color &color,
	    const Geom::Rect &rect) const;
	[[nodiscard]] Geom::Rect getItemRect(const Info &info,
	    double index) const;
	[[nodiscard]] Geom::Rect getMarkerRect(const Info &info,
	    const Geom::Rect &itemRect) const;
	[[nodiscard]] Geom::TransformedRect getLabelRect(const Info &info,
	    const Geom::Rect &itemRect) const;
	[[nodiscard]] Geom::Rect getBarRect(const Info &info) const;

	void extremaLabel(const Info &info, double value, int pos) const;
	void colorBar(const Info &info, const Geom::Rect &rect) const;
	void lightnessBar(const Info &info, const Geom::Rect &rect) const;
	void sizeBar(const Info &info, const Geom::Rect &rect) const;
};

}

#endif
