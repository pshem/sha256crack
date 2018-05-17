#include "task_brute.h"

void Task_brute::run()
{
	brute_all_ends(block, p_ender);
}


inline void Task_brute::brute_all_ends(std::vector<std::uint_fast8_t> block, Ender* const p_ender)
{
	const std::string wanted_hash = p_ender->wanted_hash;
	for (std::uint_fast8_t i = 32; i <= 126; ++i) {
		block.push_back(i);
		std::string hash = picosha2::hash256_hex_string(block.begin(), block.end());
		if (hash == wanted_hash) {
			p_ender->set_result(std::string(block.begin(), block.end()));
		}
		block.pop_back();
	}
}
