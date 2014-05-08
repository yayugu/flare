#ifndef	HANDLER_TIME_WATCHER_POLLING
#define	HANDLER_TIME_WATCHER_POLLING

#include "thread_handler.h"
#include "time_watcher.h"
#include "time_watcher_listener.h"
#include "time_watcher_target_info.h"

namespace gree {
namespace flare {

template<class T>
class handler_time_watcher_polling : public thread_handler {
protected:
	time_watcher<T>& _time_watcher;
	time_watcher_listener<T>& _listener;
	timeval							_polling_interval;
	timeval							_threshold;

public:
	handler_time_watcher_polling(
			shared_thread thread,
			time_watcher<T>& time_watcher,
			time_watcher_listener<T>& listener,
			timeval polling_interval,
			timeval threshold
	);
	~handler_time_watcher_polling();
	virtual int run();

protected:
	void _check_timestamp(const time_watcher_target_info<T>& info);
	void _check_timestamps();
};

}	// namespace flare
}	// namespace gree

#endif
