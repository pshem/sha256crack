#include "task_create.h"


void Task_create::run()
{
	bruteforce(block, p_ender);
}


inline void Task_create::bruteforce(std::vector<std::uint_fast8_t> block, Ender* p_ender)
{
	if (p_ender->create_length > block.size()) {

		for (std::uint_fast8_t i = 32; i <= 126; ++i) {
			block.push_back(i);

			p_ender->farma.add_task(new Task_create(block, p_ender));
			p_ender->farma.notify_cv();

			block.pop_back();
		}

	}
	else {
		p_ender->farma.add_task(new Task_brute(block, p_ender));
	}
}
