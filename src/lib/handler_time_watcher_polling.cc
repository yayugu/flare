#include "handler_time_watcher_polling.h"
#include "logger.h"
#include "storage_access_info.h"
#include "time_util.h"

namespace gree {
namespace flare {

handler_time_watcher_polling::handler_time_watcher_polling(
		shared_thread thread,
		time_watcher& time_watcher,
		timeval polling_interval
):
		thread_handler(thread),
		_time_watcher(time_watcher),
		_polling_interval(polling_interval) {
}

handler_time_watcher_polling::~handler_time_watcher_polling() {
}

int handler_time_watcher_polling::run()
{
	for(;;) {
		if (this->_thread->is_shutdown_request()) {
			log_info("thread shutdown request -> breaking loop");
			break;
		}
		if (this->_polling_interval.tv_sec == 0 &&
				this->_polling_interval.tv_usec == 0) {
			log_info("thread watch disabled -> breaking loop");
			break;
		}
		this->_check_timestamps();
		time_util::sleep_timeval(this->_polling_interval);
	}
	return 0;
}

void handler_time_watcher_polling::_check_timestamp(const time_watcher_target_info& info) {
	timeval now;
	timeval sub;
	gettimeofday(&now, NULL);
	time_util::timer_sub(now, info.timestamp, sub);
	if (time_util::timer_is_bigger(sub, info.threshold)) {
		info.callback(sub);
	}
}

void handler_time_watcher_polling::_check_timestamps() {
	typename time_watcher::target_info_map m = this->_time_watcher.get_map();
	for (
			typename time_watcher::target_info_map::const_iterator it = m.begin();
			it != m.end();
			it++
	) {
		this->_check_timestamp(it->second);
	}
}

}	// namespace flare
}	// namespace gree
// vim: foldmethod=marker tabstop=2 shiftwidth=2 autoindent
