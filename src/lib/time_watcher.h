#ifndef	TIME_WATHCER_H
#define	TIME_WATHCER_H

#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <map>
#include <stdint.h>
#include <pthread.h>
#include "time_watcher_processor.h"
#include "time_watcher_target_info.h"

using namespace std;
using boost::shared_ptr;

namespace gree {
namespace flare {

class time_watcher_processor;

class time_watcher {
public:
	typedef map<uint32_t, time_watcher_target_info> target_info_map;

protected:
	target_info_map	_map;
	pthread_mutex_t	_map_mutex;

	// TODO: unique_ptrの方がいい気がする. boostで使えるか確認
	shared_ptr<time_watcher_processor>		_processor;
	shared_ptr<boost::thread>	_thread;

public:
	time_watcher();
	~time_watcher();
	void register_(uint32_t id, timeval threshold, boost::function<void(timeval)>);
	void unregister(uint32_t id);
	map< uint32_t, time_watcher_target_info > get_map();
	void start(uint32_t polling_interval_msec);
	void stop();
};

}	// namespace flare
}	// namespace gree

#endif
