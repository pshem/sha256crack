#include "farm.h"

void Farm::add_task(Task *task)
{
	kolejka_mutex.lock();
	kolejka.push_back(task);
	kolejka_mutex.unlock();
}

void Farm::run()
{
	const std::uint_fast8_t num_threads = std::thread::hardware_concurrency();

	std::vector<std::thread *> threads;

	for (unsigned int i = 0; i < num_threads; ++i) {
		//create a thread
		threads.push_back(new std::thread([this] {  //Why doesn't it work with kolejka, kolejka_mutex?
			while (accomplish_task()) {
			}})
		);

		//do not create more threads unti there are tasks for them
		//if one second passes before that, threads are unnecessary
		{
			std::mutex m;
			std::unique_lock<std::mutex> lk(m);
			//get the control variable(now) from ender
			get_cv()->wait_for(lk,std::chrono::milliseconds(100));
		}
	}
	//be collection agnostic
	for (const auto& thr : threads) {
		thr->join();
	}
}

inline bool Farm::accomplish_task() {
	Task *task;
	if (!get_hash_found()) {
		kolejka_mutex.lock();
		if (!kolejka.empty()) {
			task = kolejka.back();
			kolejka.pop_back();
			kolejka_mutex.unlock();
			task->run();
			delete task;
			return true;
		}
		kolejka_mutex.unlock();
	}
	//wall.Wait();
	return false;
}

void Farm::notify_cv() {
	cv.notify_all();
}

std::condition_variable* Farm::get_cv() {
	return &cv;
}

bool Farm::get_hash_found() {
	return hash_found;
}

void Farm::set_hash_found() {
	//[possibly empty the queue instead
	hash_found = true;
}
