#ifndef	TIME_WATHCER_H
#define	TIME_WATHCER_H

#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <map>
#include <stdint.h>
#include <pthread.h>
#include "time_watcher_processor.h"
#include "time_watcher_target_info.h"
#include "util.h"

using namespace std;
using boost::shared_ptr;

namespace gree {
namespace flare {

class time_watcher_processor;

class time_watcher {
public:
	typedef map<uint64_t, time_watcher_target_info> target_info_map;
	typedef vector<target_info_map> target_info_map_slots;
	typedef vector<pthread_mutex_t> mutex_slots;

protected:
	target_info_map_slots              _map_slots;
	mutex_slots                        _map_mutex_slots;
	AtomicCounter                      _id_generator;

	// TODO: unique_ptrの方がいい気がする. boostで使えるか確認
	shared_ptr<time_watcher_processor> _processor;
	shared_ptr<boost::thread>          _thread;

public:
	time_watcher(size_t map_slots_size);
	~time_watcher();
	uint64_t register_target(timeval threshold, boost::function<void(timeval)>);
	void unregister_target(uint64_t id);
	target_info_map_slots get_map_slots();
	void start(uint32_t polling_interval_msec);
	void stop();

protected:
	uint64_t _generate_id();
	uint32_t _slot_index_from_id(uint64_t id);
};

}	// namespace flare
}	// namespace gree

#endif
