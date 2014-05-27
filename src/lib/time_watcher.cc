#include "handler_time_watcher_polling.h"
#include "storage_access_info.h"
#include "time_util.h"
#include "time_watcher.h"

namespace gree {
namespace flare {

template<class T>
time_watcher<T>::time_watcher(thread_pool& thread_pool):
	_thread_pool(thread_pool) {
	this->_map.clear();
	pthread_mutex_init(&this->_map_mutex, NULL);
}

template<class T>
time_watcher<T>::~time_watcher() {
}

template<class T>
typename time_watcher<T>::target_info_map time_watcher<T>::get_map() {
	pthread_mutex_lock(&this->_map_mutex);
	time_watcher<T>::target_info_map m = this->_map;
	pthread_mutex_unlock(&this->_map_mutex);
	return m;
}

template<class T>
void time_watcher<T>::notify_begin(uint32_t id, T additional_info) {
	time_watcher_target_info<T> info;
	gettimeofday(&info.timestamp, NULL);
	info.additional_info = additional_info;
	pthread_mutex_lock(&this->_map_mutex);
	this->_map[id] = info;
	pthread_mutex_unlock(&this->_map_mutex);

}

template<class T>
void time_watcher<T>::notify_end(uint32_t id) {
	pthread_mutex_lock(&this->_map_mutex);
	this->_map.erase(id);
	pthread_mutex_unlock(&this->_map_mutex);
}

template<class T>
void time_watcher<T>::start_polling_thread(
		time_watcher_listener<T>& listener,
		uint32_t polling_interval_msec,
		uint32_t threshold_msec
) {
	this->_polling_thread = this->_thread_pool.get(thread_pool::thread_type_thread_watch);
	handler_time_watcher_polling<T>* handler = new handler_time_watcher_polling<T>(
			this->_polling_thread,
			*this,
			listener,
			time_util::msec_to_timeval(polling_interval_msec),
			time_util::msec_to_timeval(threshold_msec)
	);
	this->_polling_thread->trigger(handler);
}

template<class T>
void time_watcher<T>::stop_polling_threads() {
	this->_polling_thread->shutdown(
		true, // graceful
		true  // async
	);
}

template class time_watcher<storage_access_info>;

}	// namespace flare
}	// namespace gree
