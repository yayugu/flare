#include "time_watcher.h"
#include "time_watcher_processor.h"
#include "logger.h"
#include "time_util.h"

using namespace gree::flare;

int polling_count = 0;
int polling_time_us = 0;
int polling_map_count_sum = 0;
time_watcher* tw = NULL;

void callback(timeval tv, uint64_t dummy) {
	log_err("running too long time: %u.%6u sec.  %d", tv.tv_sec, tv.tv_usec, dummy);
}

void run(int num_target) {
	timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	volatile uint64_t dummy = 1234;
	for (;;) {
		vector<uint64_t> targets;
		for (int i = 0; i < num_target; i++) {
			targets.push_back(tw->register_target(tv, boost::bind(&callback, _1, dummy)));;
		}
		usleep(1 * 1000);
		for (vector<uint64_t>::iterator it = targets.begin(); it != targets.end(); it++) {
			tw->unregister_target(*it);
		}
	}
}

void create_thread(int num_thread) {
	boost::function<void()> run2 = boost::bind<void, int>(&run, 10);
	for (int i = 0; i < num_thread; i++) {
		boost::thread th(run2);
		th.detach();
	}
}

int main(int argc, char **argv) {
	logger_singleton::instance().open("twbench", "local1");

	tw = new time_watcher();
	create_thread(100);
	tw->start(1);

	sleep(5);

	tw->stop();
	usleep(5 * 1000 * 100); // 0.5 sec
	if (polling_count == 0) {
		printf("polling_count = 0. need to execute with more long time\n");
		exit(0);
	}
	printf("count:%d\ntime:%dms\n%lftimes/sec\navg map count:%d\n",
			polling_count,
			polling_time_us / 1000,
			(double)polling_count / ((double)polling_time_us / 1000 / 1000),
			polling_map_count_sum / polling_count);

	delete tw;
	logger_singleton::instance().close();
}
