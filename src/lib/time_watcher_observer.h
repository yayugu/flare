#ifndef	TIME_WATHCER_OBSERVER
#define	TIME_WATHCER_OBSERVER

#include <stdint.h>
#include <time.h>
#include "storage_access_info.h"

namespace gree {
namespace flare {

class storage_access_info;

class time_watcher_observer {
public:
	static void on_storage_access_no_response(const timeval& difference, const storage_access_info& additional_info);
	static void register_on_storage_access_no_response_callback(uint32_t id, storage_access_info info);
	static void unregister_on_storage_access_no_response_callback(uint32_t id);
};

}	// namespace flare
}	// namespace gree

#endif
