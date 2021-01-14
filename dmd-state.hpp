#pragma once

#include <cmath>
#include <string>
#include <chrono>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <complex>

#include <queue>

namespace dmd {
    struct Point {
        float x, y;
    };
    using PhasePoints = std::vector<Point>;

struct State {
    static std::wstring to_ws(std::chrono::milliseconds ms) {
        using namespace std::chrono;
        seconds s = duration_cast<seconds>(ms);
        minutes m = duration_cast<minutes>(ms);
        hours   h = duration_cast<hours>(ms);
        wchar_t buf[6];
        swprintf(buf, sizeof(buf), L"%.2d:%.2d", m.count() % 60, s.count() % 60);
        std::wstring ts{buf};
        if (h.count() > 0) ts.insert(0, std::to_wstring(h.count()) + L':');
        return ts;
    };

    std::vector<std::pair<std::wstring, std::wstring>> params;

    struct {
        std::wstring title;
        std::wstring value;
    } header;

    struct Log {
        std::vector<std::pair<std::wstring, std::wstring>> buffer;

        void push(std::chrono::milliseconds cur_ms, const std::wstring &line) {
            auto timestamp = to_ws(cur_ms) + L' ';
            if (buffer.size() > 100) buffer.erase(buffer.begin());
            buffer.emplace_back(timestamp, line);
        }
    } log;

    struct Progress {
        std::wstring prefix;
        unsigned position;
        unsigned limit;
        std::wstring suffix;

        Progress() : position{0}, limit{0} {}

        [[nodiscard]] auto value() const { return (limit) ? std::min(100.0f, float(position) / float(limit)) : 0; }

        [[nodiscard]] std::wstring percent() const {
            wchar_t buf[6];
            swprintf(buf, sizeof(buf), L"%3d%% ", std::lround(value() * 100));
            return buf;
        }

        void update(std::chrono::milliseconds cur_ms, unsigned shift) {
            using namespace std::chrono;
            position += shift;
            milliseconds eta_ms{std::lround(cur_ms.count() / value())};
            if (cur_ms > eta_ms) eta_ms = cur_ms;
            std::wstringstream ss;
            ss << std::fixed << std::setprecision(1) << position * 1000.0 / cur_ms.count();

            suffix = L" " + to_ws(cur_ms) + L"<" + to_ws(eta_ms) + L", " + ss.str() + L"it/s ";
        }
    } progress;

    struct Status {
        int bits, text, phase;
        Status() : bits{0}, text{0}, phase{0} {}
    } status;

    dmd::PhasePoints points;
    std::chrono::steady_clock::time_point start;

    State() : start{std::chrono::steady_clock::now()} {}

    void push_progress(unsigned shift, const std::wstring &log_line) {
        using namespace std::chrono;
        auto cur_ms = duration_cast<milliseconds>(steady_clock::now() - start);
        log.push(cur_ms, log_line);
        progress.update(cur_ms, shift);
    }

    void push_phase(const dmd::PhasePoints &p) {
        points.insert(points.end(), p.begin(), p.end());
        status.phase += p.size();
    }
};

}