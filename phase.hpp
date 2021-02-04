#pragma once

#include <ftxui/screen/color.hpp>

#include <array>
#include <chrono>

namespace ftxui::phase_cfg {

using namespace std::chrono_literals;

static const int width  = 21;
static const int height =  9;
static const std::array<wchar_t, 4> symbols{L'\x2800', L'\x2500', L'\x2502', L'\x253C'}; // "⠀─│┼"
static constexpr std::chrono::milliseconds fade_time{500ms};

static auto &palette() {
    static const std::array<ftxui::Color, 4> colors{
            ftxui::Color::YellowLight, ftxui::Color::Yellow, ftxui::Color::GrayLight, ftxui::Color::GrayDark
    };
    return colors;
};

}