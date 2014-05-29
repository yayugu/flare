#include "handler_time_watcher_polling.h"
#include "storage_access_info.h"
#include "time_util.h"
#include "time_watcher.h"

namespace gree {
namespace flare {

time_watcher::time_watcher(thread_pool& thread_pool):
	_thread_pool(thread_pool) {
	this->_map.clear();
	pthread_mutex_init(&this->_map_mutex, NULL);
}

time_watcher::~time_watcher() {
}

time_watcher::target_info_map time_watcher::get_map() {
	pthread_mutex_lock(&this->_map_mutex);
	time_watcher::target_info_map m = this->_map;
	pthread_mutex_unlock(&this->_map_mutex);
	return m;
}

void time_watcher::notify_begin(uint32_t id, boost::function<void(timeval)>) {
	time_watcher_target_info info;
	gettimeofday(&info.timestamp, NULL);
	pthread_mutex_lock(&this->_map_mutex);
	this->_map[id] = info;
	pthread_mutex_unlock(&this->_map_mutex);
}

void time_watcher::notify_end(uint32_t id) {
	pthread_mutex_lock(&this->_map_mutex);
	this->_map.erase(id);
	pthread_mutex_unlock(&this->_map_mutex);
}

void time_watcher::start_polling_thread(uint32_t polling_interval_msec) {
	this->_polling_thread = this->_thread_pool.get(thread_pool::thread_type_thread_watch);
	handler_time_watcher_polling* handler = new handler_time_watcher_polling(
			this->_polling_thread,
			*this,
			time_util::msec_to_timeval(polling_interval_msec)
	);
	this->_polling_thread->trigger(handler);
}

void time_watcher::stop_polling_threads() {
	this->_polling_thread->shutdown(
		true, // graceful
		true  // async
	);
}

}	// namespace flare
}	// namespace gree
