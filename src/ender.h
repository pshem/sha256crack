#pragma once
#ifndef ender
#define ender

#include "ender.fwd.h"

#include <string>
#include "farm.h"

class Ender {
public:
  Ender(std::string wanted_hash, std::uint_fast8_t create_length): wanted_hash(wanted_hash), create_length(create_length)
  {
  };
  void set_result(std::string new_result);

  //return the private string
  std::string get_result();

  //TODO: try to use this instead
  Farm farma;

  //since they're constant, there is no point in a getter
  const std::string wanted_hash;
  const std::uint_fast8_t create_length;

private:
  std::string result = "";
};

#endif /* end of include guard: ender */
