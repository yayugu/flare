#ifndef	HANDLER_TIME_WATCHER_POLLING
#define	HANDLER_TIME_WATCHER_POLLING

#include "thread_handler.h"
#include "time_watcher.h"
#include "time_watcher_listener.h"
#include "time_watcher_target_info.h"

namespace gree {
namespace flare {

class handler_time_watcher_polling : public thread_handler {
protected:
	time_watcher& _time_watcher;
	timeval							_polling_interval;

public:
	handler_time_watcher_polling(
			shared_thread thread,
			time_watcher& time_watcher,
			timeval polling_interval
	);
	~handler_time_watcher_polling();
	virtual int run();

protected:
	void _check_timestamp(const time_watcher_target_info& info);
	void _check_timestamps();
};

}	// namespace flare
}	// namespace gree

#endif
