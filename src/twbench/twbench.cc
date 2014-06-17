#include "time_watcher.h"
#include "time_watcher_processor.h"
#include "logger.h"
#include "time_util.h"

using namespace gree::flare;

uint64_t polling_count = 0;
uint64_t polling_time_us = 0;
uint64_t polling_map_count_sum = 0;
time_watcher* tw = NULL;
bool shutdown_flag = 0;

void callback(timeval tv, uint64_t dummy) {
	log_err("running too long time: %u.%6u sec.  %d", tv.tv_sec, tv.tv_usec, dummy);
}

void run(int num_target) {
	timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	volatile uint64_t dummy = 1234;
	for (;;) {
		usleep(2 * 1000);
		vector<uint64_t> targets;
		for (int i = 0; i < num_target; i++) {
			targets.push_back(tw->register_target(tv, boost::bind(&callback, _1, dummy)));;
		}
		usleep(8 * 1000);
		for (vector<uint64_t>::iterator it = targets.begin(); it != targets.end(); it++) {
			tw->unregister_target(*it);
		}
		if (shutdown_flag) {
			return;
		}
	}
}

void create_thread(int num_thread, int num_target_per_thread, vector<pthread_t>& threads) {
	boost::function<void()> run2 = boost::bind<void, int>(&run, num_target_per_thread);
	for (int i = 0; i < num_thread; i++) {
		boost::thread th(run2);
		threads.push_back(th.native_handle());
		th.detach();
	}
}

void bench(int num_thread, int num_target_per_thread, int polling_exec_time) {
	tw = new time_watcher(1024);
	polling_count = 0;
	polling_time_us = 0;
	polling_map_count_sum = 0;
	shutdown_flag = false;
	vector<pthread_t> threads;
	create_thread(num_thread, num_target_per_thread, threads);
	tw->start(1);

	sleep(polling_exec_time);

	tw->stop();
	shutdown_flag = true;
	//usleep(5 * 1000 * 100); // 0.5 sec
	sleep(5);
	if (polling_count == 0) {
		printf("%d\t%d\n", num_thread, num_target_per_thread);
		printf("polling_count = 0. need to execute with more long time\n");
		return;
	}
	printf("%d\t%d\t%lu\t%lu\t%lf\t%lf\t%lu\t%lu\n",
			num_thread,
			num_target_per_thread,
			polling_count,
			polling_time_us / 1000,
			(double)polling_time_us / 1000 / (double)polling_count,
			(double)polling_time_us / 1000.0 / 1000.0 / (double)polling_exec_time * 100.0,
			polling_map_count_sum / polling_count,
			polling_map_count_sum);

	delete tw;
}

int main(int argc, char **argv) {
	logger_singleton::instance().open("twbench", "local1");

	//int num_thread[] = {1, 10, 100};
	int num_thread[] = {100};
	//int num_target_per_thread[] = {1, 10, 100};
	int num_target_per_thread[] = {100};
	int polling_exec_time = 30;

	printf("numth\ttgt/th\tcount\ttime[ms]\tavg polling time[ms]\tpolling exexution time percentage\tavg map count\n");
	for (int i = 0; i < sizeof(num_thread) / sizeof(int); i++) {
		for (int j = 0; j < sizeof(num_target_per_thread) / sizeof(int); j++) {
			bench(num_thread[i], num_target_per_thread[j], polling_exec_time);
		}
	}
	/*
	bench(100, 300, polling_exec_time);
	bench(300, 100, polling_exec_time);
	bench(8000, 5, polling_exec_time);
	*/

	logger_singleton::instance().close();
}
