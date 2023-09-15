#ifndef COLORBUILDER_H
#define COLORBUILDER_H

#include "base/anim/interpolated.h"
#include "base/gfx/color.h"
#include "base/gfx/colorgradient.h"
#include "base/gfx/colorpalette.h"
#include "base/math/range.h"
#include "chart/generator/colorbase.h"

namespace Vizzu::Draw
{

struct ColorBuilder
{
	using LighnessRange = Math::Range<double>;

	LighnessRange lighnessRange{0.4, -0.4};
	const Gfx::ColorGradient *gradient{nullptr};
	const ::Anim::Interpolated<Gfx::ColorPalette> *palette{nullptr};

	ColorBuilder() = default;

	ColorBuilder(const LighnessRange &lighnessRange,
	    const ::Anim::Interpolated<Gfx::ColorPalette> &palette,
	    const Gfx::ColorGradient &gradient);

	[[nodiscard]] Gfx::Color render(
	    const ::Anim::Interpolated<Gen::ColorBase> &colorBase) const;

	[[nodiscard]] Gfx::Color render(
	    const Gen::ColorBase &colorBase) const;

private:
	[[nodiscard]] Gfx::Color baseColor(
	    const Gen::ColorBase &colorBase) const;

	[[nodiscard]] Gfx::Color indexedColor(
	    const uint32_t &colorIndex) const;

	[[nodiscard]] Gfx::Color lightnessAdjusted(
	    const Gfx::Color &color,
	    double lightness) const;
};

}

#endif
