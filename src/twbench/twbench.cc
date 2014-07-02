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
bool shutdowned = 0;
pthread_cond_t ready_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t start_count_mutex = PTHREAD_MUTEX_INITIALIZER;
bool ready_flag = 0;
int start_count = 0;

void callback(timeval tv, uint64_t dummy) {
	log_err("running too long time: %u.%6u sec.  %d", tv.tv_sec, tv.tv_usec, dummy);
}

void* run(void* param) {
	static int num_target = 2;
	timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	volatile uint64_t dummy = 1234;


	pthread_mutex_lock(&start_count_mutex);
	start_count++;
	pthread_cond_signal(&ready_cond);
	pthread_mutex_unlock(&start_count_mutex);

	while (ready_flag) {
		usleep(1 * 1000);
	}
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
			return 0;
		}
	}
	return 0;
}

void create_thread(int num_thread, int num_target_per_thread, vector<pthread_t>& threads) {
	//boost::function<void()> run2 = boost::bind<void, int>(&run, num_target_per_thread);
	for (int i = 0; i < num_thread; i++) {
		//boost::thread th(run2);
		//threads.push_back(th.native_handle());
		//th.detach();
		pthread_t tid;
		pthread_create(&tid, NULL, run, NULL);
		threads.push_back(tid);
	}
}

void join_thread(vector<pthread_t>& threads) {
	for (vector<pthread_t>::iterator it = threads.begin(); it != threads.end(); it++) {
		pthread_join(*it, NULL);
	}
}

void bench(int num_thread, int num_target_per_thread, int polling_exec_time) {
	tw = new time_watcher(1024);
	polling_count = 0;
	polling_time_us = 0;
	polling_map_count_sum = 0;
	shutdown_flag = false;
	pthread_cond_init(&ready_cond, NULL);
	pthread_mutex_init(&start_count_mutex, NULL);
	ready_flag = 0;
	start_count = 0;
	timeval start, end, sub;
	vector<pthread_t> threads;
	printf("thread initialize\n");

	pthread_mutex_lock(&start_count_mutex);
	create_thread(num_thread, num_target_per_thread, threads);
	while (start_count < num_thread) {
		pthread_cond_wait(&ready_cond, &start_count_mutex);
	}
	pthread_mutex_unlock(&start_count_mutex);
	ready_flag = 1;
	sleep(1);
	printf("start\n");
	gettimeofday(&start, NULL);
	tw->start(1);

	//tw->stop();
	shutdown_flag = true;
	while (!shutdowned) {
		usleep(100 * 1000); // 100ms
	}
	gettimeofday(&end, NULL);
	time_util::timer_sub(end, start, sub);
	join_thread(threads);
	printf("%d\t%d\t%ld.%06ld\n",
			num_thread,
			num_target_per_thread,
			sub.tv_sec, sub.tv_usec);

	sleep(1);
	delete tw;
}

int main(int argc, char **argv) {
	logger_singleton::instance().open("twbench", "local1");

	//int num_thread[] = {1, 10, 100};
	int num_thread[] = {100};
	//int num_target_per_thread[] = {1, 10, 100};
	int num_target_per_thread[] = {100};
	int polling_exec_time = 30;

	printf("numth\ttgt/th\ttime[s]\n");
	/*
	for (int i = 0; i < sizeof(num_thread) / sizeof(int); i++) {
		for (int j = 0; j < sizeof(num_target_per_thread) / sizeof(int); j++) {
			bench(num_thread[i], num_target_per_thread[j], polling_exec_time);
		}
	}
	*/
	bench(100, 2, polling_exec_time);
	bench(300, 2, polling_exec_time);
	bench(2000, 2, polling_exec_time);
	bench(5000, 2, polling_exec_time);
	bench(8000, 2, polling_exec_time);
	bench(10000, 2, polling_exec_time);
	bench(15000, 2, polling_exec_time);
	bench(20000, 2, polling_exec_time);

	logger_singleton::instance().close();
}
