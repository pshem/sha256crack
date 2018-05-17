#include "ender.h"

void Ender::set_result(std::string new_result) {
  result = new_result;
	farma.set_hash_found();
};

std::string Ender::get_result() {
  return result;
}
