#include "ftxui/dom/elements.hpp"
#include "dmd-render.hpp"

namespace ftxui {

class PhaseDiagram : public Node {
public:
    static const int width  = 21;
    static const int height = 9;

    explicit PhaseDiagram(const dmd::PhaseData &data) : Node() {
        static const wchar_t symbols[] = L"\x2800\x2500\x2502\x253C"; // "⠀─│┼"
        canvas.fill({symbols[0], Color::Default});

        auto paint = [this](dmd::Point p, int color_index) {
            static const float delta[2] = { -1e-5, +1e-5 };
            p.x = std::clamp(p.x, -1.0f, +1.0f + delta[0]);
            p.y = std::clamp(p.y, -1.0f, +1.0f + delta[0]);
            if (p.x == 0) p.x += delta[std::rand() % 2];  // NOLINT(cert-msc30-c, cert-msc50-cpp)
            if (p.y == 0) p.y += delta[std::rand() % 2];  // NOLINT(cert-msc30-c, cert-msc50-cpp)
            static const int scale_x = ((width  - 1) / 2) * 2;
            static const int scale_y = ((height - 1) / 2) * 4;
            int vx = int(p.x * scale_x + scale_x);
            int vy = int(p.y * scale_y + scale_y);
            int cx = int(p.x > 0) + vx / 2;
            int cy = int(p.y > 0) + vy / 4;

            auto &px = pixel(cx, cy);
            uint8_t symbol = px.symbol - L'\x2800';
            static const uint8_t masks[4][2] = {
                {0x01, 0x08},
                {0x02, 0x10},
                {0x04, 0x20},
                {0x40, 0x80},
            };
            symbol |= masks[vy % 4][vx % 2];
            px.symbol = wchar_t(L'\x2800' + symbol);
            px.color_index = color_index;
        };

        using namespace std::chrono;
        for (auto const &d : data) {
            auto ms = duration_cast<milliseconds>(d.first - steady_clock::now()).count();
            auto idx = ms / 500;
            if (idx >= 5) break;
            for (auto const &p : d.second) paint(p, idx);
        }
    }

    void ComputeRequirement() override {
        requirement_.min_x = width;
        requirement_.min_y = height;
    }

    void Render(Screen &screen) override {
        auto draw_axes = [=](int x, int y, Pixel &pixel) {
            static const wchar_t symbols[] = L"\x2800\x2500\x2502\x253C"; // "⠀─│┼"
            auto idx = int(x == width / 2) + 2 * (y == height / 2);
            if (!idx) return false;
            pixel.foreground_color = Color::Magenta;
            pixel.character = symbols[idx];
            return true;
        };

        static const Color colors[] = { Color::Default, Color::White, Color::GrayLight, Color::GrayDark};
        for (int y = box_.y_min; y <= box_.y_max; ++y) {
            for (int x = box_.x_min; x <= box_.x_max; ++x) {
                auto &px = screen.PixelAt(x, y);
                if (!draw_axes(x, y, px)) {
                    auto rpx = pixel(x - box_.x_min, y - box_.y_min);
                    px.character = rpx.symbol;
                    px.foreground_color = colors[
                        std::min<unsigned>(rpx.color_index, sizeof(colors) / sizeof(colors[0]))
                    ];
                }
            }
        }
    }

private:
    struct RawPixel {
        unsigned color_index;
        wchar_t symbol;
    };
    std::array<RawPixel, width * height> canvas{};
    RawPixel &pixel(int x, int y) { return canvas[y * width + x]; }
};

Element phase(const dmd::PhaseData &data) {
    return std::make_shared<PhaseDiagram>(data);
}

}
