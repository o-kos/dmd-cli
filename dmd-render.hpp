#pragma once

#include "dmd-state.hpp"
#include <ftxui/dom/elements.hpp>
//#include <queue>

namespace ftxui {
    Element phase();
    Element phase(const dmd::PhaseData &);
    Element phase(const dmd::PhasePoints &);
}

namespace dmd {

static auto make_doc(const State &state) {
    using namespace ftxui;
    static const unsigned pw = 21, ph = 9;
    Elements log_lines;
    const auto &lines = state.log.buffers;
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

    auto push_status = [](Elements &e, const std::wstring &title, const std::wstring &value, Color c) {
        e.push_back(hbox({
            text(L"·")          | color(c),
            text(title + L" ")  | color(Color::GrayDark),
            text(value)         | color(c)
        }));
        e.push_back(text(L" ⁞ ") | color(Color::GrayDark));
    };

    Elements params_line;
    for (const auto &p : state.params)
        push_status(params_line, p.first, p.second, Color::BlueLight);
    if (!params_line.empty()) params_line.pop_back();

    Elements results_line;
    auto push_result = [&](const std::wstring &title, int value) {
        if (value >= 0) push_status(results_line, title, std::to_wstring(value), Color::GreenLight);
    };
    push_result(L"bits",  state.status.bits);
    push_result(L"text",  state.status.text);
    push_result(L"phase", state.status.phase);
    results_line.pop_back();

    return
        vbox({
            hbox({
                vbox({
                    hbox({text(state.header.title + L" "), text(state.header.value) | color(Color::DarkGreen)}),
                    hbox({text(L"Params "), hbox(params_line)}),
                    hbox({
                        text(state.progress.prefix + L" "),
                        text(state.progress.percent()) | color(Color::CyanLight),
                        gauge(state.progress.value())  | color(Color::CyanLight) | flex,
                        text(state.progress.suffix)    | color(Color::GrayDark),
                    }),
                    vbox(log_lines) | size(HEIGHT, EQUAL, ph - 4),
                    hbox({text(L"Results "), hbox(results_line)}),
                }) | flex,
                phase(state.points.data) | color(Color::Magenta)
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
        Render(screen, dmd::make_doc(state));
        static std::string reset_position;
        if (reset_position.empty()) std::cout << "\x1B[?25l" << std::flush;
        std::cout << reset_position << screen.ToString() << std::flush;
        reset_position = screen.ResetPosition();
    }
};

}