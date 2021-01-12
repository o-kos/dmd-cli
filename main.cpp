#include <chrono>
#include <thread>
#include <fstream>

#include <ryml.hpp>

#include "loren.hpp"
#include "dmd-render.hpp"

using namespace std;
using namespace chrono;
using namespace chrono_literals;

int main(int /*argc*/, const char *argv[])
{
    dmd::State state;

    state.header.title = L"Processing file";
    state.header.value = L"~/signals/psk8-wav";
    state.progress.prefix = L"Parse data";
    state.progress.limit = 1;
    state.params = {{L"freq", L"1800"}, {L"mode", L"PSK-8"}, {L"speed", L"1200"}, {L"interleaver", L"long"}};
    state.status.text = -1;
    auto r = make_unique<dmd::TuiRender>();
    r->render(state);

    ifstream t(argv[1]);
    stringstream buffer;
    buffer << t.rdbuf();
    auto tree = ryml::parse(c4::to_csubstr(buffer.str().c_str()));
    struct Nodes {
        explicit Nodes(ryml::NodeRef root)
                : burst_node{root.first_child()}
                , times{steady_clock::now(), steady_clock::now(), steady_clock::now()} {
            auto last_burst_node = burst_node.last_sibling();
            auto last_tick_node = last_burst_node.first_child().first_child();
            unsigned t;
            c4::atou(last_tick_node.val(), &t);
            times.finish = times.start + milliseconds(t * 1000);
        }
        ryml::NodeRef burst_node;
        struct {
            steady_clock::time_point start;
            steady_clock::time_point next;
            steady_clock::time_point finish;
            [[nodiscard]] unsigned duration() const {
                return duration_cast<chrono::milliseconds>(finish - start).count() + 1000;
            }
        } times;
        bool next() {
            burst_node = burst_node.next_sibling();
            return burst_node.valid();
        }
        bool read_points(dmd::PhasePoints &points) {
            if (steady_clock::now() < times.next) return false;

            auto tick_node = burst_node.first_child().first_child();
            unsigned t;
            c4::atou(tick_node.val(), &t);
            times.next = times.start + milliseconds(t * 1000);
            auto points_node = tick_node.next_sibling();
            points.clear();
            points.reserve(points_node.num_children());
            for (auto pn : points_node) {
                auto px_node = pn.first_child();
                auto py_node = px_node.next_sibling();
                float px, py;
                c4::atof(px_node.val(), &px);
                c4::atof(py_node.val(), &py);
                points.push_back({px, py});
            }
            return true;
        }
    } nodes{tree.rootref()};
    state.progress.prefix = L"Process";
    state.progress. limit = nodes.times.duration();
    r->render(state);

    const unsigned speed = 100;
    dmd::PhasePoints points;
    for (;;) {
        state.status.bits += 1;
        state.push_progress(100 * speed);
        if (state.status.bits % 10 == 0) state.push_log(nextLoren());
        if (nodes.read_points(points)) {
            state.push_phase(points);
            state.push_log(nextLoren());
            state.status.phase += points.size();
        }
        r->render(state);

        state.points.clear();
        if (!nodes.next()) break;
        std::this_thread::sleep_for(100ms);
    }
}

