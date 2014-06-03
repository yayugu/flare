#ifndef	TIME_WATHCER_TARGET_INFO_H
#define	TIME_WATHCER_TARGET_INFO_H

#include <sys/time.h>
#include <boost/function.hpp>

using namespace boost;

namespace gree {
namespace flare {

class time_watcher_target_info {
public:
	timeval timestamp;
	timeval threshold;
	function<void(timeval)> callback;
};

}	// namespace flare
}	// namespace gree

#endif
