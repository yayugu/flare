#include "storage_access_info.h"
#include "time_util.h"
#include "time_watcher.h"
#include <libhashkit/hashkit.h>

namespace gree {
namespace flare {

time_watcher::time_watcher(size_t map_slots_size):
	_id_generator(0) {
	for (size_t i = 0; i < map_slots_size; i++) {
		target_info_map m;
		this->_map_slots.push_back(m);
	}
	for (size_t i = 0; i < map_slots_size; i++) {
		pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
		this->_map_mutex_slots.push_back(mutex);
	}
	this->_thread.reset();
	this->_processor.reset();
}

time_watcher::~time_watcher() {
}

time_watcher::target_info_map_slots time_watcher::get_map_slots() {
	time_watcher::target_info_map_slots v;
	for (size_t i = 0; i < this->_map_slots.size(); i++) {
		pthread_mutex_lock(&this->_map_mutex_slots[i]);
		v.push_back(this->_map_slots[i]); // copy
		pthread_mutex_unlock(&this->_map_mutex_slots[i]);
	}
	return v;
}

uint64_t time_watcher::register_target(timeval threshold, boost::function<void(timeval)> callback) {
	time_watcher_target_info info;
	gettimeofday(&info.timestamp, NULL);
	info.threshold = threshold;
	info.callback = callback;
	uint64_t id = this->_generate_id();
	uint32_t slot_index = this->_slot_index_from_id(id);
	pthread_mutex_lock(&this->_map_mutex_slots[slot_index]);
	this->_map_slots[slot_index][id] = info;
	pthread_mutex_unlock(&this->_map_mutex_slots[slot_index]);
	return id;
}

void time_watcher::unregister_target(uint64_t id) {
	uint32_t slot_index = this->_slot_index_from_id(id);
	pthread_mutex_lock(&this->_map_mutex_slots[slot_index]);
	this->_map_slots[slot_index].erase(id);
	pthread_mutex_unlock(&this->_map_mutex_slots[slot_index]);
}

void time_watcher::start(uint32_t polling_interval_msec) {
	this->_processor.reset(new time_watcher_processor(
		this->_processor,
		*this,
		time_util::msec_to_timeval(polling_interval_msec)
	));
	this->_thread.reset(new boost::thread(boost::ref(*this->_processor)));
}

void time_watcher::stop() {
	this->_processor->request_shutdown();
	this->_thread->detach();
	this->_thread.reset();
	this->_processor.reset();
}

uint64_t time_watcher::_generate_id() {
	uint64_t id;
	for (;;) {
		id = this->_id_generator.incr();
		uint32_t slot_index = this->_slot_index_from_id(id);
		pthread_mutex_lock(&this->_map_mutex_slots[slot_index]);
		target_info_map& m = this->_map_slots[slot_index];
		if (m.find(id) == m.end()) {
			pthread_mutex_unlock(&this->_map_mutex_slots[slot_index]);
			break;
		}
		pthread_mutex_unlock(&this->_map_mutex_slots[slot_index]);
	}
	return id;
}

uint32_t time_watcher::_slot_index_from_id(uint64_t id) {
	return libhashkit_murmur((const char *)&id, sizeof(id)) % this->_map_mutex_slots.size();
}

}	// namespace flare
}	// namespace gree
