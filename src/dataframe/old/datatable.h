//
// Created by bela on 2024.02.29..
//

#ifndef DATAFRAME_OLD2_DATATABLE_H
#define DATAFRAME_OLD2_DATATABLE_H

#include <base/type/uniquelist.h>

#include "../impl/dataframe.h"

namespace Vizzu::Data
{

struct RowWrapper
{
	[[nodiscard]] dataframe::cell_value get_value(
	    const std::string_view &i) const
	{
		return parent->get_data(recordId, i);
	}

	const dataframe::dataframe_interface *parent;
	std::size_t recordId;
};

class data_table
{
public:
	using Type = dataframe::series_type;
	[[nodiscard]] std::string_view getUnit(
	    std::string_view const &colIx) const;

	void addColumn(std::string_view name,
	    std::string_view unit,
	    const std::span<const double> &values);

	void addColumn(std::string_view name,
	    const std::span<const char *const> &categories,
	    const std::span<const std::uint32_t> &values);

	void pushRow(const std::span<const char *> &cells);

	[[nodiscard]] std::string getInfos() const;

	[[nodiscard]] const dataframe::dataframe &getDf() const
	{
		return df;
	}

private:
	dataframe::dataframe df;
};

class series_index_t
{
	std::string orig_name;
	std::string_view sid;
	std::optional<dataframe::aggregator_type> aggr;

public:
	series_index_t(std::string const &str, const data_table &table);

	[[nodiscard]] const dataframe::aggregator_type &getAggr() const
	{
		return *aggr;
	}

	[[nodiscard]] const std::string_view &getColIndex() const;

	bool operator==(const series_index_t &rhs) const;
	bool operator<(const series_index_t &rhs) const;

	[[nodiscard]] bool isDimension() const;

	[[nodiscard]] const std::string &toString() const
	{
		return orig_name;
	}
};

using series_index_list_t = Type::UniqueList<series_index_t>;

struct slice_index_t
{
	std::string_view column;
	std::string_view value;

	[[nodiscard]] bool operator<(const slice_index_t &) const;

	[[nodiscard]] bool operator==(
	    const slice_index_t &) const = default;
};

using subslice_index_t = std::vector<slice_index_t>;

struct CellInfo
{
	std::vector<std::pair<std::string, std::string>> categories;
	std::vector<std::pair<std::string, double>> values;
};

class data_cube_t
{
	struct iterator_t;

public:
	struct multi_index_t
	{
		std::optional<std::size_t> rid;
		std::vector<std::size_t> old;

		[[nodiscard]] bool isEmpty() const;
	};
	using MultiIndex = multi_index_t;

	struct Id
	{
		using SubSliceIndex = subslice_index_t;
		using SliceIndex = std::ranges::range_value_t<SubSliceIndex>;

		SubSliceIndex itemSliceIndex;
		std::size_t seriesId{};
		std::size_t itemId{};

		[[nodiscard]] bool operator==(const Id &) const;
	};

	std::shared_ptr<dataframe::dataframe_interface> df;
	std::shared_ptr<dataframe::dataframe_interface> removed;
	std::map<std::pair<std::string_view, dataframe::aggregator_type>,
	    std::string>
	    measure_names;
	std::vector<std::pair<std::string_view, std::size_t>> dim_reindex;

	std::shared_ptr<std::map<std::set<std::string>,
	    std::shared_ptr<dataframe::dataframe_interface>>>
	    cacheImpl = std::make_shared<
	        typename decltype(cacheImpl)::element_type>();

	template <class Options, class Filter>
	data_cube_t(const data_table &table,
	    const Options &options,
	    const Filter &filter);

	[[nodiscard]] size_t combinedSizeOf(
	    const series_index_list_t &colIndices) const;

	[[nodiscard]] bool empty() const;

	[[nodiscard]] CellInfo cellInfo(const multi_index_t &index) const;

	[[nodiscard]] double aggregateAt(const multi_index_t &multiIndex,
	    const series_index_list_t &sumCols,
	    const series_index_t &seriesId) const;

	[[nodiscard]] double valueAt(const multi_index_t &multiIndex,
	    const series_index_t &seriesId) const;

	[[nodiscard]] Id getId(const series_index_list_t &,
	    const multi_index_t &) const;

	[[nodiscard]] static std::string getValue(
	    const slice_index_t &index);

	[[nodiscard]] const std::string &getName(
	    const series_index_t &seriesId) const;

	[[nodiscard]] std::string_view getUnit(
	    std::string_view const &colIx) const;

	[[nodiscard]] iterator_t begin() const;
	[[nodiscard]] static iterator_t end();

private:
	struct iterator_t
	{
		const data_cube_t *parent{};
		std::size_t rid{};
		multi_index_t index;

		[[nodiscard]] bool operator!=(const iterator_t &oth) const;

		void operator++();

		[[nodiscard]] const multi_index_t &operator*() const;
	};

	void check(iterator_t &it) const;
	void incr(iterator_t &it) const;
};

}

#endif // DATAFRAME_OLD2_DATATABLE_H
