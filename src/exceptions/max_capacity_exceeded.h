#pragma once

#include <stdexcept>
class MaxCapacityExceededException : public std::runtime_error {
  public:
    MaxCapacityExceededException(const std::string& message)
        : std::runtime_error{message} {}
};

