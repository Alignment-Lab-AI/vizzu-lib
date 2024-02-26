#include "dataframe.h"

#include <cmath>
#include <numeric>
#include <optional>
#include <ranges>
#include <utility>

#include "aggregators.h"

namespace Vizzu::dataframe
{
using Refl::unsafe_get;

std::shared_ptr<dataframe_interface>
dataframe::copy(bool remove_filtered, bool inherit_sorting) const &
{
	if (source == source_type::copying) {
		const auto &cp = unsafe_get<source_type::copying>(source);
		return std::make_shared<dataframe>(cp.other,
		    remove_filtered ? std::get_if<std::vector<bool>>(&filter)
		    : cp.pre_remove ? &*cp.pre_remove
		                    : nullptr,
		    inherit_sorting && cp.sorted_indices ? &*cp.sorted_indices
		                                         : nullptr);
	}

	return std::make_shared<dataframe>(
	    unsafe_get<source_type::owning>(source),
	    remove_filtered ? std::get_if<std::vector<bool>>(&filter)
	                    : nullptr,
	    nullptr);
}

dataframe::dataframe(std::shared_ptr<const data_source> other,
    std::vector<bool> const *filtered,
    std::vector<std::size_t> const *sorted) :
    source(std::in_place_index<1>, std::move(other))
{
	auto &cp = unsafe_get<source_type::copying>(source);
	if (filtered) cp.pre_remove.emplace(*filtered);
	if (sorted) cp.sorted_indices.emplace(*sorted);
	if (!cp.other->finalized) { migrate_data(); }
	else
		state_data.emplace<state_type::finalized>(
		    *cp.other->finalized);
}

void valid_dimension_aggregator(
    const dataframe::any_aggregator_type &agg)
{
	using enum aggregator_type;
	if (std::holds_alternative<aggregator_type>(agg)) {
		switch (*std::get_if<aggregator_type>(&agg)) {
		case sum:
		case min:
		case max:
		case mean:
			throw std::runtime_error(
			    "Dimension series cannot be aggregated by sum, min, "
			    "max or mean.");
		default: return;
		}
	}
	if (!std::holds_alternative<std::monostate>(agg))
		throw std::runtime_error("Dimension series cannot be "
		                         "aggregated by custom aggregator.");
}

void valid_measure_aggregator(
    const dataframe::any_aggregator_type &agg)
{
	if (std::holds_alternative<std::monostate>(agg))
		throw std::runtime_error(
		    "Measure series must be aggregated.");

	if (!std::holds_alternative<aggregator_type>(agg)) {
		if (Refl::enum_names<aggregator_type>.end()
		    != std::ranges::find(Refl::enum_names<aggregator_type>,
		        std::get_if<custom_aggregator>(&agg)->get_name())) {
			throw std::runtime_error(
			    "Custom aggregator name cannot be any of these: "
			    + std::string{
			        Refl::enum_name_holder<aggregator_type>.data(),
			        Refl::enum_name_holder<aggregator_type>.size()});
		}
	}
}

std::string dataframe::set_aggregate(series_identifier series,
    const any_aggregator_type &aggregator) &
{
	change_state_to(state_type::aggregating,
	    state_modification_reason::needs_series_type);

	auto &&ser = get_data_source().get_series(series);
	switch (ser) {
		using enum series_type;
	default: throw std::runtime_error("Series does not exists.");
	case dimension: valid_dimension_aggregator(aggregator); break;
	case measure: valid_measure_aggregator(aggregator); break;
	}

	change_state_to(state_type::aggregating,
	    state_modification_reason::needs_own_state);
	auto &aggs = unsafe_get<state_type::aggregating>(state_data);

	const auto *agg = std::get_if<aggregator_type>(&aggregator);
	if (ser == series_type::dimension && !agg) {
		aggs.dims.emplace(unsafe_get<series_type::dimension>(ser));
		return {};
	}

	return aggs.add_aggregated(std::move(ser),
	    agg ? aggregators[*agg]
	        : std::get<custom_aggregator>(aggregator));
}

void dataframe::set_filter(std::function<bool(record_type)> &&filt) &
{
	if (auto *bools = std::get_if<std::vector<bool>>(&filter)) {
		bools->assign(get_record_count(), false);
		if (filt) {
			visit(
			    [&filt, it = bools->begin()](
			        record_type record) mutable
			    {
				    *it++ = filt(record);
			    },
			    false);
		}
	}
	else
		filter = std::move(filt);
}

void dataframe::set_sort(series_identifier series,
    any_sort_type sort,
    na_position na_pos) &
{
	change_state_to(state_type::sorting,
	    state_modification_reason::needs_series_type);

	const sort_type *sort_ptr = std::get_if<sort_type>(&sort);
	auto ser = get_data_source().get_series(series);

	switch (ser) {
		using enum series_type;
	default: throw std::runtime_error("Series does not exists.");
	case dimension: {
		std::optional<std::vector<std::uint32_t>> indices;
		if (const auto &dim = unsafe_get<dimension>(ser).second;
		    std::ranges::is_sorted(
		        indices.emplace(dim.get_indices(sort)))
		    && (na_pos == dim.na_pos
		        || std::ranges::find(dim.values, data_source::nav)
		               == dim.values.end()))
			break;

		if (source == source_type::copying) migrate_data();

		auto &s = *unsafe_get<source_type::owning>(source);
		const auto &[name, dim] =
		    unsafe_get<dimension>(s.get_series(series));
		dim.sort_by(std::move(*indices), na_pos);

		sort_ptr = nullptr;
		ser.emplace<dimension>(name, dim);
		break;
	}
	case measure:
		if (!sort_ptr)
			throw std::runtime_error(
			    "Measure series cannot be sorted by categories.");
		switch (*sort_ptr) {
		case sort_type::less:
		case sort_type::greater: break;
		case sort_type::natural_less:
		case sort_type::natural_greater:
			throw std::runtime_error(
			    "Measure series cannot be sorted by natural order.");
		}
		break;
	}

	change_state_to(state_type::sorting,
	    state_modification_reason::needs_own_state);

	std::get_if<data_source::sorting_type>(&state_data)
	    ->emplace_back(std::in_place_index<0>,
	        ser,
	        sort_ptr ? *sort_ptr : sort_type::less,
	        na_pos);
}

void dataframe::set_custom_sort(
    std::function<std::weak_ordering(record_type, record_type)>
        custom_sort) &
{
	if (!custom_sort) return;

	change_state_to(state_type::sorting,
	    state_modification_reason::needs_own_state);

	std::get_if<data_source::sorting_type>(&state_data)
	    ->emplace_back(std::move(custom_sort));
}

void dataframe::add_dimension(
    std::span<const char *const> dimension_categories,
    std::span<const std::uint32_t> dimension_values,
    const char *name,
    adding_type adding_strategy,
    std::span<const std::pair<const char *, const char *>> info) &
{
	if (!name)
		throw std::runtime_error("Series name cannot be null.");

	change_state_to(state_type::modifying,
	    state_modification_reason::needs_series_type);

	switch (get_data_source().get_series(name)) {
	default: {
		if (adding_strategy == adding_type::override_full
		    || adding_strategy
		           == adding_type::override_all_with_rotation)
			throw std::runtime_error("override unknown dimension");

		change_state_to(state_type::modifying,
		    state_modification_reason::needs_own_state);

		unsafe_get<source_type::owning>(source)->add_new_dimension(
		    dimension_categories,
		    dimension_values,
		    name,
		    info);
		break;
	}
	case series_type::dimension: {
		if (adding_strategy == adding_type::create_or_throw)
			throw std::runtime_error("create existing dimension");

		change_state_to(state_type::modifying,
		    state_modification_reason::needs_own_state);

		auto &dims = unsafe_get<series_type::dimension>(
		    unsafe_get<source_type::owning>(source)->get_series(name))
		                 .second;

		switch (adding_strategy) {
		case adding_type::create_or_throw:
		case adding_type::create_or_add: {
			dims.add_more_data(dimension_categories,
			    dimension_values);
			break;
		}
		case adding_type::override_full: {
			dims.categories.assign(dimension_categories.begin(),
			    dimension_categories.end());
			dims.values.assign(dimension_values.begin(),
			    dimension_values.end());
			dims.info = {info.begin(), info.end()};
			break;
		}
		case adding_type::override_all_with_rotation: {
			dims.categories.assign(dimension_categories.begin(),
			    dimension_categories.end());
			for (auto i = std::size_t{}; i < dims.values.size(); ++i)
				dims.values[i] =
				    dimension_values[i % dimension_values.size()];
			dims.info = {info.begin(), info.end()};
			break;
		}
		}
		break;
	}
	case series_type::measure:
		throw std::runtime_error("add dimension -> measure");
	}
}

void dataframe::add_measure(std::span<const double> measure_values,
    const char *name,
    adding_type adding_strategy,
    std::span<const std::pair<const char *, const char *>> info) &
{
	if (!name)
		throw std::runtime_error("Series name cannot be null.");

	change_state_to(state_type::modifying,
	    state_modification_reason::needs_series_type);

	switch (get_data_source().get_series(name)) {
	default: {
		if (adding_strategy == adding_type::override_full
		    || adding_strategy
		           == adding_type::override_all_with_rotation)
			throw std::runtime_error("override unknown measure");

		change_state_to(state_type::modifying,
		    state_modification_reason::needs_own_state);

		unsafe_get<source_type::owning>(source)->add_new_measure(
		    measure_values,
		    name,
		    info);
		break;
	}
	case series_type::measure: {
		if (adding_strategy == adding_type::create_or_throw)
			throw std::runtime_error("create existing measure");

		change_state_to(state_type::modifying,
		    state_modification_reason::needs_own_state);

		auto &meas = unsafe_get<series_type::measure>(
		    unsafe_get<source_type::owning>(source)->get_series(name))
		                 .second;

		switch (adding_strategy) {
		case adding_type::create_or_throw:
		case adding_type::create_or_add: {
			meas.values.insert(meas.values.end(),
			    measure_values.begin(),
			    measure_values.end());
			break;
		}
		case adding_type::override_full: {
			meas.values.assign(measure_values.begin(),
			    measure_values.end());
			meas.info = {info.begin(), info.end()};
			break;
		}
		case adding_type::override_all_with_rotation: {
			for (auto i = std::size_t{}; i < meas.values.size(); ++i)
				meas.values[i] =
				    measure_values[i % measure_values.size()];
			meas.info = {info.begin(), info.end()};
			break;
		}
		}
		break;
	}
	case series_type::dimension:
		throw std::runtime_error("add measure -> dimension");
	}
}

void dataframe::add_series_by_other(series_identifier curr_series,
    const char *name,
    std::function<cell_value(record_type, cell_value)>
        value_transform,
    std::span<const std::pair<const char *, const char *>> info) &
{
	if (!name)
		throw std::runtime_error("Series name cannot be null.");

	change_state_to(state_type::modifying,
	    state_modification_reason::needs_series_type);

	const auto &pre = get_data_source();
	auto &&type = pre.get_series(curr_series);
	if (type == series_type::unknown)
		throw std::runtime_error("Reference series does not exists.");
	if (pre.get_series(name) != series_type::unknown)
		throw std::runtime_error("Series name already exists.");

	change_state_to(state_type::modifying,
	    state_modification_reason::needs_record_count);

	if (get_record_count() == 0)
		throw std::runtime_error("Dataframe contains no records.");

	Refl::EnumVariant<series_type,
	    std::monostate,
	    data_source::dimension_t,
	    data_source::measure_t>
	    v;

	auto add_val = [&v, &info](cell_value &&add_val)
	{
		using enum series_type;
		const auto *d = std::get_if<double>(&add_val);
		switch (v) {
		default:
			if (d)
				v.emplace<measure>(std::ranges::single_view(*d),
				    info);
			else
				v.emplace<dimension>(
				    std::ranges::single_view(
				        *std::get_if<std::string_view>(&add_val)),
				    std::ranges::single_view(0),
				    info);
			break;
		case measure:
			if (!d) throw std::runtime_error("Mixed return types.");
			unsafe_get<measure>(v).values.emplace_back(*d);
			break;
		case dimension:
			if (d) throw std::runtime_error("Mixed return types.");
			unsafe_get<dimension>(v).add_element(
			    *std::get_if<std::string_view>(&add_val));
			break;
		}
	};

	change_state_to(state_type::modifying,
	    state_modification_reason::needs_sorted_records);

	switch (type) {
		using enum series_type;
	default: break;
	case dimension: {
		visit(
		    [&value_transform,
		        &add_val,
		        &dim = unsafe_get<dimension>(type).second](
		        record_type record)
		    {
			    add_val(value_transform(record,
			        dim.get(std::get<std::size_t>(record.recordId))));
		    },
		    false);
		break;
	}
	case measure: {
		visit(
		    [&value_transform,
		        &add_val,
		        &mea = unsafe_get<measure>(type).second](
		        record_type record)
		    {
			    add_val(value_transform(record,
			        mea.values[std::get<std::size_t>(
			            record.recordId)]));
		    },
		    false);
		break;
	}
	}

	change_state_to(state_type::modifying,
	    state_modification_reason::needs_own_state);

	auto &s = *unsafe_get<source_type::owning>(source);

	switch (v) {
		using enum series_type;
	default: break;
	case measure:
		s.add_new_measure(std::move(unsafe_get<measure>(v)), name);
		break;
	case dimension:
		s.add_new_dimension(std::move(unsafe_get<dimension>(v)),
		    name);
		break;
	}
}

void dataframe::remove_series(
    std::span<const series_identifier> names) &
{
	if (names.empty()) return;

	change_state_to(state_type::modifying,
	    state_modification_reason::needs_series_type);

	std::vector<std::size_t> remove_dimensions;
	std::vector<std::size_t> remove_measures;

	for (auto &&s = get_data_source(); const auto &name : names) {
		switch (auto ser = s.get_series(name)) {
			using enum series_type;
		default: throw std::runtime_error("Series does not exists.");
		case dimension: {
			auto ix = &unsafe_get<dimension>(ser).second
			        - s.dimensions.data();
			remove_dimensions.insert(
			    std::ranges::lower_bound(remove_dimensions, ix),
			    ix);
			break;
		}
		case measure: {
			auto ix =
			    &unsafe_get<measure>(ser).second - s.measures.data();
			remove_measures.insert(
			    std::ranges::lower_bound(remove_measures, ix),
			    ix);
			break;
		}
		}
	}

	change_state_to(state_type::modifying,
	    state_modification_reason::needs_own_state);

	unsafe_get<source_type::owning>(source)->remove_series(
	    remove_dimensions,
	    remove_measures);
}

void dataframe::add_record(std::span<const cell_value> values) &
{
	if (values.empty())
		throw std::runtime_error("Empty record cannot be added.");

	std::size_t dimensionIx{};
	std::size_t measureIx{};

	for (const auto &v : values)
		if (std::holds_alternative<double>(v))
			++measureIx;
		else
			++dimensionIx;

	change_state_to(state_type::modifying,
	    state_modification_reason::needs_series_type);

	const auto &pre = get_data_source();
	if (measureIx != pre.measures.size())
		throw std::runtime_error("Measure count not match.");
	if (dimensionIx != pre.dimensions.size())
		throw std::runtime_error("Dimension count not match.");

	change_state_to(state_type::modifying,
	    state_modification_reason::needs_own_state);

	auto &s = *unsafe_get<source_type::owning>(source);
	s.normalize_sizes();

	measureIx = 0;
	dimensionIx = 0;
	for (const auto &v : values) {
		if (const double *measure = std::get_if<double>(&v))
			s.measures[measureIx++].values.emplace_back(*measure);
		else {
			s.dimensions[dimensionIx++].add_element(
			    *std::get_if<std::string_view>(&v));
		}
	}
}

void dataframe::remove_records(
    std::span<const record_identifier> record_ids) &
{

	change_state_to(state_type::modifying,
	    state_modification_reason::needs_sorted_records);

	std::vector<std::size_t> remove_ix;
	for (auto &&s = get_data_source(); auto id : record_ids) {
		if (std::holds_alternative<std::string_view>(id))
			s.change_record_identifier_type(id);

		remove_ix.emplace_back(*std::get_if<std::size_t>(&id));
	}
	std::sort(remove_ix.begin(), remove_ix.end());

	change_state_to(state_type::modifying,
	    state_modification_reason::needs_own_state);

	unsafe_get<source_type::owning>(source)->remove_records(
	    remove_ix);
}

void dataframe::remove_records(
    std::function<bool(record_type)> filter) &
{
	if (!filter) return;

	change_state_to(state_type::modifying,
	    state_modification_reason::needs_record_count);

	if (get_record_count() == 0) return;

	change_state_to(state_type::modifying,
	    state_modification_reason::needs_sorted_records);

	std::vector<std::size_t> remove_ix;
	visit(
	    [&remove_ix, &filter](const record_type &r)
	    {
		    if (filter(r))
			    remove_ix.push_back(
			        std::get<std::size_t>(r.recordId));
	    },
	    false);

	change_state_to(state_type::modifying,
	    state_modification_reason::needs_own_state);

	unsafe_get<source_type::owning>(source)->remove_records(
	    remove_ix);
}

void dataframe::change_data(record_identifier record_id,
    series_identifier column,
    cell_value value) &
{
	const double *d = std::get_if<double>(&value);

	change_state_to(state_type::modifying,
	    state_modification_reason::needs_sorted_records);

	const auto &s = get_data_source();

	if (std::holds_alternative<std::string_view>(record_id))
		s.change_record_identifier_type(record_id);

	if (s.get_record_count() <= *std::get_if<std::size_t>(&record_id))
		throw std::runtime_error("Record does not exists.");

	switch (s.get_series(column)) {
		using enum series_type;
	default: throw std::runtime_error("Series does not exists.");
	case measure:
		if (!d) throw std::runtime_error("Measure must be a number.");
		break;
	case dimension:
		if (d)
			throw std::runtime_error("Dimension must be a string.");
		break;
	}

	change_state_to(state_type::modifying,
	    state_modification_reason::needs_own_state);

	switch (auto &&ser =
	            unsafe_get<source_type::owning>(source)->get_series(
	                column)) {
		using enum series_type;
	default: break;
	case measure:
		unsafe_get<measure>(ser)
		    .second.values[*std::get_if<std::size_t>(&record_id)] =
		    *d;
		break;
	case dimension:
		unsafe_get<dimension>(ser).second.set(
		    *std::get_if<std::size_t>(&record_id),
		    *std::get_if<std::string_view>(&value));
		break;
	}
}

void dataframe::fill_na(series_identifier column, cell_value value) &
{
	const double *d = std::get_if<double>(&value);

	change_state_to(state_type::modifying,
	    state_modification_reason::needs_series_type);

	switch (get_data_source().get_series(column)) {
		using enum series_type;
	default: throw std::runtime_error("Series does not exists.");
	case measure:
		if (!d) throw std::runtime_error("Measure must be a number.");
		if (std::isnan(*d))
			throw std::runtime_error("Value must not be NaN.");
		break;
	case dimension:
		if (d)
			throw std::runtime_error("Dimension must be a string.");
		break;
	}

	change_state_to(state_type::modifying,
	    state_modification_reason::needs_own_state);

	switch (auto &&ser =
	            unsafe_get<source_type::owning>(source)->get_series(
	                column)) {
		using enum series_type;
	default: break;
	case measure:
		for (auto &v : unsafe_get<measure>(ser).second.values)
			if (std::isnan(v)) v = *d;
		break;
	case dimension:
		unsafe_get<dimension>(ser).second.set_nav(
		    *std::get_if<std::string_view>(&value));
		break;
	}
}

void dataframe::finalize() &
{
	change_state_to(state_type::finalized,
	    state_modification_reason::needs_own_state);
}

std::string dataframe::as_string() const & { return {}; }

std::span<const std::string> dataframe::get_dimensions() const &
{
	return get_data_source().dimension_names;
}

std::span<const std::string> dataframe::get_measures() const &
{
	return get_data_source().measure_names;
}

std::span<const std::string> dataframe::get_categories(
    series_identifier dimension) const &
{
	switch (auto &&ser = get_data_source().get_series(dimension)) {
		using enum series_type;
	default: throw std::runtime_error("Series does not exists.");
	case measure:
		throw std::runtime_error("Measure series has no categories.");
	case dimension:
		return unsafe_get<dimension>(ser).second.categories;
	}
}

std::pair<double, double> dataframe::get_min_max(
    series_identifier measure) const &
{
	auto &&s = get_data_source();
	switch (auto &&meas = s.get_series(measure)) {
		using enum series_type;
	default: throw std::runtime_error("Series does not exists.");
	case dimension:
		throw std::runtime_error("Dimension series has no min/max.");
	case measure:
		return s.get_min_max(unsafe_get<measure>(meas).second);
	}
}

cell_value dataframe::get_data(record_identifier record_id,
    series_identifier column) const &
{
	const auto &s = get_data_source();

	if (std::holds_alternative<std::string_view>(record_id))
		s.change_record_identifier_type(record_id);

	return s.get_data(*std::get_if<std::size_t>(&record_id), column);
}

void dataframe::visit(std::function<void(record_type)> function,
    bool filtered) const &
{
	if (const auto *fun = std::get_if<0>(&filter);
	    fun && filtered && *fun)
		throw std::runtime_error(
		    "Filtered dataframe is not finalized.");

	const auto *cp = get_if<source_type::copying>(&source);

	const auto *filt = filtered ? std::get_if<1>(&filter) : nullptr;
	if (!filt && cp && cp->pre_remove) filt = &*cp->pre_remove;
	visit(function,
	    cp && cp->sorted_indices ? &*cp->sorted_indices : nullptr,
	    filt);
}

void dataframe::visit(
    const std::function<void(record_type)> &function,
    const std::vector<std::size_t> *sort,
    const std::vector<bool> *filt) const
{
	for (std::size_t i{}, max = get_record_count(); i < max; ++i)
		if (auto ix = sort ? (*sort)[i] : i; !filt || !(*filt)[ix])
			function({this, ix});
}

void dataframe::migrate_data()
{
	auto *s = get_if<source_type::copying>(&source);
	switch (state_data) {
		using enum state_type;
	case aggregating: {
		source = std::make_shared<data_source>(
		    std::move(*std::get_if<data_source::aggregating_type>(
		        &state_data)),
		    s && s->pre_remove ? &*s->pre_remove : nullptr,
		    get_record_count());

		break;
	}
	case sorting: {
		if (s) {
			if (auto &&indices = s->other->get_sorted_indices(this,
			        std::exchange(unsafe_get<sorting>(state_data),
			            {}));
			    !std::ranges::is_sorted(indices))
				s->sorted_indices = std::move(indices);
			else
				s->sorted_indices.reset();
		}
		[[fallthrough]];
	}
	default:
	case modifying:
	case finalized: {
		if (!s) return;
		source = std::make_shared<data_source>(s->other,
		    std::exchange(s->pre_remove, {}),
		    std::exchange(s->sorted_indices, {}));
		break;
	}
	}
}

void dataframe::change_state_to(state_type new_state,
    state_modification_reason reason)
{
	using reason_t = state_modification_reason;
	if (state_data == new_state) return;

	switch (state_data) {
		using enum state_type;
	case modifying:
		if (reason == reason_t::needs_series_type
		    || reason == reason_t::needs_sorted_records)
			return;
		if (auto *owned = get_if<source_type::owning>(&source))
			(*owned)->normalize_sizes();
		break;
	case aggregating: migrate_data(); break;
	case sorting:
		if (reason == reason_t::needs_record_count
		    || reason == reason_t::needs_series_type)
			return;
		if (auto *ptr = std::get_if<copy_source>(&source)) {
			if (auto &&indices = ptr->other->get_sorted_indices(this,
			        std::exchange(unsafe_get<sorting>(state_data),
			            {}));
			    !std::ranges::is_sorted(indices))
				ptr->sorted_indices = std::move(indices);
		}
		else {
			auto &owning = *unsafe_get<source_type::owning>(source);
			if (auto &&indices = owning.get_sorted_indices(this,
			        std::exchange(unsafe_get<sorting>(state_data),
			            {}));
			    !std::ranges::is_sorted(indices))
				owning.sort(std::move(indices));
		}
		break;
	default:
	case finalized:
		if (reason != reason_t::needs_own_state) return;
		break;
	}

	switch (new_state) {
		using enum state_type;
	case modifying:
		migrate_data();
		state_data.emplace<modifying>();
		break;
	case aggregating:
		if (auto *ptr = std::get_if<copy_source>(&source);
		    ptr && ptr->sorted_indices)
			throw std::runtime_error(
			    "Dataframe cannot aggregate sorted data.");
		state_data.emplace<aggregating>();
		break;
	case sorting:
		if (auto *ptr = std::get_if<copy_source>(&source);
		    ptr && ptr->sorted_indices)
			throw std::runtime_error(
			    "Dataframe cannot sort already sorted data.");

		state_data.emplace<sorting>();
		break;
	case finalized: {
		if (auto *ptr = get_if<source_type::owning>(&source))
			state_data.emplace<finalized>((*ptr)->finalize());
		else
			state_data.emplace<finalized>(
			    *unsafe_get<copy_source>(source).other->finalized);

		auto the_filter = std::move(*std::get_if<0>(&this->filter));
		this->filter.emplace<1>(get_record_count());
		if (the_filter) set_filter(std::move(the_filter));
		break;
	}
	}
}

const data_source &dataframe::get_data_source() const
{
	using enum source_type;
	return source == owning ? *unsafe_get<owning>(source)
	                        : *unsafe_get<copying>(source).other;
}
std::string_view dataframe::get_series_name(
    const series_identifier &id) const &
{
	switch (auto &&ser = get_data_source().get_series(id)) {
		using enum series_type;
	default: return {};
	case measure: return unsafe_get<measure>(ser).first;
	case dimension: return unsafe_get<dimension>(ser).first;
	}
}

std::string_view dataframe::get_record_unique_id(
    const record_identifier &id) const &
{
	if (const auto *ptr = std::get_if<std::string_view>(&id))
		return *ptr;

	const auto *state = get_if<state_type::finalized>(&state_data);
	if (!state)
		throw std::runtime_error("Dataframe is not finalized.");

	const data_source::final_info &info = state->get();
	auto ix = *std::get_if<std::size_t>(&id);
	return ix < info.record_unique_ids.size()
	         ? info.record_unique_ids[ix]
	         : std::string_view{};
}

}
