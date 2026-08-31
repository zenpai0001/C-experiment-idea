#include <iostream>
#include <ncurses.h>
#include <panel.h>

#include "windowctrl.h"

namespace window_cpp {
void test() {
  global_ctrl.dispatch.handler();
  global_ctrl.dispatch.class_function().handler();
  global_ctrl.dispatch_ptr = {};
}

int main() {
  std::cout << "main.cpp created";
  return 0;
}
} // namespace window_cpp
