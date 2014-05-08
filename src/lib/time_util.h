/**
 *	time_util.h
 *
 *	@author	Yuya YAGUCHI <yuya.yaguchi@gree.net>
 *
 *	$Id$
 */
#ifndef	TIME_UTIL_H
#define	TIME_UTIL_H

#include <time.h>
#include <stdint.h>

namespace gree {
namespace flare {

class time_util {
public:
	static timeval msec_to_timeval(uint32_t msec);
	static void sleep_timeval(const timeval& t);

	// OSX doesn't have timersub() and timercmp()
	static void timer_sub(const timeval& a, const timeval& b, timeval& res);
	static bool timer_is_bigger(const timeval& a, const timeval& b);
};

}	// namespace flare
}	// namespace gree

#endif	// TIME_UTIL_H
// vim: foldmethod=marker tabstop=2 shiftwidth=2 autoindent
