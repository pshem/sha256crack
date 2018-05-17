#pragma once

#include "farm.fwd.h"

#include <mutex>
#include <vector>
#include "task.h"

#include <condition_variable>

#include <thread>
#include <chrono>
#include <atomic>

/** A collection of tasks that should be performed in parallel. */
class Farm {
public:

	Farm() : hash_found(false)
	{
		//Hopefully it never has to resize
		//kolejka.reserve(300);
	}

	/** Add a task to the farm.
	The task will be deleted once it has been run. */
	void add_task(Task *task);

	/** Run all the tasks in the farm.
	This method only returns once all the tasks in the farm
	have been completed. */
	void run();

	//notify the condition variable to spin up more threads if there is work for them
	void notify_cv();

	//get a pointer to the condition variable
	std::condition_variable* get_cv();

	//true if one of the tasks on the farm have found the correct hash
	bool get_hash_found();
	void set_hash_found();


private:
	std::vector<Task *> kolejka;
	std::mutex kolejka_mutex;
	inline bool accomplish_task();

	std::condition_variable cv;

	//true if one of the tasks on the farm have found the correct hash
	std::atomic<bool> hash_found;
};
