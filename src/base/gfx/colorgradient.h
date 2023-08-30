#ifndef GFX_COLORGRADIENT
#define GFX_COLORGRADIENT

#include <vector>

#include "base/math/segmentedfunc.h"

#include "color.h"

namespace Gfx
{

struct ColorGradient : Math::SegmentedFunction<Gfx::Color>
{
	static ColorGradient HeatMap5Color();
	static ColorGradient HeatMap7Color();

	using Math::SegmentedFunction<Gfx::Color>::SegmentedFunction;

	explicit ColorGradient(
	    Math::SegmentedFunction<Gfx::Color> gradient) :
	    Math::SegmentedFunction<Gfx::Color>(std::move(gradient))
	{}

	explicit ColorGradient(
	    const std::string &stoplist = std::string());

	explicit operator std::string() const;

	using Math::SegmentedFunction<Gfx::Color>::at;
};

}

#endif
