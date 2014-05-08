#include "handler_time_watcher_polling.h"
#include "logger.h"
#include "storage_access_info.h"
#include "time_util.h"

namespace gree {
namespace flare {

template<class T>
handler_time_watcher_polling<T>::handler_time_watcher_polling(
		shared_thread thread,
		time_watcher<T>& time_watcher,
		time_watcher_listener<T>& listener,
		timeval polling_interval,
		timeval threshold
):
		thread_handler(thread),
		_time_watcher(time_watcher),
		_listener(listener),
		_polling_interval(polling_interval),
		_threshold(threshold) {
}

template<class T>
handler_time_watcher_polling<T>::~handler_time_watcher_polling() {
}

template<class T>
int handler_time_watcher_polling<T>::run()
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

template<class T>
void handler_time_watcher_polling<T>::_check_timestamp(const time_watcher_target_info<T>& info) {
	timeval now;
	timeval sub;
	gettimeofday(&now, NULL);
	time_util::timer_sub(now, info.timestamp, sub);
	if (time_util::timer_is_bigger(sub, this->_threshold)) {
		_listener.time_watcher_on_over_threshold(sub, info.additional_info);
	}
}

template<class T>
void handler_time_watcher_polling<T>::_check_timestamps() {
	typename time_watcher<T>::target_info_map m = this->_time_watcher.get_map();
	for (
			typename time_watcher<T>::target_info_map::const_iterator it = m.begin();
			it != m.end();
			it++
	) {
		this->_check_timestamp(it->second);
	}
}

template class handler_time_watcher_polling<storage_access_info>;


}	// namespace flare
}	// namespace gree
// vim: foldmethod=marker tabstop=2 shiftwidth=2 autoindent
