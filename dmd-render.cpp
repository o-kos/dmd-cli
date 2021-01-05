#include "ftxui/dom/elements.hpp"
#include "dmd-render.hpp"

namespace ftxui {

class PhaseDiagram : public Node {
public:
    static const int width  = 21;
    static const int height = 9;

    explicit PhaseDiagram(const dmd::PhasePoints &points) : Node() {
        static const wchar_t symbols[] = L"\x2800\x2500\x2502\x253C"; // "⠀─│┼"
        canvas.fill(symbols[0]);
        for (int x = 0; x < width;  ++x) pixel(x, height / 2) = symbols[1];
        for (int y = 0; y < height; ++y) pixel(width / 2,  y) = symbols[2];
        pixel(width /2, height / 2) = symbols[3];

        auto paint = [this](dmd::Point p) {
            static const float delta[2] = { -1e-5, +1e-5 };
//            p.x = std::clamp(p.x, -1.0f, +1.0f);
//            p.y = std::clamp(p.y, -1.0f, +1.0f);
//            p.x = std::clamp(p.x, -1.0f + delta[1], +1.0f + delta[0]);
//            p.y = std::clamp(p.y, -1.0f + delta[1], +1.0f + delta[0]);
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

            uint8_t symbol = pixel(cx, cy) - L'\x2800';
            static const uint8_t masks[4][2] = {
                {0x01, 0x08},
                {0x02, 0x10},
                {0x04, 0x20},
                {0x40, 0x80},
            };
            symbol |= masks[vy % 4][vx % 2];
            pixel(cx, cy) = L'\x2800' + symbol;
        };

        for (auto const &p : points) paint(p);
    }

    void ComputeRequirement() override {
        requirement_.min_x = width;
        requirement_.min_y = height;
    }

    void Render(Screen &screen) override {
        for (int y = box_.y_min; y <= box_.y_max; ++y) {
            for (int x = box_.x_min; x <= box_.x_max; ++x) {
                wchar_t c = pixel(x - box_.x_min, y - box_.y_min);
                screen.at(x, y) = c;
                if (c < L'\x2800') screen.PixelAt(x, y).foreground_color = Color::Magenta;
            }
        }
    }

private:
    std::array<wchar_t, width * height> canvas{};
    wchar_t &pixel(int x, int y) { return canvas[y * width + x]; }
};

Element phase(const dmd::PhasePoints &points) {
    return std::make_shared<PhaseDiagram>(points);
}

}
