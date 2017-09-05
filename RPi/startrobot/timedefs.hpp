// Time typdefs to make dealing with chrono simpler
#pragma once
#include <chrono>

using namespace std::chrono;\

typedef tduration duration<double>;
typedef ttime_point system_clock::timepoint;
