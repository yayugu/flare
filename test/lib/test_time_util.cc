#include <cppcutter.h>
#include "time_util.h"

using namespace std;
using namespace gree::flare;

namespace test_time_util {
	void setup() {
	}

	void teardown() {
	}

	void test_msec_to_tiveval () {
		timeval actual;
		actual = time_util::msec_to_timeval(1500);
		cut_assert_equal_int(1, actual.tv_sec);
		cut_assert_equal_int(500000, actual.tv_usec);

		actual = time_util::msec_to_timeval(340);
		cut_assert_equal_int(0, actual.tv_sec);
		cut_assert_equal_int(340000, actual.tv_usec);
	}

	void test_timer_sub () {
		timeval actual;
		timeval a;
		timeval b;

		a.tv_sec = 15;
		a.tv_usec = 25;
		b.tv_sec = 10;
		b.tv_usec = 20;
		time_util::timer_sub(a, b, actual);
		cut_assert_equal_int(5, actual.tv_sec);
		cut_assert_equal_int(5, actual.tv_usec);

		a.tv_sec = 15;
		a.tv_usec = 25;
		b.tv_sec = 0;
		b.tv_usec = 40;
		time_util::timer_sub(a, b, actual);
		cut_assert_equal_int(14, actual.tv_sec);
		cut_assert_equal_int(999985, actual.tv_usec);

		a.tv_sec = 15;
		a.tv_usec = 25;
		b.tv_sec = 30;
		b.tv_usec = 25;
		time_util::timer_sub(a, b, actual);
		cut_assert_equal_int(-15, actual.tv_sec);
		cut_assert_equal_int(0, actual.tv_usec);
	}

	void test_timer_is_bigger () {
		timeval a;
		timeval b;

		// bigger
		a.tv_sec = 15;
		a.tv_usec = 25;
		b.tv_sec = 10;
		b.tv_usec = 20;
		cut_assert_true(time_util::timer_is_bigger(a, b));

		a.tv_sec = 15;
		a.tv_usec = 25;
		b.tv_sec = 15;
		b.tv_usec = 20;
		cut_assert_true(time_util::timer_is_bigger(a, b));

		// same
		a.tv_sec = 15;
		a.tv_usec = 25;
		b.tv_sec = 15;
		b.tv_usec = 25;
		cut_assert_false(time_util::timer_is_bigger(a, b));

		// smaller
		a.tv_sec = 15;
		a.tv_usec = 25;
		b.tv_sec = 15;
		b.tv_usec = 35;
		cut_assert_false(time_util::timer_is_bigger(a, b));
	}
}
