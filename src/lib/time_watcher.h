#ifndef	TIME_WATHCER_H
#define	TIME_WATHCER_H

#include <boost/function.hpp>
#include <map>
#include <stdint.h>
#include <pthread.h>
#include "time_watcher_listener.h"
#include "time_watcher_target_info.h"
#include "thread_pool.h"

using namespace std;
using namespace boost;

namespace gree {
namespace flare {

class time_watcher {
public:
	typedef map< uint32_t, time_watcher_target_info > target_info_map;

protected:
	target_info_map	_map;
	pthread_mutex_t	_map_mutex;
	thread_pool&		_thread_pool;
	shared_thread		_polling_thread;

public:
	time_watcher(thread_pool& thread_pool);
	~time_watcher();
	void notify_begin(uint32_t id, boost::function<void(timeval)>);
	void notify_end(uint32_t id);
	map< uint32_t, time_watcher_target_info > get_map();
	void start_polling_thread(uint32_t polling_interval_msec);
	void stop_polling_threads();
};

}	// namespace flare
}	// namespace gree

#endif
