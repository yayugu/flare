// order is important. to avoid conflict gree::flare::thread and boost::thread.
#include "storage_access_info.h"

#include "time_watcher_processor.h"
#include "logger.h"
#include "time_util.h"

namespace gree {
namespace flare {

time_watcher_processor::time_watcher_processor(
	shared_ptr<time_watcher_processor> shared_this,
	time_watcher& time_watcher,
	timeval polling_interval
):
		_shared_this(shared_this),
		_time_watcher(time_watcher),
		_polling_interval(polling_interval) {
	this->_shutdown_requested = false;
}

time_watcher_processor::~time_watcher_processor() {
}

void time_watcher_processor::operator()()
{
	timeval t, t2, tmp;
	for(;;) {
		shutdowned = false;
		if (this->_shutdown_requested) {
			log_info("thread shutdown request -> breaking loop", 0);
			break;
		}
		if (this->_polling_interval.tv_sec == 0 &&
				this->_polling_interval.tv_usec == 0) {
			log_info("thread watch disabled -> breaking loop", 0);
			break;
		}

		gettimeofday(&t, NULL);

		this->_check_timestamps();

		gettimeofday(&t2, NULL);
		polling_count++;
		time_util::timer_sub(t2, t, tmp);
		polling_time_us += tmp.tv_usec;
		//time_watcher::target_info_map m = this->_time_watcher.get_map();
		//polling_map_count_sum += m.size();

		//time_util::sleep_timeval(this->_polling_interval);
	}
	shutdowned = true;
	_shared_this.reset();
}

void time_watcher_processor::request_shutdown() {
	this->_shutdown_requested = true;
}

void time_watcher_processor::_check_timestamp(const time_watcher_target_info& info) {
	if (!info.watching) {
		return;
	}
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
