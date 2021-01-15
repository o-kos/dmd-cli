#include "datasource.hpp"
#include "dmd-render.hpp"

#include <chrono>
#include <thread>

using namespace std;
using namespace chrono;
using namespace chrono_literals;

int main(int /*argc*/, const char *argv[])
{
    std::filesystem::path path{argv[1]};

    dmd::State state;

    state.header.title = L"Processing file";
    state.header.value = path.wstring();
    state.progress.prefix = L"Parse data";
    state.progress.limit = 1;
    state.params = {{L"freq", L"1800"}, {L"mode", L"PSK-8"}, {L"speed", L"1200"}, {L"interleaver", L"long"}};
    state.status.text = -1;
    auto r = make_unique<dmd::TuiRender>();
    r->render(state);

    dmd::DataSource src{path};
    state.progress.limit = src.duration();
    r->render(state);

    const unsigned speed = 1;
    dmd::PhasePoints points;
    for (bool flag = true; flag;) {
        state.status.bits += 1;
        state.push_progress(100 * speed);
        if (state.status.bits % 10 == 0) state.push_log(src.log());
        if (src.points(points)) {
            state.push_phase(points);
            state.push_log(src.log());
            state.status.phase += points.size();
            flag = src.next();
        }
        r->render(state);

        state.points.clear();
        std::this_thread::sleep_for(100ms);
    }

    return 0;
}

