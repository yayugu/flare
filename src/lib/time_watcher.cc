/**
 *	time_watcher.h
 *
 *	@author	Yuya Yaguchi <yuya.yaguchi@gree.net>
 */
#include "storage_access_info.h"
#include "time_util.h"
#include "time_watcher.h"

namespace gree {
namespace flare {

time_watcher::time_watcher():
	_map(),
	_id_generator(0),
	_is_polling(false),
	_processor(),
	_thread() {
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

uint64_t time_watcher::register_target(timeval threshold, boost::function<void(timeval)> callback) {
	time_watcher_target_info info;
	gettimeofday(&info.timestamp, NULL);
	info.threshold = threshold;
	info.callback = callback;
	pthread_mutex_lock(&this->_map_mutex);
	uint64_t id = this->_generate_id();
	this->_map[id] = info;
	pthread_mutex_unlock(&this->_map_mutex);
	return id;
}

void time_watcher::unregister_target(uint64_t id) {
	pthread_mutex_lock(&this->_map_mutex);
	this->_map.erase(id);
	pthread_mutex_unlock(&this->_map_mutex);
}

void time_watcher::start(uint32_t polling_interval_msec) {
	if (_is_polling) {
		log_err("a polling thread is still running.", 0);
		return;
	}
	this->_processor.reset(new time_watcher_processor(
		this->_processor,
		*this,
		time_util::msec_to_timeval(polling_interval_msec)
	));
	this->_thread.reset(new boost::thread(boost::ref(*this->_processor)));
	this->_is_polling = true;
}

void time_watcher::stop() {
	if (!_is_polling) {
		return;
	}
	this->_processor->request_shutdown();
	this->_thread->detach();
	this->_thread.reset();
	this->_processor.reset();
	this->_is_polling = false;
}

uint64_t time_watcher::_generate_id() {
	uint64_t id;
	do {
		id = this->_id_generator.incr();
	} while (this->_map.find(id) != this->_map.end());
	return id;
}

}	// namespace flare
}	// namespace gree
