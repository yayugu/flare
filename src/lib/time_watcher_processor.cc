#include "time_watcher_processor.h"
#include "logger.h"
#include "storage_access_info.h"
#include "time_util.h"

namespace gree {
namespace flare {

time_watcher_processor::time_watcher_processor(
		time_watcher& time_watcher,
		timeval polling_interval
):
		_time_watcher(time_watcher),
		_polling_interval(polling_interval) {
	this->_shutdown_requested = false;
}

time_watcher_processor::~time_watcher_processor() {
}

void time_watcher_processor::operator()()
{
	for(;;) {
		if (this->_shutdown_requested) {
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
}

void time_watcher_processor::request_shutdown() {
	this->_shutdown_requested = true;
}

void time_watcher_processor::_check_timestamp(const time_watcher_target_info& info) {
	timeval now;
	timeval sub;
	gettimeofday(&now, NULL);
	time_util::timer_sub(now, info.timestamp, sub);
	if (time_util::timer_is_bigger(sub, info.threshold)) {
		info.callback(sub);
	}
}

void time_watcher_processor::_check_timestamps() {
	time_watcher::target_info_map m = this->_time_watcher.get_map();
	for (
			time_watcher::target_info_map::const_iterator it = m.begin();
			it != m.end();
			it++
	) {
		this->_check_timestamp(it->second);
	}
}

}	// namespace flare
}	// namespace gree
