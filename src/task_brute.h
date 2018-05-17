#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include "picosha2.h"
#include "task.h"
#include "ender.h"

class Task_brute : 	public Task
{
public:
	Task_brute(std::vector<std::uint_fast8_t> block, Ender* const p_ender)
		: block(block), p_ender(p_ender)
	{
	}
	void run();

private:
	std::vector<std::uint_fast8_t> block;
	Ender* const  p_ender;

	inline void brute_all_ends(std::vector<std::uint_fast8_t> block, Ender* const p_ender);
};
