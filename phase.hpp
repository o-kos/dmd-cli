#pragma once

#include <ftxui/screen/color.hpp>

#include <array>
#include <chrono>

namespace ftxui::phase_cfg {

using namespace std::chrono_literals;

static const int width  = 21;
static const int height =  9;
static const std::array<wchar_t, 4> symbols{L'\x2800', L'\x2500', L'\x2502', L'\x253C'}; // "⠀─│┼"
static constexpr std::chrono::milliseconds fade_time{250ms};

static auto &palette() {
    using namespace ftxui;
    static const std::array<Color, 3> colors{
        Color::YellowLight, Color::Yellow, Color::GrayDark
    };
    return colors;
};

}