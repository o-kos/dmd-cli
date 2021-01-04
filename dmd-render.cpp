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
                if (c < L'\x2800') screen.PixelAt(x, y).foreground_color = Color::Red;
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
