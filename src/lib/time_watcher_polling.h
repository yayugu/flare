#ifndef	TIME_WATHCER_POLLING_THREAD_H
#define	TIME_WATHCER_POLLING_THREAD_H

#include "time_watcher.h"
#include "time_watcher_target_info.h"


namespace gree {
namespace flare {

class time_watcher;

class time_watcher_polling {
protected:
	time_watcher& _time_watcher;
	timeval				_polling_interval;
	bool					_shutdown_requested;

public:
	time_watcher_polling(
			time_watcher& time_watcher,
			timeval polling_interval
	);
	~time_watcher_polling();
	void operator()(); // Callable
	void request_shutdown();

protected:
	void _check_timestamp(const time_watcher_target_info& info);
	void _check_timestamps();
};

}	// namespace flare
}	// namespace gree

#endif
