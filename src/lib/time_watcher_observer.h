#ifndef	TIME_WATHCER_OBSERVER
#define	TIME_WATHCER_OBSERVER

#include <stdint.h>
#include <sys/time.h>
#include "storage_access_info.h"

namespace gree {
namespace flare {

class storage_access_info;

class time_watcher_observer {
public:
	static void on_storage_access_no_response(const timeval& difference, const storage_access_info& additional_info);
	static uint64_t register_on_storage_access_no_response_callback(storage_access_info info);
	static void unregister_on_storage_access_no_response_callback(uint64_t id);
};

}	// namespace flare
}	// namespace gree

#endif
