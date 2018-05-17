#pragma once

#include <vector>
#include <cstdint>
#include "task.h"
#include "task_brute.h"
#include "ender.h"

class Task_create : public Task
{
public:
	Task_create(std::vector<std::uint_fast8_t> block, Ender* const p_ender)
		: block(block), p_ender(p_ender)
	{
	};
private:
	std::vector<std::uint_fast8_t> block;
	Ender* const p_ender;


	inline void bruteforce(std::vector<std::uint_fast8_t> block, Ender* const p_ender);

public:
	void run();
};
