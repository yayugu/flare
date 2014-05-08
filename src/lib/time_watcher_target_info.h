#ifndef	TIME_WATHCER_TARGET_INFO_H
#define	TIME_WATHCER_TARGET_INFO_H

#include <time.h>

namespace gree {
namespace flare {

template<class T>
class time_watcher_target_info {
public:
	timeval timestamp;
	T additional_info;
};

}	// namespace flare
}	// namespace gree

#endif
