/*
 * tracer.hpp
 *
 *  Created on: Dec 23, 2017
 *      Author: bflynt
 */

#ifndef XSTD_TRACER_HPP_
#define XSTD_TRACER_HPP_


/** \file tracer.hpp
 * Implementation of Tracer tools to trace program execution.
 *
 * The Tracer tools can be used to trace program execution by
 * printing to screen a message indented by the nested creation
 * of Tracer classes.  To enable zero runtime cost for release
 * builds a macro is used to only insert Tracer construction
 * when TRACER_ON is defined.
 */
#if defined( USE_XSTD_TRACER )

#include <iostream>
#include <string>

namespace xstd {

///  Tracer to track program execution to stdout
/**
 *
 * Tracer will trace a programs execution to the screen
 * when a Tracer is constructed and allowed to be destructed
 * when it goes out of scope.
 *
 * Example:
 * \code{.cpp}
 * int MyFunction(){
 *    Tracer my_local_tracer("My Message");
 *
 *    // Program Contents
 * };
 * \endcode
 *
 */
class Tracer {

public:

	Tracer() = delete;
	Tracer(const Tracer& T) = delete;
	Tracer& operator=(const Tracer& T) = delete;

	/// Construct with a message and no prefix.
	/**
	 * Construct the Tracer with a message to be displayed
	 *
	 * \param message[in] Message to be displayed as the trace
	 */
	Tracer(const std::string message);


	/// Construct with a prefix and message.
	/**
	 * Construct the Tracer with a prefix and message
	 * The display will appear as: <prefix>: <message>
	 *
	 * \param prefix[in] Prefix to place before message
	 * \param message[in] Message to be displayed as the trace
	 */
	Tracer(const std::string prefix, const std::string message);

	/// Destruct the Tracer
	/**
	 * Destroys this tracer instance and decrments the
	 * indexing of the parent nested Tracer by m_nest_indent
	 */
	~Tracer();

	/// Returns a reference to the current line indention count.
	/**
	 * Returns a reference to the size of the current
	 * line indention.  This is determined by the current
	 * level of nesting.  This is implemented as a static
	 * function since static data members cannot be
	 * initialized within header files.
	 */
	static std::size_t& indent();

private:
	static constexpr std::size_t m_nest_indent = 3;
	static std::size_t m_current_indent;
};

inline
Tracer::Tracer(const std::string message){
	std::cout << std::string(indent(),' ') << message << std::endl;
	indent() = indent() + m_nest_indent;
}

inline
Tracer::Tracer(const std::string prefix, const std::string message){
	std::cout << std::string(indent(),' ') << prefix << ": " << message << std::endl;
	indent() = indent() + m_nest_indent;
}

inline
Tracer::~Tracer(){
	indent() = indent() - m_nest_indent;
	std::cout << std::string(indent(),' ') << "---" << std::endl;
}

inline
std::size_t& Tracer::indent(){
	static std::size_t m_current_indent{0};
	return m_current_indent;
}


} // namespace xstd



/**
 * \def TRACER(msg)
 * \brief A macro to insert a tracer with message
 * \param	msg	Message to insert after prefix.
 */
#ifndef TRACER
#define TRACER(msg) xstd::Tracer local_scope_tracer(msg);
#endif

#else // #ifdef USE_XSTD_TRACER

/**
 * Macro to ignore Tracer construction
 */
#ifndef TRACER
#define TRACER(msg)
#endif

#endif // #ifdef USE_XSTD_TRACER


#endif /* XSTD_TRACER_HPP_ */
