#ifndef TREEMAP_H
#define TREEMAP_H

#include <cstddef>
#include <map>
#include <vector>

#include "base/geom/rect.h"

#include "specmarker.h"

namespace Vizzu::Charts
{

using Hierarchy = std::vector<std::vector<uint64_t>>;

class TreeMap
{
public:
	explicit TreeMap(const std::vector<double> &sizes,
	    const Geom::Point &p0 = Geom::Point{0, 1},
	    const Geom::Point &p1 = Geom::Point{1, 0});

	template <typename Item>
	static void setupVector(std::vector<Item> &items,
	    const Hierarchy &hierarchy);

private:
	using It = std::vector<SpecMarker>::iterator;

	std::vector<SpecMarker> markers;

	void divide(It begin,
	    It end,
	    const Geom::Point &p0,
	    const Geom::Point &p1,
	    bool horizontal = true);
};

template <typename Item>
void TreeMap::setupVector(std::vector<Item> &items,
    const Hierarchy &hierarchy)
{
	if (items.empty()) return;

	std::vector<double> sizes(hierarchy.size());
	for (std::size_t ix{}; const auto &level : hierarchy) {
		auto sum = 0.0;
		for (const auto &item : level)
			if (items[item].sizeFactor > 0)
				sum += items[item].sizeFactor;
		sizes[ix++] = sum;
	}
	TreeMap chart(sizes);

	size_t cnt = 0;
	for (const auto &level : hierarchy) {
		auto &c = chart.markers[cnt];

		std::vector<double> sizes(level.size());
		for (std::size_t ix{}; const auto &item : level)
			sizes[ix++] = items[item].sizeFactor;

		TreeMap subChart(sizes,
		    c.rect().pos,
		    c.rect().pos + c.rect().size);

		for (size_t subCnt{}; const auto &item : level) {
			auto rect = subChart.markers[subCnt++].rect().positive();
			items[item].position = rect.topRight();
			items[item].size = rect.size;
		}

		++cnt;
	}
}

}

#endif
