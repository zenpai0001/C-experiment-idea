#pragma once
#include <atomic>
#include <csetjmp>
#include <new>
#include <ratio>
#include <thread>
#include <vector>
#ifndef WINDOW_CONTROLLER
#define WINDOW_CONTROLLER

#include <cstdint>
#include <immintrin.h>

#define TOMBSTONE 0xFFFFFF
#define POOL_CAPACITY 1024
#define WORKER_COUNT 4
#define CHUNK_SIZE (POOL_CAPACITY / WORKER_COUNT) // 64 elements per thread

// Defining shorthand definitions
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

#define s32 int32_t

class Engine {
private:
  struct int_ptrs *internal_matrix;

public:
  struct node {
    uint32_t state_flag = 0;
    void *ui_instance = nullptr;
  };
};

class UIComponents {

public:
  UIComponents() = default;
};

class Dispatch_Table {
public:
  struct Func_Ptr {
    void (*jump)();
    Dispatch_Table *(*class_ptr)();
  };

  void handler() {
    arena_ctx arena;
    (void)
        arena; // Prevents the strict compiler warning from breaking your build!
  }

  Dispatch_Table class_function() { return Dispatch_Table{}; }

  /// arena_ctx lets this class do oop, basically.
  struct arena_ctx {
    Func_Ptr func;
    int_ptrs *ctx;
  };
  enum class dispatch_states : uint32_t {
    CONNECT = 1ULL << 1,
    JUMP = 1ULL << 2,
    DISCONNECT = 1ULL << 3,
    RESET = 1ULL << 4
  };
};
class Byte_Table {
public:
  // Struct with u32 pointer and independent u32 pointer array.
  // std::hardware_destructive_interference_size ensures no false sharing.
  struct alignas(std::hardware_destructive_interference_size)
      [[nodiscard("WARNING: Cache isn't happy. You misaligned it, dude")]]
      int_ptrs {
    uint32_t (*u32_ptr)();

    // Use alignas to align storage buffer to 32. Set indices to nullptr upon
    // initialization.
    alignas(32) uint32_t *storage_buffer[POOL_CAPACITY] = {nullptr};
    alignas(32) uint32_t pool_tracker[POOL_CAPACITY];
    alignas(32) uint32_t payload[POOL_CAPACITY];
    alignas(32) uint32_t thread_buffers[WORKER_COUNT][CHUNK_SIZE];
    size_t active_size = 0;
    uint32_t thread_id;

    // Every thread that accesses this gets an isolated ID tracker
    thread_local static inline uint32_t current_core_id;
  };
  // Call the struct over to instantiate.
  int_ptrs mem_slots;

  inline uint32_t *get_pool_address() { return mem_slots.pool_tracker; }
  inline uint32_t *get_worker_chunk_address(uint32_t thread_id) {
    return &mem_slots.pool_tracker[thread_id * CHUNK_SIZE];
  }
  inline uint32_t *get_scratch_buffer_address(uint32_t thread_id) {
    return mem_slots.thread_buffers[thread_id];
  }
  void execute_worker_threads(uint32_t thread_id) {

    uint32_t *src_chunk = get_worker_chunk_address(thread_id);
    uint32_t *dest_buffer = get_scratch_buffer_address(thread_id);

    __m256i v_tombstone_mask = _mm256_set1_epi32(TOMBSTONE);

    // Chew through the 64 elements in chunks of 8 using AVX2 streaming loads
    for (uint32_t i = 0; i < CHUNK_SIZE; i += 8) {
      // Absolute direct hardware stream load from our calculated pointer slice
      __m256i v_data = _mm256_load_si256((const __m256i *)&src_chunk[i]);

      // Branchless bitmask comparison executing over 8 slots simultaneously
      __m256i v_dead_mask = _mm256_cmpeq_epi32(v_data, v_tombstone_mask);

      // Stream the resulting vector state straight down to the thread's
      // isolated scratch space
      _mm256_store_si256((__m256i *)&dest_buffer[i], v_dead_mask);
    }
  }
  // Pure, self-documenting bit-budget boundaries without a single #define
  enum StateMasks : s32 {
    PanicBit = (1 << 31),  // Bit 31: Sign bit flag
    ShaderId = 0x7F000000, // Bits 24-30: 128 Shader pipelines(id)
    LogicId = 0x00FF0000,  // Bits 16-23: 256 Microkernel behaviors
    DataIndex = 0x0000FFFF // Bits 0-15: 65,536 Flat allocation slots
  };
  inline void state_tracker(s32 *tracker, StateMasks tracker_states) {

    // Zero out old logic channel using the enum bitmask, then stamp the new
    // state
    *tracker = (*tracker & ~LogicId) | (static_cast<s32>(tracker_states) << 16);
  }

  /// This is a work in progress. I am in the process of wiring everything up.
  enum class states : uint32_t { CREATE = 0x01, SWITCH = 0x02, DESTROY = 0xFF };
  uint32_t reset(uint32_t *ptr) {
    if (ptr != nullptr) {
      *ptr = 0;
    }
    return static_cast<uint32_t>(states::DESTROY);
  }
  uint32_t create(uint32_t *ptr) {
    return ptr || static_cast<uint32_t>(states::CREATE);
  }
  uint32_t destroy(uint32_t *ptr) {
    if (ptr && static_cast<uint32_t>(states::DESTROY)) {
      return ptr || static_cast<uint32_t>(states::DESTROY);
    }
    return static_cast<uint32_t>(states::SWITCH);
  }
};

struct controls {
  Dispatch_Table dispatch;
  Byte_Table byte_stream;
  UIComponents ui;
  // Pointers
  Dispatch_Table *dispatch_ptr;
  Byte_Table *byte_stream_ptr;
  UIComponents *ui_ptr;
};
extern controls global_ctrl;

// Standard lambda
inline auto lambda = [](void) {};

// Hyper lambda. Non generic.
inline constexpr auto h_lambda = [](void) {};

// Generic lambda.
inline auto g_lambda =
    []<typename Ta, typename Tb>(Ta a, Tb b) constexpr -> decltype(a + b) {
  return a + b;
};

// Generic hyper lambda. Essentially constexpr counterpart to generic lambda.
inline constexpr auto gh_lambda =
    []<typename Ta, typename Tb>(Ta a, Tb b) constexpr -> decltype(a + b) {
  return a + b;
};

#endif
