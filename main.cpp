#include "dmd-state.hpp"
#include "dmd-render.hpp"
#include "loren.hpp"

#include <thread>

using namespace std;
using namespace chrono;

int main(int /*argc*/, const char** /*argv[]*/) {
    using namespace chrono_literals;

    dmd::State state;

    state.header.title = L"Processing file";
    state.header.value = L"~/signals/psk8-wav";
    state.progress.prefix = L"Process";
    state.progress.limit = 4 * 60 * 9600;
    state.params = {{L"freq", L"1800"}, {L"mode", L"PSK-8"}, {L"speed", L"1200"}, {L"interleaver", L"long"}};
    state.status.text = -1;

    auto r = make_unique<dmd::TuiRender>();
    std::this_thread::sleep_for(200ms);
    r->render(state);

    const unsigned speed = 4;
    dmd::Point shift{2.0f / 80, 2.0f / 64};
    dmd::Point p{-1, -1};
    for (;;) {
        state.push_progress(9600 * 100 / 1000 * speed, nextLoren());
        state.status.bits += 1;
        state.push_phase({p});
        p.x += shift.x;
        if (p.x >= 1) {
            p.x = -1;
            p.y += shift.y;
            if (p.y >= 1) p.y = -1;
        }
        r->render(state);

        if (state.progress.position >= state.progress.limit) break;
        std::this_thread::sleep_for(10ms);
    }
}

