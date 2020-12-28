#include <chrono>
#include <iostream>
#include <thread>
#include <cmath>
#include <iomanip>

#include <ftxui/dom/elements.hpp>

#include "loren.h"

using namespace std;
using namespace chrono;
using namespace ftxui;

namespace dmd {

struct State {
    wstring header;

    struct Log {
        vector <string> buffer;

        void push(const string &line) {
            if (buffer.size() > 100) buffer.erase(buffer.begin());
            buffer.push_back(line);
        }
    } log;

    struct Progress {
        wstring prefix;
        unsigned position;
        unsigned limit;
        steady_clock::time_point start;

        Progress() : position{0}, limit{0}, start{steady_clock::now()} {}

        [[nodiscard]] auto value() const { return (limit) ? min(100.0f, float(position) / float(limit)) : 0; }

        [[nodiscard]] wstring percent() const {
            wchar_t buf[6];
            swprintf(buf, sizeof(buf), L"%3d%% ", lround(value() * 100));
            return buf;
        }

        [[nodiscard]] wstring suffix() const {
            auto to_ws = [](milliseconds ms) {
                seconds s = duration_cast<seconds>(ms);
                minutes m = duration_cast<minutes>(ms);
                hours   h = duration_cast<hours>(ms);
                wchar_t buf[6];
                swprintf(buf, sizeof(buf), L"%.2d:%.2d", m.count() % 60, s.count() % 60);
                wstring ts{buf};
                if (h.count() > 0) ts.insert(0, to_wstring(h.count()) + L':');
                return ts;
            };

            auto cur_ms = duration_cast<milliseconds>(steady_clock::now() - start);
            milliseconds eta_ms{lround(cur_ms.count() / value())};
            if (cur_ms > eta_ms) eta_ms = cur_ms;

            wstringstream ss;
            ss << fixed << std::setprecision(1) << position * 1000.0 / cur_ms.count();

            return L" " + to_ws(cur_ms) + L"<" + to_ws(eta_ms) + L", " + ss.str() + L"it/s ";
        }
    } progress;

    struct Status {
        wstring title;
        unsigned value;

        Status() : value{0} {}
    } status;
};

static auto makeDoc(const State &state) {
    return
        hbox({
            vbox({
                text(state.header),
                hbox({
                    text(state.progress.prefix),
                    text(state.progress.percent()),
                    gauge(state.progress.value()) | flex,
                    text(state.progress.suffix()),
                }),
                text(state.status.title),
                bgcolor(Color::GrayLight, text(L"Log data")) | flex,
            }) | flex,
            bgcolor(Color::GrayDark, text(L"+") | hcenter | vcenter)
                | size(WIDTH, EQUAL, 20) | size(HEIGHT, EQUAL, 9)
        });
}

}

int main(int /*argc*/, const char** /*argv[]*/) {
    using namespace chrono_literals;

    dmd::State state;

    state.header = L"Data processor";
    state.status.title = L"Initializing...";
    state.progress.prefix = L"Process ";
    state.progress.limit = 4 * 60 * 9600;

    auto d = Dimension::Full();
    if (d.dimx < 1) d.dimx = 80;
    d.dimy = 9;
    auto screen = Screen::Create(d);

    auto render = [&screen, &state]() {
        Render(screen, dmd::makeDoc(state));
        static string reset_position;
        cout << reset_position << screen.ToString() << flush;
        reset_position = screen.ResetPosition();
    };

    std::this_thread::sleep_for(200ms);
    render();

    for (;;) {
        state.progress.position += 9600 * 100 / 1000 * 4;
        state.status.value++;
//        state.log.push(nextLoren());
        render();

        if (state.progress.position >= state.progress.limit) break;
        std::this_thread::sleep_for(100ms);
    }
    cout << endl;
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


