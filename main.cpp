#include <chrono>
#include <thread>

#include "loren.hpp"
#include "dmd-render.hpp"

using namespace std;
using namespace chrono;

int main(int /*argc*/, const char** /*argv[]*/) {
    using namespace chrono_literals;

    dmd::State state;

    state.header.title = L"Processing file";
    state.header.value = L"~/signals/psk8-wav\x2026";
    state.progress.prefix = L"Process";
    state.progress.limit = 4 * 60 * 9600;
    state.params = {{L"freq", L"1800"}, {L"mode", L"PSK-8"}, {L"speed", L"1200"}, {L"interleaver", L"long"}};
    state.status.text = -1;

    auto r = make_unique<dmd::TuiRender>();
    std::this_thread::sleep_for(200ms);
    r->render(state);

    const unsigned speed = 32;
    for (;;) {
        state.push_progress(9600 * 100 / 1000 * speed, nextLoren());
        state.status.bits += 123;
        state.status.phase += 324;
        r->render(state);

        if (state.progress.position >= state.progress.limit) break;
        std::this_thread::sleep_for(100ms);
    }
}



//#include <chrono>
//#include <cmath>
//#include <ftxui/dom/elements.hpp>
//#include <ftxui/screen/screen.hpp>
//#include <ftxui/screen/string.hpp>
//#include <iostream>
//#include <thread>
//
//class Graph {
//public:
//    std::vector<int> operator()(int width, int height) {
//        std::vector<int> output(width);
//        for (int i = 0; i < width; ++i) {
//            float v = 0;
//            v += 0.1f * sin((i + shift) * 0.1f);
//            v += 0.2f * sin((i + shift + 10) * 0.15f);
//            v += 0.1f * sin((i + shift) * 0.03f);
//            v *= height;
//            v += 0.5f * height;
//            output[i] = static_cast<int>(v);
//        }
//        return output;
//    }
//    int shift = 0;
//};
//
//std::vector<int> triangle(int width, int height) {
//    std::vector<int> output(width);
//    for (int i = 0; i < width; ++i) {
//        output[i] = i % (height - 4) + 2;
//    }
//    return output;
//}
//
//int main(int argc, const char* argv[]) {
//    using namespace ftxui;
//    using namespace std::chrono_literals;
//
//    Graph my_graph;
//
//    std::string reset_position;
//    for (int i = 0;; ++i) {
//        auto document =
//                hbox({
//                             vbox({
//                                          graph(std::ref(my_graph)),
//                                          separator(),
//                                          graph(triangle) | inverted,
//                                  }) | flex,
//                             separator(),
//                             vbox({
//                                          graph(std::ref(my_graph)) | color(Color::BlueLight),
//                                          separator(),
//                                          graph(std::ref(my_graph)) | color(Color::RedLight),
//                                          separator(),
//                                          graph(std::ref(my_graph)) | color(Color::YellowLight),
//                                  }) | flex,
//                     }) |
//                border | size(HEIGHT, GREATER_THAN, 40);
//
//        auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
//        Render(screen, document);
//        std::cout << reset_position << screen.ToString() << std::flush;
//
//        reset_position = screen.ResetPosition();
//
//        std::this_thread::sleep_for(0.03s);
//        my_graph.shift++;
//    }
//
//    return 0;
//}


