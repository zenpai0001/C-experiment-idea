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
                      this, r i);
    }

    // 2. Wait for all 4 execution lines to cross the finish line
    for (uint32_t i = 0; i < WORKER_COUNT; ++i) {
      if (workers[i].joinable()) {
        workers[i].join(); // Halts the main thread until the worker is done
      }
    }
  }
};
enum struct Event_Dispatch : u32 { IDLE, RENDER, HALT };

template <Event_Dispatch EventType>
[[nodiscard("WARNING: Wrong thread setup causes race conditions, dude")]]
class async_fire {
private:
  // Function pointer layout
  using ActionFunc = void (*)(void *context, s32 state_tracker);

  // Set these to nullptr to initialize them.
  static inline ActionFunc active_callback = nullptr;
  static inline void *context_pointer = nullptr;

public:
  static inline void subscribe(ActionFunc callback, void *ctx) {
    active_callback = callback;
    context_pointer = ctx;
  }
  static inline void broadcast(s32 state_tracker) {
    if (active_callback) {
      active_callback(context_pointer, state_tracker);
    }
  }
  static inline void async(s32 state_tracker) {
    // Reference the byte_stream object from global_ctrl struct.
    auto &byte_stream = global_ctrl.byte_stream;

    // Schedule the specific event context across 4 threads.
    for (u32 thread_id = 0; thread_id < WORKER_COUNT; ++thread_id) {

      u32 *worker_data = byte_stream.get_worker_chunk_address(thread_id);
      u32 *scratch_pad = byte_stream.get_scratch_buffer_address(thread_id);
    }
    EventStation<EventType>::broadcast(state_tracker);
  }
};

struct async_handle {
  async_fire async_fire;
  thread_pool t_pool;
};

extern async_handle async_table;

#endif
