
#include "renard.h"

#include <cmath>
#include <ranges>
#include <stdexcept>

#include "floating.h"
#include "normalizednumber.h"

using namespace Math;

Renard Renard::R3()
{
	static const std::vector<double> R3Numbers = {1.0,
	    2.0,
	    5.0,
	    10.0};
	return Renard{R3Numbers};
}
Renard Renard::R5()
{
	static const std::vector<double> R5Numbers =
	    {1.0, 1.5, 2.5, 4.0, 6.0, 10.0};
	return Renard{R5Numbers};
}

double Renard::ceil(double value)
{
	if (value == 0.0) return 0.0;

	const ScientificNumber num(value);

	for (auto Rnum : numbers)
		if (num.coefficient <= Rnum)
			return ScientificNumber(num.positive, Rnum, num.exponent)
			    .value();

	throw std::logic_error("Internal error in R-number rounding.");
}

double Renard::floor(double value)
{
	if (value == 0.0) return 0.0;

	const ScientificNumber num(value);

	for (auto Rnum : std::ranges::reverse_view(numbers))
		if (num.coefficient >= Rnum)
			return ScientificNumber(num.positive, Rnum, num.exponent)
			    .value();

	throw std::logic_error("Internal error in R-number rounding.");
}
