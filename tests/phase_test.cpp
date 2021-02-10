#include "dmd-state.hpp"
#include "dmd-render.hpp"
#include "phase.hpp"

#include "gtest/gtest.h"

#include <ftxui/screen/string.hpp>

using namespace std;
using namespace ftxui;
using namespace std::chrono;

auto &phase_screen() {
    static const array<wstring, phase_cfg::height> screen{
            L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
            L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
            L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
            L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
            L"⠤⠤⠤⠤⠤⠤⠤⠤⠤⠤⢼⠤⠤⠤⠤⠤⠤⠤⠤⠤⠤",
            L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
            L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
            L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
            L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
    };
    return screen;
}

::testing::AssertionResult IsPhaseCorrect(const Element &phase, const wstring &pattern)
{
    const auto h = phase_screen().size();
    const auto w = phase_screen()[0].size();

    auto ph = pattern.size() / w + int(pattern.size() % w != 0);
    if (ph != h)
        return ::testing::AssertionFailure() << "pattern height " << ph << " is not " << h;

    Screen screen(w, h);
    Render(screen, phase);

    auto printhex = [](wchar_t c) {
        char buf[] = { "'\\xXXXX'\0" };
        snprintf(buf, sizeof(buf), "'\\x%.4X'", unsigned(c));
        return string(buf);
    };
    for (auto y = 0; y < h; ++y)
        for (auto x = 0; x < w; ++x) {
            wchar_t cs = screen.at(x, y);
            wchar_t cp = pattern[x + y * w];
            if (x == w / 2 && y == h / 2) {
                if (cp == L'⢼') cp = L'┼';
            } else if (x == w / 2) {
                if (cp == L'⢸') cp = L'│';
            } else if (y == h / 2) {
                if (cp == L'⠤') cp = L'─';
            }
            if (cs != cp)
                return ::testing::AssertionFailure()
                    << "pixel at [" << x << ", " << y << "] "
                    << printhex(cp)
                    << " is not "
                    << printhex(cs);
        }

    return ::testing::AssertionSuccess();
}

TEST(PhaseTest, InvalidPatternSize) {           // NOLINT(cert-err58-cpp)
    const wstring small_height = L""
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠤⠤⠤⠤⠤⠤⠤⠤⠤⠤⢼⠤⠤⠤⠤⠤⠤⠤⠤⠤⠤"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀";
    EXPECT_FALSE(IsPhaseCorrect(phase(), small_height));
    const wstring big_height = L""
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠤⠤⠤⠤⠤⠤⠤⠤⠤⠤⢼⠤⠤⠤⠤⠤⠤⠤⠤⠤⠤"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀+++";
    EXPECT_FALSE(IsPhaseCorrect(phase(), big_height));
}

TEST(PhaseTest, EmptyPoints) {                  // NOLINT(cert-err58-cpp)
    const wstring empty = L""
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠤⠤⠤⠤⠤⠤⠤⠤⠤⠤⢼⠤⠤⠤⠤⠤⠤⠤⠤⠤⠤"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀";
    EXPECT_TRUE(IsPhaseCorrect(phase(), empty));
}

TEST(PhaseTest, CenterPoint) {                  // NOLINT(cert-err58-cpp)
    wstring center = L""
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠤⠤⠤⠤⠤⠤⠤⠤⠤⠤⢼⠤⠤⠤⠤⠤⠤⠤⠤⠤⠤"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀";
    static const wchar_t cp[] = L"⢀⡀⠈⠁";

    const auto h = phase_screen().size();
    const auto w = phase_screen()[0].size();

    srand(0);                                   // NOLINT(cert-msc51-cpp)
    auto x = 2 * (rand() % 2) - 1;              // NOLINT(cert-msc30-c, cert-msc50-cpp)
    auto y = 2 * (rand() % 2) - 1;              // NOLINT(cert-msc30-c, cert-msc50-cpp)
    center[w / 2 + x + (h / 2 + y) * w] = cp[(x + 1) / 2 + (y + 1)];
    srand(0);                                   // NOLINT(cert-msc51-cpp)
    EXPECT_TRUE(IsPhaseCorrect(phase({{0, 0}}), center));
}

TEST(PhaseTest, NearCenterPoints) {             // NOLINT(cert-err58-cpp)
    const wstring centers = L""
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⢸⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠤⠤⠤⠤⠤⠤⠤⠤⠤⠤⢼⠤⠤⠤⠤⠤⠤⠤⠤⠤⠤"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢸⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀";
    EXPECT_TRUE(IsPhaseCorrect(phase({{1e-5, -1e-5}, {-1e-5, -1e-5}, {-1e-5, 1e-5}, {1e-5, 1e-5}}), centers));
}

TEST(PhaseTest, CornersPoints) {                // NOLINT(cert-err58-cpp)
    const wstring corners = L""
        "⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠤⠤⠤⠤⠤⠤⠤⠤⠤⠤⢼⠤⠤⠤⠤⠤⠤⠤⠤⠤⠤"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
        "⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀";
    EXPECT_TRUE(IsPhaseCorrect(phase({{-1, -1}, {-1, 1}, {1, -1}, {1, 1}}), corners));
}

dmd::State mock_state(const vector<pair<milliseconds, dmd::PhasePoints>> &data) {
    dmd::State state;
    for (const auto &d : data)
        state.push_phase(d.second, d.first);
    return state;
}

struct TestColorPixel {
    int x, y;
    int color_index;
};

::testing::AssertionResult IsColorCorrect(const Element &phase, const vector<TestColorPixel> &pixels)
{
    const auto h = phase_screen().size();
    const auto w = phase_screen()[0].size();

    Screen screen(w, h);
    Render(screen, phase);

    for (const auto &p : pixels) {
        auto ps = screen.PixelAt(p.x, p.y);
        if (p.color_index < 0 && ps.character != phase_screen()[0][0])
            return ::testing::AssertionFailure() << "pixel at [" << p.x << ", " << p.y << "] is not empty";
        if (p.color_index >= 0 && ps.foreground_color != phase_cfg::palette()[p.color_index])
            return ::testing::AssertionFailure()
                << "pixel color at [" << p.x << ", " << p.y << "] = "
                << phase_cfg::palette()[p.color_index].Print(false)
                << ", but not "
                << ps.foreground_color.Print(false);
    }

    return ::testing::AssertionSuccess();
}

TEST(PhaseTest, CurrentColor) {                 // NOLINT(cert-err58-cpp)
    auto state = mock_state({{milliseconds::zero(), {{-1, -1}}}});
    EXPECT_TRUE(IsColorCorrect(
        phase(state.points.data),
        {{0, 0, 0}}
    ));
}

TEST(PhaseTest, SimplePastColors) {             // NOLINT(cert-err58-cpp)
    auto ts = [](unsigned count) {
        return -(ftxui::phase_cfg::fade_time * count * 11 / 10);
    };
    auto state = mock_state({
        { ts(3), {{-0.67, -1.00}}},
        { ts(2), {{-0.78, -1.00}}},
        { ts(1), {{-0.89, -1.00}}},
        { ts(0), {{-1.00, -1.00}}}
    });
    EXPECT_TRUE(IsColorCorrect(
        phase(state.points.data),
        {
            {3, 0, -1},
            {2, 0,  2},
            {1, 0,  1},
            {0, 0,  0},
        }
    ));
}
