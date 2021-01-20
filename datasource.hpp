#pragma once

#include "loren.hpp"
#include "dmd-render.hpp"

#include <ryml.hpp>

#include <ctime>
#include <fstream>
#include <filesystem>

//template<typename Clock, typename Duration>
//std::ostream &operator<<(
//    std::ostream &stream,
//    std::chrono::time_point<Clock, Duration> time_point) {
//
//    using namespace std::chrono;
//    auto ms = duration_cast<milliseconds>(time_point.time_since_epoch());
//    seconds s = duration_cast<seconds>(ms);
//    minutes m = duration_cast<minutes>(ms);
//    char buf[10];
//    snprintf(buf, sizeof(buf), "%.2d:%.2d.%.3d", m.count() % 60, s.count() % 60, ms.count() % 1000);
//    return stream << buf;
//}

namespace dmd {

class DataSource {
public:
    explicit DataSource(const std::filesystem::path &filename) : tree{}, burst_node{}, times{} {
        std::ifstream plog(filename);
        std::stringstream buffer;
        buffer << plog.rdbuf();
        tree = ryml::parse(c4::to_csubstr(buffer.str().c_str()));
        burst_node = tree.rootref().first_child();
        auto last_burst_node = burst_node.last_sibling();
        auto last_tick_node = last_burst_node.first_child().first_child();
        unsigned t;
        c4::atou(last_tick_node.val(), &t);
        times.start = std::chrono::steady_clock::now();
        times.finish = times.start + std::chrono::milliseconds(t);
        times.update_next(burst_node.first_child().first_child());
    }

    bool next() {
        burst_node = burst_node.next_sibling();
        if (!burst_node.valid()) return false;
        times.update_next(burst_node.first_child().first_child());
        return true;
    }

    bool points(dmd::PhasePoints &pts) {
        if (std::chrono::steady_clock::now() < times.next) return false;

        auto tick_node = burst_node.first_child().first_child();
        auto points_node = tick_node.next_sibling();
        pts.clear();
        pts.reserve(points_node.num_children());
        for (auto pn : points_node) {
            auto px_node = pn.first_child();
            auto py_node = px_node.next_sibling();
            float px, py;
            c4::atof(px_node.val(), &px);
            c4::atof(py_node.val(), &py);
            pts.push_back({px, py});
        }
        return true;
    }

    [[nodiscard]] std::wstring log() const {            // NOLINT(readability-convert-member-functions-to-static)
        return loren();
    }

    [[nodiscard]] unsigned duration() const { return times.duration(); }

private:
    ryml::Tree tree;
    ryml::NodeRef burst_node;
    struct {
        std::chrono::steady_clock::time_point start;
        std::chrono::steady_clock::time_point next;
        std::chrono::steady_clock::time_point finish;
        [[nodiscard]] unsigned duration() const {
            return std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() + 1000;
        }
        void update_next(ryml::NodeRef node) {
            unsigned t;
            c4::atou(node.val(), &t);
            next = start + std::chrono::milliseconds(t);
//            std::cout << std::endl << start << " " << std::chrono::steady_clock::now() << " " << next << "  " << finish << std::endl;
        }
    } times;
};

}

