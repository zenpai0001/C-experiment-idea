#include <cstddef>
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/ftxui.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>

#include "threadpool.h"
#include "windowctrl.h" // Holds your inline hyper-lambdas

enum class Events { None, IncrementClick, ProcessTextInput, Terminate };

struct EventIntent {
  Events event_command = Events::None;
  size_t window_index = 0;
};

class TUI {
private:
  struct WindowComponent {
    std::vector<std::string> titles;
    std::vector<std::string> inputs;
    std::vector<int> click_counts;
    std::vector<bool> focus_states;
  };

public:
  ftxui::Element Render_Window(const WindowComponent &data, size_t index) {
    using namespace ftxui;
    return window(text(data.titles[index]),
                  vbox({text("Input: " + data.inputs[index]), separator(),
                        text("Clicks Recorded: " +
                             std::to_string(data.click_counts[index])),
                        separator(),
                        text(data.focus_states[index] ? "[ ACTIVE WINDOW ]"
                                                      : "[ BACKGROUND ]") |
                            dim}));
  }
  ftxui::Component
  Direct_Window_Pipe(const WindowComponent &data, size_t index,
                     std::vector<EventIntent> &global_window_ctx) {}
};
