#pragma once
#ifndef WINDOW_CONTROLLER
#define WINDOW_CONTROLLER

#include <cstdint>
#include <immintrin.h>

enum class states : uint32_t { CREATE = 0x01, SWITCH = 0x02, DESTROY = 0xFF };

class WindowEngine {
private:
  struct int_ptrs *internal_matrix;

public:
  struct node {
    uint32_t state_flag = 0;
    void *ui_instance = nullptr;
  };
  WindowEngine();
  ~WindowEngine();

  void clear_vectorized();
};

class UIComponents {

public:
  UIComponents() = default;
};

class Dispatch_Table {
public:
  struct Func_Ptr {
    void (*jump[10])();
    Dispatch_Table *(*class_ptr)();
  };
  void handler() { arena_ctx arena; }

  Dispatch_Table class_function() { return Dispatch_Table{}; }

  struct arena_ctx {
    Func_Ptr func;
    int_ptrs *ctx;
  };
};
class Byte_Table {
public:
  // Struct with u32 pointer and independent u32 pointer array.
  struct int_ptrs {
    uint32_t (*u32_ptr)();

    // Use alignas to align storage buffer to 32. Set indices to nullptr upon
    // initialization.
    alignas(32) uint32_t *storage_buffer[1024] = {nullptr};
    size_t active_size = 0;

    // Fast SIMD Vectorized initialization to flash memory with nullptrs
    void clear_vectorized() {
      // Set up a 256-bit register completely filled with zeros
      __m256i zero_vector = _mm256_setzero_si256();

      // 1024 pointers / 8 pointers per register = 128 unrolled iterations
      // We step by 8 elements at a time!
      for (size_t i = 0; i < 1024; i += 8) {
        // Get the raw memory location casted to an AVX pointer type
        auto *target_address = reinterpret_cast<__m256i *>(&storage_buffer[i]);

        // Stream the zero register directly into the RAM slots simultaneously
        _mm256_store_si256(target_address, zero_vector);
      }
      active_size = 0;
    }
  };
  // Call the struct over to instantiate.
  int_ptrs slots;

  void push(uint32_t *ptr) {
    if (slots.active_size > 1024) {
      slots.storage_buffer[slots.active_size++] = ptr;
    }
  }

  enum class states : uint32_t { CREATE = 0x01, SWITCH = 0x02, DESTROY = 0xFF };
  uint32_t reset(uint32_t *ptr) {
    if (ptr != nullptr) {
      *ptr = 0;
    }
    return static_cast<uint32_t>(states::DESTROY);
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

#endif
