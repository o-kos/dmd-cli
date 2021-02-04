#include "phase.hpp"
#include "ftxui/dom/elements.hpp"
#include "dmd-render.hpp"

namespace ftxui {

using namespace phase_cfg;

class PhaseDiagram : public Node {
public:

    explicit PhaseDiagram(const dmd::PhaseData &data) : Node() {
        canvas.fill({0, symbols[0]});

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
            uint8_t symbol = px.symbol - symbols[0];
            static const uint8_t masks[4][2] = {
                {0x01, 0x08},
                {0x02, 0x10},
                {0x04, 0x20},
                {0x40, 0x80},
            };
            symbol |= masks[vy % 4][vx % 2];
            px.symbol = wchar_t(symbols[0] + symbol);
            px.color_index = color_index;
        };

        using namespace std::chrono;
        for (auto const &d : data) {
            auto ms = duration_cast<milliseconds>(steady_clock::now() - d.first).count();
            auto idx = ms / fade_time.count();
            if (idx >= palette().size()) break;
            for (auto const &p : d.second) paint(p, idx);
        }
    }

    explicit PhaseDiagram(const dmd::PhasePoints &points)
        : PhaseDiagram({{std::chrono::steady_clock::now(), points}}) {
    }

    PhaseDiagram()
        : PhaseDiagram({{std::chrono::steady_clock::now(), {}}}) {
    }

    void ComputeRequirement() override {
        requirement_.min_x = width;
        requirement_.min_y = height;
    }

    void Render(Screen &screen) override {
        auto draw_axes = [=](int x, int y, Pixel &pixel) {
            auto idx = int(y == height / 2) + 2 * int(x == width / 2);
            if (!idx) return false;
            pixel.character = symbols[idx];
            return true;
        };

        for (int y = box_.y_min; y <= box_.y_max; ++y) {
            for (int x = box_.x_min; x <= box_.x_max; ++x) {
                auto &px = screen.PixelAt(x, y);
                if (!draw_axes(x - box_.x_min, y - box_.y_min, px)) {
                    auto rpx = pixel(x - box_.x_min, y - box_.y_min);
                    if (rpx.color_index < palette().size()) {
                        px.character = rpx.symbol;
                        px.foreground_color = palette()[rpx.color_index];
                    }
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

Element phase(const dmd::PhasePoints &points) {
    return std::make_shared<PhaseDiagram>(points);
}

Element phase() {
    return std::make_shared<PhaseDiagram>();
}

}
