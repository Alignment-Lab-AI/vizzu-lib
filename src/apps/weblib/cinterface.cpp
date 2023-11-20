#include "cinterface.h"

#include "interface.h"

using Vizzu::Interface;

static_assert(sizeof(double) == 8);
static_assert(offsetof(Point, x) == 0);
static_assert(offsetof(Point, y) == 8);
static_assert(offsetof(Value, dimension) == 0);
static_assert(offsetof(Value, measureValue) == 8);
static_assert(offsetof(Value, dimensionValue) == 8);

constexpr std::uint_fast32_t hash(std::string_view s) noexcept
{
	std::uint_fast32_t val{};

	for (size_t i = 0; i < s.size(); ++i)
		val ^= static_cast<std::uint_fast32_t>(s[i])
		    << ((i % sizeof(std::uint_fast32_t)) * CHAR_BIT);

	return val;
}

const char *vizzu_errorMessage(APIHandles::Exception exceptionPtr,
    const std::type_info *typeinfo)
{
	std::string_view type_info = typeinfo->name();

	if (type_info.starts_with("St")
	    || type_info.starts_with("NSt")) { // std::
		constexpr auto valid = "abcdefghijklmnopqrstuvwxyz_";
		auto last = type_info.find_last_of(valid);
		auto first = type_info.find_last_not_of(valid, last);
		type_info = type_info.substr(first + 1, last - first);

		const std::exception *realException = nullptr;

		switch (hash(type_info)) {
		case hash("exception"):
			realException =
			    static_cast<const std::exception *>(exceptionPtr);
			break;
		case hash("bad_alloc"):
			realException =
			    static_cast<const std::bad_alloc *>(exceptionPtr);
			break;
		case hash("bad_array_new_length"):
			realException =
			    static_cast<const std::bad_array_new_length *>(
			        exceptionPtr);
			break;
		case hash("bad_optional_access"):
			realException =
			    static_cast<const std::bad_optional_access *>(
			        exceptionPtr);
			break;
		case hash("logic_error"):
			realException =
			    static_cast<const std::logic_error *>(exceptionPtr);
			break;
		case hash("invalid_argument"):
			realException =
			    static_cast<const std::invalid_argument *>(
			        exceptionPtr);
			break;
		case hash("length_error"):
			realException =
			    static_cast<const std::length_error *>(exceptionPtr);
			break;
		case hash("out_of_range"):
			realException =
			    static_cast<const std::out_of_range *>(exceptionPtr);
			break;
		case hash("runtime_error"):
			realException =
			    static_cast<const std::runtime_error *>(exceptionPtr);
			break;
		case hash("bad_cast"):
			realException =
			    static_cast<const std::bad_cast *>(exceptionPtr);
			break;
		case hash("bad_any_cast"):
			realException =
			    static_cast<const std::bad_any_cast *>(exceptionPtr);
			break;
		case hash("bad_function_call"):
			realException =
			    static_cast<const std::bad_function_call *>(
			        exceptionPtr);
			break;
		}

		if (realException) return realException->what();
	}

	thread_local std::string data;
	data = "Cannot call '";
	data += type_info;
	data += "::what()'. Please add this exception to the ";
	data += __func__;
	data += " function.";
	return data.c_str();
}

const char *vizzu_version() { return Interface::version(); }

void vizzu_setLogging(bool enable)
{
	return Interface::setLogging(enable);
}

APIHandles::Chart vizzu_createChart()
{
	return Interface::getInstance().createChart();
}

APIHandles::Canvas vizzu_createCanvas()
{
	return Interface::getInstance().createCanvas();
}

void vizzu_pointerMove(APIHandles::Chart chart,
    APIHandles::Canvas canvas,
    int pointerId,
    double x,
    double y)
{
	return Interface::getInstance().pointerMove(chart,
	    canvas,
	    pointerId,
	    x,
	    y);
}

void vizzu_pointerDown(APIHandles::Chart chart,
    APIHandles::Canvas canvas,
    int pointerId,
    double x,
    double y)
{
	return Interface::getInstance().pointerDown(chart,
	    canvas,
	    pointerId,
	    x,
	    y);
}

void vizzu_pointerUp(APIHandles::Chart chart,
    APIHandles::Canvas canvas,
    int pointerId,
    double x,
    double y)
{
	return Interface::getInstance().pointerUp(chart,
	    canvas,
	    pointerId,
	    x,
	    y);
}

void vizzu_pointerLeave(APIHandles::Chart chart,
    APIHandles::Canvas canvas,
    int pointerId)
{
	return Interface::getInstance().pointerLeave(chart,
	    canvas,
	    pointerId);
}

void vizzu_wheel(APIHandles::Chart chart,
    APIHandles::Canvas canvas,
    double delta)
{
	return Interface::getInstance().wheel(chart, canvas, delta);
}

void vizzu_update(APIHandles::Chart chart,
    APIHandles::Canvas canvas,
    double width,
    double height,
    int renderControl)
{
	return Interface::getInstance().update(chart,
	    canvas,
	    width,
	    height,
	    static_cast<Interface::RenderControl>(renderControl));
}

