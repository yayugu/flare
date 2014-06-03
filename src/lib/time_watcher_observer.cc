#include "app.h"
#include "logger.h"
#include "time_watcher.h"
#include "time_watcher_observer.h"

namespace gree {
namespace flare {

void time_watcher_observer::on_storage_access_no_response(const timeval& difference, const storage_access_info& additional_info) {
	log_err("hi", 0);
}

uint64_t time_watcher_observer::register_on_storage_access_no_response_callback(storage_access_info info) {
	timeval t;
	t.tv_sec = 1;
	t.tv_usec = 0;
	return time_watcher_object->register_target(
		t,
		boost::bind(&time_watcher_observer::on_storage_access_no_response, _1, info)
	);
}

void time_watcher_observer::unregister_on_storage_access_no_response_callback(uint64_t id) {
	time_watcher_object->unregister_target(id);
}

}	// namespace flare
}	// namespace gree
