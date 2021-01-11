#include "dmd-render.hpp"
#include "gtest/gtest.h"

#include <ftxui/screen/string.hpp>

using namespace std;
using namespace ftxui;

::testing::AssertionResult IsPhaseCorrect(const Element &phase, const wstring &pattern)
{
    static const array<wstring, 9> phase_screen{
            L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
            L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
            L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
            L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
             L"──────────┼──────────",
            L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
            L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
            L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
            L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
    };

    const auto h = phase_screen.size();
    const auto w = phase_screen[0].size();

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
            if (cs != cp)
                return ::testing::AssertionFailure()
                    << "pixel at [" << x << ", " << y << "] "
                    << printhex(cp)
                    << " is not "
                    << printhex(cs);
        }

    return ::testing::AssertionSuccess();
}

TEST(PhaseTest, InvalidPatternSize) {
    const wstring small_height =
        L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
          "──────────┼──────────"
         "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀";
    EXPECT_FALSE(IsPhaseCorrect(phase({}), small_height));
    const wstring big_height =
        L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
         "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
         "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
         "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
          "──────────┼──────────"
         "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
         "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
         "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
         "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀++++";
    EXPECT_FALSE(IsPhaseCorrect(phase({}), big_height));
}

TEST(PhaseTest, EmptyPoints) {
    const wstring empty =
        L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
         "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
         "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
         "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
          "──────────┼──────────"
         "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
         "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
         "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
         "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀";
    EXPECT_TRUE(IsPhaseCorrect(phase({}), empty));
}

TEST(PhaseTest, CenterPoint) {
    const wstring center =
        L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
         "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
         "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
         "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
          "──────────┼──────────"
         "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀"
         "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
         "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
         "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀";
    EXPECT_TRUE(IsPhaseCorrect(phase({{0, 0}}), center));
}

TEST(PhaseTest, NearCenterPoints) {
    const wstring centers =
            L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
             "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
             "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
             "⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀│⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
              "──────────┼──────────"
             "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈│⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀"
             "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
             "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
             "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀";
    EXPECT_TRUE(IsPhaseCorrect(phase({{1e-5, -1e-5}, {-1e-5, -1e-5}, {-1e-5, 1e-5}, {1e-5, 1e-5}}), centers));
}

TEST(PhaseTest, CornersPoints) {
    const wstring corners =
            L"⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈"
             "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
             "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
             "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
              "──────────┼──────────"
             "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
             "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
             "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
             "⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀";
    EXPECT_TRUE(IsPhaseCorrect(phase({{-1, -1}, {-1, 1}, {1, -1}, {1, 1}}), corners));
}

