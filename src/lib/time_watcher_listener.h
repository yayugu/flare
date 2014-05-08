#ifndef	TIME_WATHCER_LISTENER_H
#define	TIME_WATHCER_LISTENER_H

namespace gree {
namespace flare {

template<class T>
class time_watcher_listener {
public:
	virtual void time_watcher_on_over_threshold(const timeval& difference, const T& additional_info) = 0;
};

}	// namespace flare
}	// namespace gree

#endif
