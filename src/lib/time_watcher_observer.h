/**
 *	time_watcher_observer.h
 *
 *	@author	Yuya Yaguchi <yuya.yaguchi@gree.net>
 */
#ifndef	TIME_WATHCER_OBSERVER_H
#define	TIME_WATHCER_OBSERVER_H

#include <stdint.h>
#include <sys/time.h>
#include "storage_access_info.h"
#include "storage_listener.h"

namespace gree {
namespace flare {

class storage_access_info;

class time_watcher_observer {
protected:
	static timeval           _threshold_warn;
	static timeval           _threshold_ping_ng;
	static storage_listener* _storage_listener;

public:
	static void on_storage_access_no_response(const timeval& difference, const storage_access_info& additional_info);
	static uint64_t register_on_storage_access_no_response_callback(storage_access_info info);
	static void unregister_on_storage_access_no_response_callback(uint64_t id);
	static void set_threshold_warn_msec(uint32_t msec);
	static void set_threshold_ping_ng_msec(uint32_t msec);
	static void set_storage_listener(storage_listener* listener);
};

}	// namespace flare
}	// namespace gree

#endif