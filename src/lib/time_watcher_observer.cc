#include "app.h"
#include "logger.h"
#include "time_util.h"
#include "time_watcher.h"
#include "time_watcher_observer.h"

namespace gree {
namespace flare {

timeval time_watcher_observer::_threshold_warn = {0, 0};
timeval time_watcher_observer::_threshold_ping_ng = {0, 0};
storage_listener* time_watcher_observer::_storage_listener = NULL;

void time_watcher_observer::on_storage_access_no_response(const timeval& difference, const storage_access_info& additional_info) {
	if (time_util::timer_is_bigger(difference, time_watcher_observer::_threshold_ping_ng)) {
		log_err(
			"thread (id: %lu, thread_id: %lu) running too long time: %u.%6u sec",
			additional_info.thread->get_id(),
			additional_info.thread->get_thread_id(),
			difference.tv_sec,
			difference.tv_usec
		);

		time_watcher_observer::_storage_listener->on_storage_error();
		return;
	}

	log_warning(
		"thread (id: %lu, thread_id: %lu) running too long time: %u.%6u sec",
		additional_info.thread->get_id(),
		additional_info.thread->get_thread_id(),
		difference.tv_sec,
		difference.tv_usec
	);
}

uint64_t time_watcher_observer::register_on_storage_access_no_response_callback(storage_access_info info) {
	return time_watcher_object->register_target(
		time_watcher_observer::_threshold_warn,
		boost::bind(&time_watcher_observer::on_storage_access_no_response, _1, info)
	);
}

void time_watcher_observer::unregister_on_storage_access_no_response_callback(uint64_t id) {
	time_watcher_object->unregister_target(id);
}

void time_watcher_observer::set_threshold_warn_msec(uint32_t msec) {
	time_watcher_observer::_threshold_warn = time_util::msec_to_timeval(msec);
}

void time_watcher_observer::set_threshold_ping_ng_msec(uint32_t msec) {
	time_watcher_observer::_threshold_ping_ng = time_util::msec_to_timeval(msec);
}

void time_watcher_observer::set_storage_listener(storage_listener* listener) {
	time_watcher_observer::_storage_listener = listener;
}

}	// namespace flare
}	// namespace gree
