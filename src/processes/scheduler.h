#ifndef SCHEDULERH
#define SCHEDULERH

#include "process.h"

#define NUM_QUEUES 3
#define BOOST_INTERVAL 100

extern const int time_slice[NUM_QUEUES];

process_t* scheduler_get_next();

#endif