const char *style_getList() { return Interface::getStyleList(); }

void style_setValue(APIHandles::Chart chart,
    const char *path,
    const char *value)
{
	return Interface::getInstance().setStyleValue(chart, path, value);
}

const char *style_getValue(APIHandles::Chart chart,
    const char *path,
    bool computed)
{
	return Interface::getInstance().getStyleValue(chart,
	    path,
	    computed);
}

APIHandles::Snapshot chart_store(APIHandles::Chart chart)
{
	return Interface::getInstance().storeChart(chart);
}

void chart_restore(APIHandles::Chart chart,
    APIHandles::Snapshot snapshot)
{
	return Interface::getInstance().restoreChart(chart, snapshot);
}

APIHandles::Animation chart_anim_store(APIHandles::Chart chart)
{
	return Interface::getInstance().storeAnim(chart);
}

void chart_anim_restore(APIHandles::Chart chart,
    APIHandles::Animation anim)
{
	return Interface::getInstance().restoreAnim(chart, anim);
}

void object_free(APIHandles::Any handle)
{
	return Interface::getInstance().freeObj(handle);
}

const char *chart_getList() { return Interface::getChartParamList(); }

const char *chart_getValue(APIHandles::Chart chart, const char *path)
{
	return Interface::getInstance().getChartValue(chart, path);
}

void chart_setValue(APIHandles::Chart chart,
    const char *path,
    const char *value)
{
	return Interface::getInstance().setChartValue(chart, path, value);
}

void chart_setFilter(APIHandles::Chart chart,
    bool (*filter)(const Vizzu::Data::RowWrapper *),
    void (*deleter)(bool (*)(const Vizzu::Data::RowWrapper *)))
{
	if (filter)
		return Interface::getInstance().setChartFilter(chart,
		    Vizzu::JsFunctionWrapper<bool,
		        const Vizzu::Data::RowWrapper &>{{filter, deleter}});

	return Interface::getInstance().setChartFilter(chart,
	    Vizzu::JsFunctionWrapper<bool,
	        const Vizzu::Data::RowWrapper &>{});
}

const Value *record_getValue(const Vizzu::Data::RowWrapper *record,
    const char *column)
{
	thread_local Value val{{}, {}};
	std::visit(
	    []<class T>(T to)
	    {
		    if constexpr (std::is_same_v<T, double>)
			    val = Value{false, {.measureValue = to}};
		    else
			    val = Value{true, {.dimensionValue = to}};
	    },
	    Interface::getRecordValue(*record, column));

	return &val;
}

void data_addDimension(APIHandles::Chart chart,
    const char *name,
    const char **categories,
    int count)
{
	return Interface::getInstance().addDimension(chart,
	    name,
	    categories,
	    count);
}

void data_addMeasure(APIHandles::Chart chart,
    const char *name,
    const char *unit,
    double *values,
    int count)
{
	return Interface::getInstance().addMeasure(chart,
	    name,
	    unit,
	    values,
	    count);
}

void data_addRecord(APIHandles::Chart chart,
    const char **cells,
    int count)
{
	return Interface::getInstance().addRecord(chart, cells, count);
}

const char *data_metaInfo(APIHandles::Chart chart)
{
	return Interface::getInstance().dataMetaInfo(chart);
}

void addEventListener(APIHandles::Chart chart,
    const char *name,
    void (*callback)(APIHandles::Event event, const char *))
{
	return Interface::getInstance().addEventListener(chart,
	    name,
	    callback);
}

void removeEventListener(APIHandles::Chart chart,
    const char *name,
    void (*callback)(APIHandles::Event event, const char *))
{
	return Interface::getInstance().removeEventListener(chart,
	    name,
	    callback);
}

void event_preventDefault(APIHandles::Event event)
{
	return Interface::getInstance().preventDefaultEvent(event);
}

void chart_animate(APIHandles::Chart chart, void (*callback)(bool))
{
	return Interface::getInstance().animate(chart, callback);
}

const Point *
chart_relToCanvasCoords(APIHandles::Chart chart, double rx, double ry)
{
	thread_local Point res;
	Interface::getInstance().relToCanvasCoords(chart,
	    rx,
	    ry,
	    res.x,
	    res.y);
	return &res;
}

const Point *
chart_canvasToRelCoords(APIHandles::Chart chart, double x, double y)
{
	thread_local Point res;
	Interface::getInstance().canvasToRelCoords(chart,
	    x,
	    y,
	    res.x,
	    res.y);
	return &res;
}

void chart_setKeyframe(APIHandles::Chart chart)
{
	return Interface::getInstance().setKeyframe(chart);
}

void anim_control_setValue(APIHandles::Chart chart,
    const char *path,
    const char *value)
{
	return Interface::getInstance().setAnimControlValue(chart,
	    path,
	    value);
}

const char *anim_control_getValue(APIHandles::Chart chart,
    const char *path)
{
	return Interface::getInstance().getAnimControlValue(chart, path);
}

void anim_setValue(APIHandles::Chart chart,
    const char *path,
    const char *value)
{
	return Interface::getInstance().setAnimValue(chart, path, value);
}
