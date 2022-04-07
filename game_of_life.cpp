#include <iostream>
#include <vector>
#include <unordered_set>
#include <tuple>
#include <algorithm>
#include <chrono>
#include <thread>
#include <cstdio>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <cstdlib>

#include "TextImage.h"


// Game of Life Automata
using namespace g80;
using Creature = Dim;
using Creatures = std::unordered_set<Creature>;

constexpr int FPS = 15;
constexpr int MSPF = 1000 / FPS;
namespace chr = std::chrono;
namespace this_thread = std::this_thread;
using TimePointSysClock = chr::time_point<chr::system_clock>;
using SysClock = chr::system_clock;

auto is_key_pressed() -> int;
auto spawner(const Area &area, const Dim &N) -> Creatures;

auto neighbor_count(Creatures &creatures, Creature &creature) -> Dim;
auto kill_creatures_with_single_neighbors(Creatures &creatures, const Area &area, const Dim &at_most_neighbors = 2) -> Creatures;
auto kill_creatures(const Creatures &creatures_to_kill, Creatures &creatures) -> void;

auto main(int argc, char **argv) -> int {

    const Dim N = 1000;
    Area area({140, 50});
    TextImage screen(area);
    Creatures creatures = spawner(area, N);
    
    do {
        TimePointSysClock start {SysClock::now()};

        // Erase previous
        for (auto &c : creatures)
            screen.set_text(c, ' ');

        // Execute Policies
        Creatures to_kill = kill_creatures_with_single_neighbors(creatures, area, 2);
        kill_creatures(to_kill, creatures);

        // Render
        for (auto &c : creatures) {
            screen.set_text(c, 'x');
            screen.set_color(c, rand() % 8);
        }

        screen.show();
        TimePointSysClock end = SysClock::now();
        int delay = MSPF - chr::duration_cast<chr::milliseconds>(end - start).count();
        if (delay > 0) 
            this_thread::sleep_for(chr::milliseconds(delay));

        exit(0);
    } while(!is_key_pressed() && creatures.size() > 0);
}

auto kill_creatures(const Creatures &creatures_to_kill, Creatures &creatures) -> void {
    for (auto &c : creatures_to_kill)
        creatures.erase(c);
}

auto neighbor_count(const Creatures &creatures, const Area &area, const Creature &creature) -> Dim {
    Dim neighbor = 0;

    Dim top = creature - area.w;
    Dim upper_left = top - 1;
    Dim upper_right = top + 1;
    Dim left = creature - 1;
    Dim right = creature + 1;
    Dim bottom = creature + area.w;
    Dim bottom_left = bottom - 1;
    Dim bottom_right = bottom + 1;    

    if (creatures.find(top) != creatures.end()) ++neighbor;
    if (creatures.find(upper_left) != creatures.end()) ++neighbor;
    if (creatures.find(upper_right) != creatures.end()) ++neighbor;
    if (creatures.find(left) != creatures.end()) ++neighbor;
    if (creatures.find(right) != creatures.end()) ++neighbor;
    if (creatures.find(bottom) != creatures.end()) ++neighbor;
    if (creatures.find(bottom_left) != creatures.end()) ++neighbor;
    if (creatures.find(bottom_right) != creatures.end()) ++neighbor;

    return neighbor;
}

auto kill_creatures_with_single_neighbors(Creatures &creatures, const Area &area, const Dim &at_most_neighbors) -> Creatures {

    Creatures to_kill;

    to_kill.reserve(area());
    for (auto &creature : creatures)
        if (neighbor_count(creatures, area, creature) <= at_most_neighbors) 
            to_kill.insert(creature);
    
    return to_kill;
}

auto spawner(const Area &area, const Dim &N) -> Creatures {
    
    std::vector<Dim> creature_ixs;
    
    creature_ixs.reserve(area());
    for (Dim i = 0; i < area(); ++i)
        creature_ixs.push_back(i);

    for (Dim i = 0; i < area(); ++i)
        std::swap(creature_ixs[i], creature_ixs[rand() % area()]);

    Creatures creatures;
    creatures.reserve(N);
    for (Dim i = 0; i < N; ++i)
        creatures.insert(creature_ixs[i]);

    return creatures;
}

auto is_key_pressed() -> int {
    static const int STDIN = 0;
    static bool initialized = false;

    if (!initialized) {
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting); 
    return bytesWaiting;
}
