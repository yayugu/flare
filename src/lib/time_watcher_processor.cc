/**
 *	time_watcher_processor.cc
 *
 *	@author	Yuya Yaguchi <yuya.yaguchi@gree.net>
 */

// order is important. to avoid conflict gree::flare::thread and boost::thread.
#include "storage_access_info.h"

#include "time_watcher_processor.h"
#include "logger.h"
#include "time_util.h"

namespace gree {
namespace flare {

time_watcher_processor::time_watcher_processor(
	boost::shared_ptr<time_watcher_processor> shared_this,
	time_watcher& time_watcher,
	timeval polling_interval
):
		_shared_this(shared_this),
		_time_watcher(time_watcher),
		_polling_interval(polling_interval),
		_shutdown_requested(false) {
	pthread_mutex_init(&this->_mutex_shutdown_requested, NULL);
	pthread_cond_init(&this->_cond_shutdown_requested, NULL);
}

time_watcher_processor::~time_watcher_processor() {
	pthread_mutex_destroy(&this->_mutex_shutdown_requested);
	pthread_cond_destroy(&this->_cond_shutdown_requested);
}

void time_watcher_processor::operator()()
{
	for(;;) {
		if (this->_shutdown_requested) {
			log_info("thread shutdown request -> breaking loop", 0);
			break;
		}
		if (this->_polling_interval.tv_sec == 0 &&
				this->_polling_interval.tv_usec == 0) {
			log_info("thread watch disabled -> breaking loop", 0);
			break;
		}
		this->_check_timestamps();

		this->_sleep_with_shutdown_request_wait();
	}
	_shared_this.reset();
}

void time_watcher_processor::request_shutdown() {
	pthread_mutex_lock(&this->_mutex_shutdown_requested);
	this->_shutdown_requested = true;
	pthread_mutex_unlock(&this->_mutex_shutdown_requested);
	pthread_cond_signal(&this->_cond_shutdown_requested);
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

void time_watcher_processor::_sleep_with_shutdown_request_wait() {
	pthread_mutex_lock(&this->_mutex_shutdown_requested);
	if (this->_shutdown_requested == false) {
		timespec interval_timespec = time_util::timeval_to_timespec(this->_polling_interval);
		pthread_cond_timedwait(&this->_cond_shutdown_requested, &this->_mutex_shutdown_requested, &interval_timespec);
	}
	pthread_mutex_unlock(&this->_mutex_shutdown_requested);
}

}	// namespace flare
}	// namespace gree
