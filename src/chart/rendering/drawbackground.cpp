#include "drawbackground.h"

#include "chart/main/events.h"

namespace Vizzu::Draw
{

void DrawBackground::draw(Gfx::ICanvas &canvas,
    const Geom::Rect &rect,
    const Styles::Box &style,
    Util::EventDispatcher::Event &onDraw,
    std::unique_ptr<Util::EventTarget> &&eventTarget) const
{
	if (Events::OnRectDrawEvent eventObj(*eventTarget, {rect, false});
	    !style.borderColor->isTransparent()
	    || !style.backgroundColor->isTransparent()) {
		canvas.setBrushColor(*style.backgroundColor);
		canvas.setLineColor(*style.borderColor);
		canvas.setLineWidth(*style.borderWidth);
		if (onDraw.invoke(std::move(eventObj))) {
			canvas.rectangle(rect);
			renderedChart.emplace(
			    Geom::TransformedRect::fromRect(rect),
			    std::move(eventTarget));
		}
		canvas.setLineWidth(0);
	}
	else if (onDraw.invoke(std::move(eventObj))) {
		renderedChart.emplace(Geom::TransformedRect::fromRect(rect),
		    std::move(eventTarget));
	}
}

}