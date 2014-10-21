/**
 *	time_watcher.h
 *
 *	@author	Yuya Yaguchi <yuya.yaguchi@gree.net>
 */
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
	typedef map<uint64_t, time_watcher_target_info> target_info_map;

protected:
	target_info_map                    _map;
	pthread_mutex_t                    _map_mutex;
	uint64_t                           _id_last;
	bool                               _is_polling;
	boost::shared_ptr<time_watcher_processor> _processor;
	boost::shared_ptr<boost::thread>          _thread;

public:
	time_watcher();
	virtual ~time_watcher();
	uint64_t register_target(timeval threshold, boost::function<void(timeval)>);
	void unregister_target(uint64_t id);
	target_info_map get_map();
	void start(uint32_t polling_interval_msec);
	void stop();

protected:
	uint64_t _generate_id();
};

}	// namespace flare
}	// namespace gree

#endif