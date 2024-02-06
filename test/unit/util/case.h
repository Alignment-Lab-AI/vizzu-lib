#ifndef TEST_ABSTRACT_CASE
#define TEST_ABSTRACT_CASE

#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <utility>

#include "src_location.h"

namespace test
{

using runnable = std::function<void()>;

struct assertion_error : std::runtime_error
{
	src_location location;

	explicit assertion_error(const std::string &msg,
	    src_location location = src_location()) noexcept :
	    runtime_error("Assert " + msg),
	    location(location)
	{}
};

struct skip_error : std::runtime_error
{
	src_location location;

	explicit skip_error(const std::string &msg,
	    src_location location = src_location()) noexcept :
	    runtime_error("Skip " + msg),
	    location(location)
	{}
};

class case_type
{
public:
	case_type(std::string_view suite_name,
	    std::string_view case_name,
	    runnable runner,
	    src_location location) noexcept :
	    suite_name(suite_name),
	    case_name(case_name),
	    runner(runner),
	    location(location)
	{}

	void operator()()
	{
		using std::chrono::steady_clock;

		print_start();
		auto start = steady_clock::now();

		run_safely();

		auto duration = steady_clock::now() - start;
		print_summary(duration);
	}

	void fail(const src_location &location,
	    const std::string &message)
	{
		if (!error_messages.contains(location))
			error_messages.insert({location, message});
	}

	[[nodiscard]] std::string full_name() const
	{
		return "[" + std::string{suite_name} + "] "
		     + std::string{case_name} + " (" + file_name() + ":"
		     + std::to_string(location.get_line()) + ")";
	}

	[[nodiscard]] std::string file_name() const
	{
		return location.get_file_name();
	}

	explicit operator bool() const
	{
		return skip || error_messages.empty();
	}

	void set_latest_location(const src_location &loc)
	{
		latest_location = loc;
	}

private:
	std::string_view suite_name;
	std::string_view case_name;
	runnable runner;
	bool skip = false;
	src_location location;
	src_location latest_location = location;
	std::map<src_location, std::string> error_messages;

	void run_safely() noexcept
	{
		try {
			runner();
		}
		catch (const assertion_error &e) {
			fail(e.location, e.what());
		}
		catch (const skip_error &e) {
			if (error_messages.empty()) { skip = true; }
			fail(e.location, e.what());
		}
		catch (std::exception &e) {
			fail(latest_location,
			    "exception thrown: " + std::string(e.what()));
		}
		catch (...) {
			fail(latest_location, "unknown exception thrown");
		}
	}

	void print_start() const
	{
		std::cout << ansi::fg_yellow << "[ RUN... ] " << ansi::reset
		          << this->full_name() << "\r";
		std::cout.flush();
	}

	void print_summary(auto duration) const
	{
		using std::chrono::milliseconds;

		std::cout << (error_messages.empty()
		                  ? std::string(ansi::fg_green)
		                        + "[     OK ] "
		              : skip
		                  ? std::string(ansi::fg_yellow)
		                        + "[   SKIP ] "
		                  : std::string(ansi::fg_red) + "[ FAILED ] ")
		          << ansi::reset << full_name() << " ("
		          << (duration_cast<milliseconds>(duration).count())
		          << " ms)\n";

		for (const auto &error : error_messages)
			std::cerr << error.first.error_prefix() << error.second
			          << "\n";
	}
};

}

#endif
