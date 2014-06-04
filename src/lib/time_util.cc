/**
 *	time_util.cc
 *
 *	@author	Yuya YAGUCHI <yuya.yaguchi@gree.net>
 *
 *	$Id$
 */
#include "time_util.h"
#include <unistd.h>

namespace gree {
namespace flare {

timeval time_util::msec_to_timeval(uint32_t msec) {
	timeval t;
	t.tv_sec = msec / 1000;
	t.tv_usec = (msec - t.tv_sec * 1000) * 1000;
	return t;
}

void time_util::timer_sub(const timeval& a, const timeval& b, timeval& result) {
	result.tv_sec = a.tv_sec - b.tv_sec;
	result.tv_usec = a.tv_usec - b.tv_usec;
	if (result.tv_usec < 0) {
		--result.tv_sec;
		result.tv_usec += 1000000;
	}
}

bool time_util::timer_is_bigger(const timeval& a, const timeval& b) {
	return a.tv_sec == b.tv_sec
		? a.tv_usec > b.tv_usec
		: a.tv_sec > b.tv_sec;
}

void time_util::sleep_timeval(const timeval& t) {
	if (t.tv_sec != 0) {
		sleep(t.tv_sec);
	}
	usleep(t.tv_usec);
}

}	// namespace flare
}	// namespace gree

// vim: foldmethod=marker tabstop=2 shiftwidth=2 autoindent
