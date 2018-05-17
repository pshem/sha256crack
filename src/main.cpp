#include <iostream>
#include <cstdint>
#include <cassert>
#include <cstring>
#include <cctype>
#include "picosha2.h"

//for benchmarking
#include <chrono>

//for alternative threading - compilation error
#include <omp.h>

//for threadfarm
#include <thread>
#include <mutex>
#include "task_brute.h"
#include "task_create.h"
#include "farm.h"

// Import things we need from the standard library
using std::chrono::duration_cast;
using std::chrono::milliseconds;

// Define the alias "the_clock" for the clock type we're going to use.
typedef std::chrono::steady_clock the_clock;

//attempts all byte combinations for a given length checking if they match wanted_string
//starting string defaults to empty string - C++11 required
std::string bruteforce(std::vector<std::uint_fast8_t> starting_string, const std::string wanted_hash, const std::uint_fast8_t tested_length);
inline std::string bruteforce(const std::string wanted_hash, const std::uint_fast8_t tested_length) { return bruteforce({}, wanted_hash, tested_length); }

std::string thr_bruteforce(std::vector<std::uint_fast8_t> block, const std::string wanted_hash, const std::uint_fast8_t tested_length);
inline std::string thr_bruteforce(const std::string wanted_hash, const std::uint_fast8_t tested_length) { return thr_bruteforce({}, wanted_hash, tested_length); }

std::string omp_bruteforce(std::vector<std::uint_fast8_t> block, const std::string wanted_hash, const std::uint_fast8_t tested_length);
inline std::string omp_bruteforce(const std::string wanted_hash, const std::uint_fast8_t tested_length) { return omp_bruteforce({}, wanted_hash, tested_length); }

std::string omp_in_bruteforce(std::vector<std::uint_fast8_t> block, const std::string wanted_hash, const std::uint_fast8_t tested_length);

inline std::string sha256sum(std::string message) { return picosha2::hash256_hex_string(message); }

//single threaded main
int main(int argc, char* argv[]) {
	std::string hash;
	std::string result;
	std::uint_fast8_t max_tested_length = 5;

	const std::string hash_me = "xyz";

	switch (argc) {
	case 1:
		std::cout << "No input string found, running test using "<< hash_me << ".\n";
		hash = sha256sum(hash_me);
		break;
	case 3:
		max_tested_length = std::atoi(argv[2]);
	case 2:
		//check if the passed hash looks like sha256
		if(std::strlen(argv[1]) != 64) {
			std::cout << argv[1] <<" is not a sha256 hash\n";
			return 1;
		}
		hash = argv[1];
		
		//validate hash
		for (std::uint_fast8_t i = 0; i < 64; ++i) {
			if (std::isalnum(hash[i])) {
				hash[i] = std::tolower(hash[i]);
			} else {
				std::cout << argv[1] <<" is not a sha256 hash\n";
				return 4;
			}
		}
		break;
	default:
		std::cout << "Too many things in the command line." << std::endl;
		return(3);
	}

	// Start timing
	the_clock::time_point start = the_clock::now();

	//control the number of characters
	std::uint_fast8_t counter = 1;

	//inform the user
	std::cout << "Attempting to bruteforce :\n" << hash << "\n";


	std::string message = "";
	//check the special case so that we can use it as a "not found" later
	if (sha256sum("") == hash) {
		std::cout << "The passed hash is for an empty string" << std::endl;
		return 0;
	}
	else {
		//check strings of increasing length
		while (message == "" && counter <= max_tested_length) {
			std::cout << "Testing " << (int)counter << " char long combinations" << std::endl;
			message = thr_bruteforce(hash, counter);
			++counter;
		}
	}

	// Stop timing
	the_clock::time_point end = the_clock::now();

	// Compute the difference between the two times in milliseconds
	auto time_taken = duration_cast<milliseconds>(end - start).count();
	std::cout << "Bruteforcing the hash took " << time_taken << " ms." << std::endl;

	if (message == "") {
		std::cout << "Failed to reverse hash after hashing inputs of up to " << counter << " characters" << std::endl;
		return 2;
	} else {
		std::cout << "The passed hash is for \"" << message << "\"." << std::endl;
	}
	return 0;
} //end main

std::string bruteforce(std::vector<std::uint_fast8_t> block, const std::string wanted_hash, const std::uint_fast8_t tested_length)
{
	std::string result = "";
	if (tested_length >  block.size()) {

		for (std::uint_fast8_t i = 32; i < 126; ++i) {
			block.push_back(i);

			const std::string result = bruteforce(block, wanted_hash, tested_length);

			if (result != "") {
				return result;
			}
			block.pop_back();
		}
	}
	else  //their length is the same
	{
		std::string hash = picosha2::hash256_hex_string(block.begin(), block.end());

		if (hash == wanted_hash) {
			result = std::string(block.begin(), block.end());
		}
	}
	return result;
} //end bruteforce

std::string thr_bruteforce(std::vector<std::uint_fast8_t> block, const std::string wanted_hash, const std::uint_fast8_t tested_length) {

	//one pointer to rule them all (aka. store static-ish variables)
	Ender* const p_ender = new Ender(wanted_hash, tested_length-1);

	//give the farm some work
	p_ender->farma.add_task(new Task_create(block, p_ender));

	//start the farm
	p_ender->farma.run();

	std::string result=p_ender->get_result();

	delete p_ender;

	return result;
} //end thr_bruteforce

std::string omp_bruteforce(std::vector<std::uint_fast8_t> block, const std::string wanted_hash, const std::uint_fast8_t tested_length)
{
	if (tested_length-1 >  block.size()) {
		std::string result;
		for (std::uint_fast8_t i = 32; i < 126; ++i) {
			block.push_back(i);
			result = omp_bruteforce(block, wanted_hash, tested_length);
			if (result != "") {
				return result;
			}
			block.pop_back();
		}
		//nothing was found
		return "";
	}
	else  //their length is the same
	{
		return omp_in_bruteforce(block, wanted_hash, tested_length);
	}
	assert(0);
} //end omp_bruteforce

std::string omp_in_bruteforce(std::vector<std::uint_fast8_t> block, const std::string wanted_hash, const std::uint_fast8_t tested_length)
{
	std::string result = "";
	//ints instead of usual uints so VS compiles
#pragma omp parallel for firstprivate(block) shared(result)
	for (std::int_fast8_t i = 32; i < 126; ++i) {
		block.push_back(i);
		//std::cout << std::string(block.begin(), block.end()) << std::endl;
		std::string hash = picosha2::hash256_hex_string(block.begin(), block.end());
		if (hash == wanted_hash) {
			//as hashes are unique, only a single thread can ever get here
			result = std::string(block.begin(), block.end());
		}
		block.pop_back();
	}
	return result;
} //end omp_in_bruteforce
