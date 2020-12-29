#pragma once

#include <cmath>
#include <string>
#include <chrono>
#include <iostream>
#include <cmath>
#include <iomanip>

#include <ftxui/dom/elements.hpp>

namespace dmd {

struct State {
    static std::wstring to_ws(std::chrono::milliseconds ms) {
        using namespace std::chrono;
        seconds s = duration_cast<seconds>(ms);
        minutes m = duration_cast<minutes>(ms);
        hours   h = duration_cast<hours>(ms);
        wchar_t buf[6];
        swprintf(buf, sizeof(buf), L"%.2d:%.2d", m.count() % 60, s.count() % 60);
        std::wstring ts{buf};
        if (h.count() > 0) ts.insert(0, std::to_wstring(h.count()) + L':');
        return ts;
    };

    std::vector<std::pair<std::wstring, std::wstring>> params;

    struct {
        std::wstring title;
        std::wstring value;
    } header;

    struct Log {
        std::vector<std::pair<std::wstring, std::wstring>> buffer;

        void push(std::chrono::milliseconds cur_ms, const std::wstring &line) {
            auto timestamp = to_ws(cur_ms) + L' ';
            if (buffer.size() > 100) buffer.erase(buffer.begin());
            buffer.emplace_back(timestamp, line);
        }
    } log;

    struct Progress {
        std::wstring prefix;
        unsigned position;
        unsigned limit;
        std::wstring suffix;

        Progress() : position{0}, limit{0} {}

        [[nodiscard]] auto value() const { return (limit) ? std::min(100.0f, float(position) / float(limit)) : 0; }

        [[nodiscard]] std::wstring percent() const {
            wchar_t buf[6];
            swprintf(buf, sizeof(buf), L"%3d%% ", std::lround(value() * 100));
            return buf;
        }

        void update(std::chrono::milliseconds cur_ms, unsigned shift) {
            using namespace std::chrono;
            position += shift;
            milliseconds eta_ms{std::lround(cur_ms.count() / value())};
            if (cur_ms > eta_ms) eta_ms = cur_ms;
            std::wstringstream ss;
            ss << std::fixed << std::setprecision(1) << position * 1000.0 / cur_ms.count();

            suffix = L" " + to_ws(cur_ms) + L"<" + to_ws(eta_ms) + L", " + ss.str() + L"it/s ";
        }
    } progress;

    struct Status {
        std::wstring title;
        unsigned value;

        Status() : value{0} {}
    } status;

    std::chrono::steady_clock::time_point start;

    State() : start{std::chrono::steady_clock::now()} {}

    void push_progress(unsigned shift, const std::wstring &log_line) {
        using namespace std::chrono;
        auto cur_ms = duration_cast<milliseconds>(steady_clock::now() - start);
        log.push(cur_ms, log_line);
        progress.update(cur_ms, shift);
    }
};

static auto makeDoc(const State &state) {
    using namespace ftxui;
    static const unsigned pw = 20, ph = 9;
    Elements log_lines;
    const auto &lines = state.log.buffer;
    for (auto it = lines.cend() - std::min<unsigned>(lines.size(), ph - 4); it < lines.end(); ++it) {
        auto line = *it;
//TODO Add tripoints for long lines
//        for (int x = 0; x < std::min<unsigned>(line.size(), maxWidth); ++x)
//            image(x, y).c = line[x];
//        if (line.size() > maxWidth) {
//            image(maxWidth - 1, y).c = '…';
//        }
        log_lines.push_back(hbox({
            color(Color::GrayDark, text(line.first)),
            color(Color::Cyan, text(line.second))
        }));
    }

    Elements params_line;
    for (const auto &p : state.params) {
        params_line.push_back(hbox({
           color(Color::GrayDark, text(p.first + L' ')),
           color(Color::Cyan, text(p.second + L"  "))
        }));
    }

    return
        vbox({
            hbox({
                vbox({
                    hbox({text(state.header.title), color(Color::DarkGreen, text(state.header.value))}),
                    hbox(params_line),
                    hbox({
                        text(state.progress.prefix),
                        color(Color::CyanLight, text(state.progress.percent())),
                        color(Color::CyanLight, gauge(state.progress.value())) | flex,
                        color(Color::GrayDark, text(state.progress.suffix)),
                    }),
                    vbox(log_lines) | size(HEIGHT, EQUAL, ph - 4),
                    text(state.status.title),
                }) | flex,
                bgcolor(Color::GrayDark, text(L"+") | hcenter | vcenter)
                    | size(WIDTH, EQUAL, 20) | size(HEIGHT, EQUAL, ph)
            }),
        });
}

class StateRender {
public:
    StateRender() = default;
    virtual ~StateRender() = default;
    virtual void render(const State &state) const = 0;
};

class TuiRender : public StateRender {
public:
    TuiRender() : StateRender() {
    }

    ~TuiRender() override {
        std::cout << "\x1B[?25h" << std::flush;
    }

    void render(const State &state) const override {
        using namespace ftxui;
        static Dimension d{-1, -1};
        if (d.dimx == -1) {
            d = Dimension::Full();
            d.dimy = 9 + 1;
        }
        if (d.dimx < 1) d.dimx = 80;
        auto screen = Screen::Create(d);
        Render(screen, dmd::makeDoc(state));
        static std::string reset_position;
        if (reset_position.empty()) std::cout << "\x1B[?25l" << std::flush;
        std::cout << reset_position << screen.ToString() << std::flush;
        reset_position = screen.ResetPosition();
    }
};

}