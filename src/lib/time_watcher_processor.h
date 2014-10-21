/**
 *	time_watcher_processor.h
 *
 *	@author	Yuya Yaguchi <yuya.yaguchi@gree.net>
 */
#ifndef	TIME_WATHCER_PROCESSOR_H
#define	TIME_WATHCER_PROCESSOR_H

#include "time_watcher.h"
#include "time_watcher_target_info.h"


namespace gree {
namespace flare {

class time_watcher;

class time_watcher_processor {
protected:
	shared_ptr<time_watcher_processor> _shared_this;
	time_watcher&                      _time_watcher;
	timeval                            _polling_interval;
	bool                               _shutdown_requested;

public:
	time_watcher_processor(
		shared_ptr<time_watcher_processor> shared_this, // for memory management
		time_watcher& time_watcher,
		timeval polling_interval
	);
	~time_watcher_processor();
	void operator()(); // Callable
	void request_shutdown();

protected:
	void _check_timestamp(const time_watcher_target_info& info);
	void _check_timestamps();
};

}	// namespace flare
}	// namespace gree

#endif
