#ifndef SERIESTYPE_H
#define SERIESTYPE_H

#include "data/table/datatable.h"

#include "aggregator.h"

namespace Vizzu
{
namespace Data
{

class SeriesType
{
	typedef ColumnInfo::Type CT;
	typedef Aggregator::Type AT;

public:

	static const SeriesType Index;
	static const SeriesType Discrete;
	static const SeriesType Exists;
	static const SeriesType Sum;
	static const SeriesType Count;
	static const SeriesType Distinct;
	static const SeriesType Min;
	static const SeriesType Max;
	static const SeriesType Mean;

	bool isDiscrete() const {
		return columnType == CT::Discrete;
	}

	bool isContinous() const {
		return columnType == CT::Continous;
	}

	bool isReal() const {
		return real;
	}

	bool isValid() const {
		return name != nullptr;
	}

	ColumnInfo::Type getColumnType() {
		return columnType;
	}

	Aggregator::Type aggregatorType() const {
		return (Aggregator::Type)index;
	}

	SeriesType() : name(nullptr) {}

	SeriesType(bool real, 
		ColumnInfo::Type columnType, 
		ColumnInfo::Type nestedColumnType, 
		size_t index, 
		const char * name)
		: real(real), columnType(columnType), 
		  nestedColumnType(nestedColumnType), 
		  index(index), name(name)
	{}

	bool operator==(const SeriesType &other) const {
		return index == other.index && columnType == other.columnType;
	}

	bool operator<(const SeriesType &other) const {
		return index < other.index
			|| (index == other.index && columnType < other.columnType);
	}

	std::string toString() const { return name; }
	static SeriesType fromString(const std::string &name, bool throws = true);

	void deduceName();

	bool isNestedDiscrete() const {
		return nestedColumnType == CT::Discrete;
	}

private:
	static const std::vector<SeriesType> constTypes;

	bool real; // needs data series
	ColumnInfo::Type columnType;
	ColumnInfo::Type nestedColumnType;
	uint64_t index;
	const char *name;
};

}
}

#endif
