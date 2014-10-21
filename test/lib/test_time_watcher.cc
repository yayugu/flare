/**
*	test_time_watcher.cc
*
*	@author	Yuya YAGUCHI <yuya.yaguchi@gree.net>
*/

#include <cppcutter.h>

#include <time_watcher.h>
#include <time_util.h>

using namespace gree::flare;

namespace test_time_watcher {

bool fired;
bool fired2;
time_watcher* tw;

void _callback(timeval t) {
	fired = true;
}

void _callback2(timeval t) {
	fired2 = true;
}

void cut_setup() {
	fired = false;
	fired2 = false;
	tw = new time_watcher;
}

void test_time_watcher_over_threshold() {
	tw->start(1);
	uint64_t id = tw->register_target(time_util::msec_to_timeval(1), _callback);
	time_util::sleep_timeval(time_util::msec_to_timeval(10));
	tw->stop();
	cut_assert_true(fired);
}

void test_time_watcher_below_threshold() {
	tw->start(1);
	uint64_t id = tw->register_target(time_util::msec_to_timeval(100), _callback);
	time_util::sleep_timeval(time_util::msec_to_timeval(10));
	tw->stop();
	cut_assert_false(fired);
}

void test_time_watcher_disabled() {
	tw->start(0); // which means disabled
	uint64_t id = tw->register_target(time_util::msec_to_timeval(1), _callback);
	time_util::sleep_timeval(time_util::msec_to_timeval(10));
	tw->stop();
	cut_assert_false(fired);
}

void test_time_watcher_callback_multi() {
	tw->start(1);
	uint64_t id = tw->register_target(time_util::msec_to_timeval(100), _callback);
	uint64_t id2 = tw->register_target(time_util::msec_to_timeval(1), _callback2);
	time_util::sleep_timeval(time_util::msec_to_timeval(10));
	tw->stop();
	cut_assert_false(fired);
	cut_assert_true(fired2);
}

void test_time_watcher_restart() {
	tw->start(1);
	uint64_t id = tw->register_target(time_util::msec_to_timeval(100), _callback);
	tw->stop();
	tw->start(1);
	tw->unregister_target(id);
	uint64_t id2 = tw->register_target(time_util::msec_to_timeval(1), _callback2);
	time_util::sleep_timeval(time_util::msec_to_timeval(10));
	tw->stop();
	cut_assert_false(fired);
	cut_assert_true(fired2);
}

void test_time_watcher_long_polling_interval_sleep() {
	tw->start(10000);
	timeval started;
	gettimeofday(&started, NULL);
	time_util::sleep_timeval(time_util::msec_to_timeval(10));
	tw->stop();
	timeval stopped;
	gettimeofday(&stopped, NULL);
	timeval diff;
	time_util::timer_sub(stopped, started, diff);

	timeval expect_max = time_util::msec_to_timeval(50);
	cut_assert_true(time_util::timer_is_bigger(expect_max, diff));
}

void cut_teardown() {
	delete tw;
}

}