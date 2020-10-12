/*
 * profiler.hpp
 *
 *  Created on: May 24, 2019
 *      Author: bflynt
 */

#ifndef PROFILER_PROFILER_HPP_
#define PROFILER_PROFILER_HPP_


#if defined( XSTD_USE_PROFILE )

#include "xstd/detail/config/current.hpp"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>


namespace xstd {

/// Profiler class for recording function timings
/**
 * The Profiler class records function call times by
 * using the construction and destruction of the class
 * as the start and end times for that function call.
 * Creation of further nested Profiler instances can be
 * subtracted or included within the timing values.
 *
 * It is recommended to use the PROFILE() preprocessor
 * macro to properly insert the profiler and capture
 * the file function names, files and line numbers.
 *
 * Example:
 * \code{.cpp}
 * int MyFunction(){
 *    PROFILE();
 *
 *    // Program Contents
 * };
 * \endcode
 *
 * By default the PROFILE() macro evaluates to nothing
 * and will not profile the application. To enable profiling
 * the XSTD_USE_PROFILE environment variable must be set when
 * building the applications.  When set the macro evaluates
 * to create an instance of the Profiler as shown below.
 *
 * Example:
 * \code{.cpp}
 * int MyFunction(){
 *    xstd::Profiler::Injection local_scope_profiler(__PRETTY_FUNCTION__,__FILE__,__LINE__);
 *
 *    // Program Contents
 * };
 * \endcode
 *
 * To display or log the profile information a second macro
 * must be used at the point where profile information should
 * be displayed.  For example, at the end of the application
 * the following statement can be inserted.
 *
 * Example:
 * \code{.cpp}
 * int main(){
 *    PROFILE();
 *
 *    // Program Contents ...
 *
 *    // ... End of Program
 *    PROFILE_TO_STREAM(std::cout);
 * };
 * \endcode
 *
 */
class Profiler final {

public:
	using system_clock      = std::chrono::system_clock;
	using steady_clock      = std::chrono::steady_clock;
	using system_duration   = typename system_clock::duration;
	using steady_duration   = typename steady_clock::duration;
	using system_time_point = typename system_clock::time_point;
	using steady_time_point = typename steady_clock::time_point;
	using String          = std::string;
	using LineNumber      = unsigned;
	using SizeType        = std::size_t;
	using Stack           = std::stack<String>;

	class Injection final {
	public:
		Injection() = delete;
		Injection(const Injection&) = delete;
		Injection(Injection&&) = delete;
		Injection& operator=(const Injection&) = delete;
		Injection& operator=(Injection&&) = delete;

		Injection(const String name, const String file, const LineNumber line)
		: system_(system_clock::now()),
		  steady_(steady_clock::now()){
			Profiler::instance().start(name,file,line);
		}

		~Injection(){
			Profiler::instance().stop(
					system_clock::now() - system_,
					steady_clock::now() - steady_);
		}

	private:
		system_time_point system_;
		steady_time_point steady_;
	};

	// Return One and Only Instance of Singleton
	static Profiler& instance(){
		static Profiler instance_;
		return instance_;
	}

	~Profiler(){
		assert( stack_.empty() );
	}

	void start(
			const String     name,
			const String     file,
			const LineNumber line){
		stack_.push(name);
		if( total_.count(name) == 0 ){
			total_.emplace(
					std::piecewise_construct,
					std::forward_as_tuple(name),
					std::forward_as_tuple(name,file,line));
		}
	}

	void stop(
			const system_duration system_dur,
			const steady_duration steady_dur){
		assert( not stack_.empty() );
		const String name = stack_.top();
		stack_.pop();

		// Add to my totals
		auto acc_iter = total_.find(name);
		assert(acc_iter != total_.end());
		(*acc_iter).second.increment_all(system_dur,steady_dur);

		// Subtract my total from my parents exclusive time
		if( not stack_.empty() ){
			const String parent_name = stack_.top();
			auto parent_iter = total_.find(parent_name);
			assert(parent_iter != total_.end());
			(*parent_iter).second.sub_exclusive(system_dur,steady_dur);
		}
	}

	std::string to_string() const {
		return this->sort_to_string_();
	}




private:

	class Accumulator final {
	public:
		Accumulator() = delete;
		Accumulator(const Accumulator&) = default;
		Accumulator(Accumulator&&) = default;
		Accumulator& operator=(const Accumulator&) = default;
		Accumulator& operator=(Accumulator&&) = default;

		Accumulator(const String name, const String file, const LineNumber line)
			: name_(name), file_(file), count_(0), line_(line){}

		void increment_all(
				const system_duration& system_dur,
				const steady_duration& steady_dur){
			system_excl_ += system_dur;
			steady_excl_ += steady_dur;
			system_      += system_dur;
			steady_      += steady_dur;
			count_       += 1;
		}

		void sub_exclusive(
				const system_duration& system_dur,
				const steady_duration& steady_dur){
			system_excl_ -= system_dur;
			steady_excl_ -= steady_dur;
		}

		bool operator<(const Accumulator& rhs){
			constexpr SizeType one = 1;
			auto my_cost  = steady_excl_ / std::max(one,count_);
			auto rhs_cost = rhs.steady_excl_ / std::max(one,rhs.count_);
			return (my_cost < rhs_cost);
		}

		String          name_;
		String          file_;
		system_duration system_excl_;
		steady_duration steady_excl_;
		system_duration system_;
		steady_duration steady_;
		SizeType        count_;
		LineNumber      line_;
	};


	std::string sort_to_string_() const{

		// Sort the timing by (steady_excl_/count_)
		std::vector<Accumulator> vacc;
		vacc.reserve(total_.size());
		for(const auto& acc : total_){
			vacc.push_back(acc.second);
		}
		std::sort(vacc.begin(),vacc.end());
		std::reverse(vacc.begin(),vacc.end());

		// Iterate over sending to log_out
		std::stringstream log_out;
		for(const auto& acc : vacc){
			log_out << acc.name_ << "\n";
			log_out << "File: "   << acc.file_ << "\n";
			log_out << "Line: "   << acc.line_ << "\n";
			log_out << "Calls: "  << acc.count_ << "\n";
			log_out << "System: " << acc.system_.count() << "\n";
			log_out << "Steady: " << acc.steady_.count() << "\n";
			log_out << "System Exclusive: " << acc.system_excl_.count() << "\n";
			log_out << "Steady Exclusive: " << acc.steady_excl_.count() << "\n";
			log_out << "\n";
		}
		return log_out.str();
	}


	using UnorderedMap = std::unordered_map<String,Accumulator>;

	UnorderedMap total_;
	Stack        stack_;
};

} /* namespace xstd */


/// Profile remainder of scope
#ifndef PROFILE
#define PROFILE() xstd::Profiler::Injection local_scope_profiler(XSTD_CURRENT_FUNCTION,__FILE__,__LINE__);
#endif


/// Macro to display profile information to stream
#ifndef PROFILE_TO_STREAM
#define PROFILE_TO_STREAM(log_stream)                           \
	do{                                                         \
		log_stream << xstd::Profiler::instance().to_string();   \
	}while(0);
#endif


#else  /* #ifndef XSTD_USE_PROFILE */


/// Ignore the Profiler Request
#ifndef PROFILE
#define PROFILE() ((void)0);
#endif

/// Ignore the display profile request
#ifndef PROFILE_TO_STREAM
#define PROFILE_TO_STREAM(log_stream) ((void)0);
#endif

#endif /* #ifndef XSTD_USE_PROFILE */


#endif /* PROFILER_PROFILER_HPP_ */
