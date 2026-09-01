#pragma once

// include the windowcrtl.h header file.
#include "windowctrl.h"

// include other stuff
#include <thread>

// Macro guards
#define THREADING
#ifndef THREADING

class thread_pool {

  void Byte_Table::run_parallel_pipeline() {
    std::thread workers[WORKER_COUNT]; // WORKER_COUNT is 4

    // 1. Kick off all 4 threads simultaneously
    for (uint32_t i = 0; i < WORKER_COUNT; ++i) {
      workers[i] =
          std::thread(&Byte_Table::execute_worker_threads, // The class method
                      this, // The specific instance pointer
                      i     // The thread_id argument
          );
    }

    // 2. Wait for all 4 execution lines to cross the finish line
    for (uint32_t i = 0; i < WORKER_COUNT; ++i) {
      if (workers[i].joinable()) {
        workers[i].join(); // Halts the main thread until the worker is done
      }
    }
  }
};
class async_fire {};

#endif
