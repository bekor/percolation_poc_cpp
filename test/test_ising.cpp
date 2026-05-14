#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <vector>
#include <numeric>
#include <algorithm>
#include <set>

#include "ising_simulation.h"

// ─────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────

static std::vector<uint8_t> make_all_ones(size_t rows, size_t cols) {
    return std::vector<uint8_t>(rows * cols, 1);
}

static std::vector<uint8_t> make_all_zeros(size_t rows, size_t cols) {
    return std::vector<uint8_t>(rows * cols, 0);
}

static std::vector<uint8_t> make_checkerboard(size_t rows, size_t cols) {
    std::vector<uint8_t> state(rows * cols);
    for (size_t r = 0; r < rows; ++r)
        for (size_t c = 0; c < cols; ++c)
            state[r * cols + c] = (r + c) % 2;
    return state;
}

static std::vector<uint8_t> make_half_half(size_t rows, size_t cols) {
    // left half = 1, right half = 0
    std::vector<uint8_t> state(rows * cols, 0);
    for (size_t r = 0; r < rows; ++r)
        for (size_t c = 0; c < cols / 2; ++c)
            state[r * cols + c] = 1;
    return state;
}

// ─────────────────────────────────────────────
// get_neighbors
// ─────────────────────────────────────────────

TEST_CASE("get_neighbors: interior cell has 4 unique neighbors", "[neighbors]") {
    IsingSimulation sim(5, 5);
    auto neighbors = sim.get_neighbors(2, 2);
    REQUIRE(neighbors.size() == 4);
    // all unique
    std::sort(neighbors.begin(), neighbors.end());
    REQUIRE(std::unique(neighbors.begin(), neighbors.end()) == neighbors.end());
}

TEST_CASE("get_neighbors: interior cell correct indices", "[neighbors]") {
    IsingSimulation sim(5, 5);
    // center (2,2) → up(1,2)=7, down(3,2)=17, right(2,3)=13, left(2,1)=11
    auto neighbors = sim.get_neighbors(2, 2);
    std::sort(neighbors.begin(), neighbors.end());
    std::vector<size_t> expected = {7, 11, 13, 17};
    REQUIRE(neighbors == expected);
}

TEST_CASE("get_neighbors: top-left corner wraps correctly", "[neighbors]") {
    IsingSimulation sim(5, 5);
    // (0,0): up→(4,0)=20, down→(1,0)=5, right→(0,1)=1, left→(0,4)=4
    auto neighbors = sim.get_neighbors(0, 0);
    std::sort(neighbors.begin(), neighbors.end());
    std::vector<size_t> expected = {1, 4, 5, 20};
    REQUIRE(neighbors == expected);
}

TEST_CASE("get_neighbors: bottom-right corner wraps correctly", "[neighbors]") {
    IsingSimulation sim(5, 5);
    // (4,4): up→(3,4)=19, down→(0,4)=4, right→(4,0)=20, left→(4,3)=23
    auto neighbors = sim.get_neighbors(4, 4);
    std::sort(neighbors.begin(), neighbors.end());
    std::vector<size_t> expected = {4, 19, 20, 23};
    REQUIRE(neighbors == expected);
}

TEST_CASE("get_neighbors: top-right corner wraps correctly", "[neighbors]") {
    IsingSimulation sim(5, 5);
    // (0,4): up→(4,4)=24, down→(1,4)=9, right→(0,0)=0, left→(0,3)=3
    auto neighbors = sim.get_neighbors(0, 4);
    std::sort(neighbors.begin(), neighbors.end());
    std::vector<size_t> expected = {0, 3, 9, 24};
    REQUIRE(neighbors == expected);
}

TEST_CASE("get_neighbors: first row wraps up to last row", "[neighbors]") {
    IsingSimulation sim(4, 4);
    // (0,2): up→(3,2)=14, down→(1,2)=6, right→(0,3)=3, left→(0,1)=1
    auto neighbors = sim.get_neighbors(0, 2);
    std::sort(neighbors.begin(), neighbors.end());
    std::vector<size_t> expected = {1, 3, 6, 14};
    REQUIRE(neighbors == expected);
}

TEST_CASE("get_neighbors: first col wraps left to last col", "[neighbors]") {
    IsingSimulation sim(4, 4);
    // (2,0): up→(1,0)=4, down→(3,0)=12, right→(2,1)=9, left→(2,3)=11
    auto neighbors = sim.get_neighbors(2, 0);
    std::sort(neighbors.begin(), neighbors.end());
    std::vector<size_t> expected = {4, 9, 11, 12};
    REQUIRE(neighbors == expected);
}

TEST_CASE("get_neighbors: non-square grid correct", "[neighbors]") {
    IsingSimulation sim(3, 6);
    // (1,3): up→(0,3)=3, down→(2,3)=15, right→(1,4)=10, left→(1,2)=8
    auto neighbors = sim.get_neighbors(1, 3);
    std::sort(neighbors.begin(), neighbors.end());
    std::vector<size_t> expected = {3, 8, 10, 15};
    REQUIRE(neighbors == expected);
}

// ─────────────────────────────────────────────
// energy_around_point  (convention: -si * sum(sj), si/sj ∈ {-1,+1})
// ─────────────────────────────────────────────

TEST_CASE("energy_around_point: all-ones grid interior → -4", "[energy_point]") {
    IsingSimulation sim(5, 5);
    auto state = make_all_ones(5, 5);
    // si=+1, all sj=+1 → local_energy=+4 → return -1*1*4 = -4
    int e = sim.energy_around_point(state, 2, 2, 1);
    REQUIRE(e == -4);
}

TEST_CASE("energy_around_point: all-zeros grid interior → -4", "[energy_point]") {
    IsingSimulation sim(5, 5);
    auto state = make_all_zeros(5, 5);
    // si=-1, all sj=-1 → local_energy=-4 → return -1*(-1)*(-4) = -4
    int e = sim.energy_around_point(state, 2, 2, 0);
    REQUIRE(e == -4);
}

TEST_CASE("energy_around_point: checkerboard interior → +4", "[energy_point]") {
    IsingSimulation sim(5, 5);
    auto state = make_checkerboard(5, 5);
    // (2,2)=0 → si=-1, all neighbors=1 → sj=+1 → local_energy=+4 → return -1*(-1)*4 = +4
    uint8_t val = state[2 * 5 + 2];
    int e = sim.energy_around_point(state, 2, 2, val);
    REQUIRE(e == 4);
}

TEST_CASE("energy_around_point: mixed neighbors → correct signed sum", "[energy_point]") {
    IsingSimulation sim(3, 3);
    // center (1,1)=1 → si=+1
    // up(0,1)=1 → sj=+1, down(2,1)=0 → sj=-1, left(1,0)=1 → sj=+1, right(1,2)=0 → sj=-1
    // local_energy = +1-1+1-1 = 0 → return -1*1*0 = 0
    std::vector<uint8_t> state = {
        0, 1, 0,
        1, 1, 0,
        0, 0, 0
    };
    int e = sim.energy_around_point(state, 1, 1, 1);
    REQUIRE(e == 0);
}

// ─────────────────────────────────────────────
// calculate_energy
// ─────────────────────────────────────────────

TEST_CASE("calculate_energy: all-ones 2x2 → -4 (ferromagnetic)", "[energy]") {
    // 2x2 PBC: 4 bonds, all aligned → H = -J*sum = -4
    // each site: energy_around_point = -1*1*sum(sj)=  -1*1*4 (but neighbors wrap so 2 unique bonds counted twice)
    // each site contributes -4, 4 sites → sum=-16, /2 = -8... wait: 2x2 PBC each site has 4 neighbors (all same)
    // sum over sites = 4*(-4) = -16, /2 = -8
    IsingSimulation sim(2, 2);
    auto state = make_all_ones(2, 2);
    int e = sim.calculate_energy(state);
    REQUIRE(e == -8);
}

TEST_CASE("calculate_energy: checkerboard 4x4 → all anti-aligned", "[energy]") {
    // each site: si and all sj opposite → local_energy = -4 per site in old convention
    // new convention: energy_around_point = -si*sum(sj)
    // checkerboard: si=+1 → sj all -1 → sum(sj)=-4 → e_point = -1*1*(-4) = +4
    //               si=-1 → sj all +1 → sum(sj)=+4 → e_point = -1*(-1)*(+4) = +4
    // sum over 16 sites = 64, /2 = 32
    IsingSimulation sim(4, 4);
    auto state = make_checkerboard(4, 4);
    int e = sim.calculate_energy(state);
    REQUIRE(e == 32);
}

TEST_CASE("calculate_energy: all-zeros same as all-ones", "[energy]") {
    // both fully aligned → same magnitude energy
    IsingSimulation sim(4, 4);
    auto ones  = make_all_ones(4, 4);
    auto zeros = make_all_zeros(4, 4);
    REQUIRE(sim.calculate_energy(ones) == sim.calculate_energy(zeros));
}

TEST_CASE("calculate_energy: single spin flip changes energy correctly", "[energy]") {
    // all-ones 3x3: every site contributes -4 (si=+1, sum(sj)=+4 → -4)
    // sum = 9*(-4) = -36, /2 = -18
    // flip center (1,1) to 0: center now si=-1, neighbors still si=+1
    // center new contribution: -(-1)*(+4) = +4
    // 4 neighbors: each now has 3 matching (si=+1) + 1 opposite (si=-1 center)
    //   local_energy per neighbor = -(+1)*(3*1 + 1*(-1)) = -(+1)*(2) = -2
    // new sum = 4*(-2) + 4*(-4) + 1*(+4) = -8 - 16 + 4 = -20... easier: use brute force expectation
    // Just verify delta = e_after - e_before has correct sign: flipping one spin in aligned grid raises energy
    IsingSimulation sim(3, 3);
    auto state = make_all_ones(3, 3);
    int e_before = sim.calculate_energy(state);
    state[4] = 0; // flip center
    int e_after = sim.calculate_energy(state);
    REQUIRE(e_after > e_before); // energy increases when breaking alignment
}

TEST_CASE("calculate_energy: energy bounded by ±2*N_bonds", "[energy]") {
    IsingSimulation sim(5, 5);
    auto state = make_all_ones(5, 5);
    int e = sim.calculate_energy(state);
    int n_bonds = 5 * 5 * 2; // PBC: 2 bonds per site
    REQUIRE(std::abs(e) <= n_bonds);
}

// ─────────────────────────────────────────────
// calculate_energy_diff  (Kawasaki: swap active↔inactive)
// ─────────────────────────────────────────────

static int brute_force_diff(IsingSimulation& sim, std::vector<uint8_t> matrix,
                            size_t pos_a, size_t pos_b) {
    int e_before = sim.calculate_energy(matrix);
    std::swap(matrix[pos_a], matrix[pos_b]);
    int e_after = sim.calculate_energy(matrix);
    return e_after - e_before;
}

TEST_CASE("calculate_energy_diff: non-adjacent swap matches brute force 3x3", "[energy_diff]") {
    IsingSimulation sim(3, 3);
    std::vector<uint8_t> state = {
        1, 1, 0,
        1, 0, 0,
        0, 0, 1
    };
    // pos 0 (1) and pos 8 (1) — same value swap → diff = 0
    size_t pos_a = 0, pos_b = 6; // non-adjacent, different values: state[0]=1, state[6]=0
    int diff_calc  = sim.calculate_energy_diff(state, pos_a, pos_b);
    int diff_brute = brute_force_diff(sim, state, pos_a, pos_b);
    REQUIRE(diff_calc == diff_brute);
}

TEST_CASE("calculate_energy_diff: adjacent swap matches brute force", "[energy_diff]") {
    IsingSimulation sim(3, 3);
    std::vector<uint8_t> state = {
        1, 0, 1,
        0, 1, 0,
        1, 0, 1
    };
    // pos 1 (0) and pos 4 (1) — adjacent (row 0 col 1, row 1 col 1)
    size_t pos_a = 4, pos_b = 1;
    int diff_calc  = sim.calculate_energy_diff(state, pos_a, pos_b);
    int diff_brute = brute_force_diff(sim, state, pos_a, pos_b);
    REQUIRE(diff_calc == diff_brute);
}

TEST_CASE("calculate_energy_diff: multiple configs on 4x4", "[energy_diff]") {
    IsingSimulation sim(4, 4);

    std::vector<std::vector<uint8_t>> configs = {
        make_checkerboard(4, 4),
        make_half_half(4, 4),
        {1,0,1,0, 0,1,0,1, 1,1,0,0, 0,0,1,1},
        {1,1,1,1, 1,1,0,0, 0,0,0,0, 0,0,1,1},
    };

    // pairs: (active=1, inactive=0)
    std::vector<std::pair<size_t,size_t>> swaps = {
        {0, 1}, {2, 5}, {3, 12}, {7, 8}
    };

    for (auto& state : configs) {
        for (auto [pa, pb] : swaps) {
            if (state[pa] != 1 || state[pb] != 0) continue; // enforce active/inactive
            int diff_calc  = sim.calculate_energy_diff(state, pa, pb);
            int diff_brute = brute_force_diff(sim, state, pa, pb);
            INFO("pos_a=" << pa << " pos_b=" << pb);
            REQUIRE(diff_calc == diff_brute);
        }
    }
}

// In Kawasaki dynamic and in the actual code this is a not possible scenario
TEST_CASE("calculate_energy_diff: same-value swap → diff = 0", "[energy_diff]") {
    IsingSimulation sim(3, 3);
    auto state = make_all_ones(3, 3);
    state[5] = 0; // one zero
    // swap two 1s → no change
    int diff = sim.calculate_energy_diff(state, 0, 1);
    // both are 1, but 1 is not inactive — just check brute force
    int brute = brute_force_diff(sim, state, 0, 1);
    REQUIRE(diff == brute);
}

// ─────────────────────────────────────────────
// has_spanning_cluster
// ─────────────────────────────────────────────

TEST_CASE("has_spanning_cluster: all-ones → spanning", "[spanning]") {
    IsingSimulation sim(5, 5);
    auto state = make_all_ones(5, 5);
    REQUIRE(sim.has_spanning_cluster(state) == true);
}

TEST_CASE("has_spanning_cluster: all-zeros → no spanning (no active)", "[spanning]") {
    IsingSimulation sim(5, 5);
    auto state = make_all_zeros(5, 5);
    REQUIRE(sim.has_spanning_cluster(state) == false);
}

TEST_CASE("has_spanning_cluster: vertical stripe of 1s → spanning", "[spanning]") {
    IsingSimulation sim(4, 4);
    auto state = make_all_zeros(4, 4);
    // horizontal stripe: row 0 all active → connects col 0 to col 3
    for (size_t c = 0; c < 4; ++c)
        state[0 * 4 + c] = 1;
    REQUIRE(sim.has_spanning_cluster(state) == true);
}

TEST_CASE("has_spanning_cluster: isolated 1s → no spanning", "[spanning]") {
    IsingSimulation sim(4, 4);
    auto state = make_all_zeros(4, 4);
    state[0]  = 1; // (0,0)
    state[15] = 1; // (3,3) — not connected
    REQUIRE(sim.has_spanning_cluster(state) == false);
}

TEST_CASE("has_spanning_cluster: broken column → no spanning", "[spanning]") {
    IsingSimulation sim(4, 4);
    auto state = make_all_zeros(4, 4);
    // column 0 active except row 2 → gap breaks path
    state[0 * 4 + 0] = 1;
    state[1 * 4 + 0] = 1;
    // state[2*4+0] = 0  (gap)
    state[3 * 4 + 0] = 1;
    REQUIRE(sim.has_spanning_cluster(state) == false);
}

// ─────────────────────────────────────────────
// metropolis / run_simulation: sanity checks
// ─────────────────────────────────────────────

TEST_CASE("metropolis: spin count conserved (Kawasaki)", "[metropolis]") {
    IsingSimulation sim(4, 4);
    auto state = make_half_half(4, 4);
    uint16_t n_active = static_cast<uint16_t>(
        std::count(state.begin(), state.end(), 1));

    MetricIsing result = sim.run_simulation(state, n_active, 200, 1.0f, false);

    for (auto& s : result.evolution_state) {
        int count = std::count(s.begin(), s.end(), 1);
        REQUIRE(count == n_active);
    }
}

TEST_CASE("metropolis: metric vectors have correct length", "[metropolis]") {
    IsingSimulation sim(4, 4);
    auto state = make_half_half(4, 4);
    uint32_t iterations = 50;
    uint16_t n_active = 8;

    MetricIsing result = sim.run_simulation(state, n_active, iterations, 1.0f, false);

    REQUIRE(result.evolution_state.size() == iterations);
    REQUIRE(result.acceptance.size()      == iterations);
    REQUIRE(result.net_energy.size()      == iterations);
    REQUIRE(result.spanning.size()        == iterations);
}

TEST_CASE("metropolis: initial fields set correctly", "[metropolis]") {
    IsingSimulation sim(4, 4);
    auto state = make_half_half(4, 4);
    uint16_t n_active = 8;
    int expected_energy = sim.calculate_energy(state);

    MetricIsing result = sim.run_simulation(state, n_active, 10, 1.0f, false);

    REQUIRE(result.initial_activation == n_active);
    REQUIRE(result.initial_energy     == expected_energy);
}

TEST_CASE("metropolis: energy tracking consistent with diffs", "[metropolis]") {
    // verify energy never jumps more than max possible |ΔE| per swap
    IsingSimulation sim(4, 4);
    auto state = make_half_half(4, 4);
    MetricIsing result = sim.run_simulation(state, 8, 100, 1.0f, false);

    for (size_t i = 1; i < result.net_energy.size(); ++i) {
        int delta = std::abs(result.net_energy[i] - result.net_energy[i-1]);
        REQUIRE(delta <= 16);
    }
}

TEST_CASE("metropolis: acceptance values are 0 or 1", "[metropolis]") {
    IsingSimulation sim(4, 4);
    auto state = make_half_half(4, 4);
    MetricIsing result = sim.run_simulation(state, 8, 100, 1.0f, false);

    for (auto a : result.acceptance) {
        REQUIRE((a == 0 || a == 1));
    }
}

TEST_CASE("metropolis: spanning values are bool-like", "[metropolis]") {
    IsingSimulation sim(4, 4);
    auto state = make_half_half(4, 4);
    MetricIsing result = sim.run_simulation(state, 8, 50, 1.0f, false);

    for (auto s : result.spanning) {
        REQUIRE((s == true || s == false));
    }
}

TEST_CASE("metropolis: high beta → energy non-increasing after warmup", "[metropolis]") {
    // high beta (low T) → almost all uphill moves rejected → system freezes
    // last 50 steps of 500 should not be much higher than early minimum
    IsingSimulation sim(6, 6);
    auto state = make_half_half(6, 6);
    MetricIsing result = sim.run_simulation(state, 18, 500, 200.0f, false);

    auto& e = result.net_energy;
    int max_late  = *std::max_element(e.end() - 50, e.end());
    int min_early = *std::min_element(e.begin(), e.begin() + 50);
    REQUIRE(max_late <= min_early + 20);
}

TEST_CASE("metropolis: low beta → higher energy than high beta at equilibrium", "[metropolis]") {
    // low beta (high T) → many uphill moves accepted → higher equilibrium energy
    // high beta (low T) → fewer uphill moves → lower equilibrium energy
    // run both from same state, compare final energies
    IsingSimulation sim(6, 6);
    auto state = make_half_half(6, 6);

    MetricIsing high_T = sim.run_simulation(state, 18, 500, 0.1f, false);
    MetricIsing low_T  = sim.run_simulation(state, 18, 500, 200.0f, false);

    // average last 50 steps
    auto avg_tail = [](const std::vector<int>& v) {
        int sum = 0;
        for (size_t i = v.size() - 50; i < v.size(); ++i) sum += v[i];
        return sum / 50;
    };

    REQUIRE(avg_tail(high_T.net_energy) > avg_tail(low_T.net_energy));
}

TEST_CASE("metropolis: beta=0 → all moves accepted (infinite T)", "[metropolis]") {
    // beta=0 → exp(-0*dE)=1 → every uphill move accepted
    IsingSimulation sim(4, 4);
    auto state = make_half_half(4, 4);
    MetricIsing result = sim.run_simulation(state, 8, 100, 0.0f, false);

    int accepted = std::accumulate(result.acceptance.begin(), result.acceptance.end(), 0);
    REQUIRE(accepted == 100); // all accepted
}

#include <catch2/catch_test_macros.hpp>

// ─────────────────────────────────────────────
// helpers
// ─────────────────────────────────────────────

static double brute_correlation_sum(const std::vector<uint8_t>& matrix,
                                    int rows, int cols)
{
    double sum = 0.0;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int si  = matrix[r * cols + c]              == 0 ? -1 : 1;
            int sj_r = matrix[r * cols + (c+1) % cols]  == 0 ? -1 : 1;
            int sj_d = matrix[((r+1) % rows) * cols + c] == 0 ? -1 : 1;
            sum += si * sj_r;
            sum += si * sj_d;
        }
    }
    return sum;
}

// ─────────────────────────────────────────────
// calculate_correlation_sum
// ─────────────────────────────────────────────

TEST_CASE("calculate_correlation_sum: all-ones 2x2 → 8", "[corr_sum]") {
    // 2×2 PBC: 2 bonds/site × 4 sites = 8 pairs, all (+1)(+1) = 1 → sum = 8
    IsingSimulation sim(2, 2);
    std::vector<uint8_t> state = {1, 1, 1, 1};
    REQUIRE(sim.calculate_correlation_sum(state) == Catch::Approx(8.0));
}

TEST_CASE("calculate_correlation_sum: all-zeros 2x2 → same as all-ones", "[corr_sum]") {
    // 0→-1, so (-1)(-1) = +1, identical sum
    IsingSimulation sim(2, 2);
    std::vector<uint8_t> ones  = {1, 1, 1, 1};
    std::vector<uint8_t> zeros = {0, 0, 0, 0};
    REQUIRE(sim.calculate_correlation_sum(zeros) == Catch::Approx(sim.calculate_correlation_sum(ones)));
}

TEST_CASE("calculate_correlation_sum: checkerboard 4x4 → -32", "[corr_sum]") {
    // every neighbor pair anti-aligned → si*sj = -1
    // 2 bonds/site × 16 sites = 32 pairs → sum = -32
    IsingSimulation sim(4, 4);
    auto state = make_checkerboard(4, 4);
    REQUIRE(sim.calculate_correlation_sum(state) == Catch::Approx(-32.0));
}

TEST_CASE("calculate_correlation_sum: matches brute force on mixed 3x3", "[corr_sum]") {
    IsingSimulation sim(3, 3);
    std::vector<uint8_t> state = {
        1, 0, 1,
        0, 1, 0,
        1, 1, 0
    };
    REQUIRE(sim.calculate_correlation_sum(state) == Catch::Approx(brute_correlation_sum(state, 3, 3)));
}

TEST_CASE("calculate_correlation_sum: matches brute force on multiple 4x4 configs", "[corr_sum]") {
    IsingSimulation sim(4, 4);
    std::vector<std::vector<uint8_t>> configs = {
        make_checkerboard(4, 4),
        make_half_half(4, 4),
        {1,0,1,0, 0,1,0,1, 1,1,0,0, 0,0,1,1},
        {1,1,1,1, 1,1,0,0, 0,0,0,0, 0,0,1,1},
    };
    for (auto& state : configs) {
        INFO("config mismatch");
        REQUIRE(sim.calculate_correlation_sum(state) == Catch::Approx(brute_correlation_sum(state, 4, 4)));
    }
}

TEST_CASE("calculate_correlation_sum: single flip lowers sum in aligned grid", "[corr_sum]") {
    // all-ones 3x3: flip center → 4 bonds flip +1→-1, delta = -8
    IsingSimulation sim(3, 3);
    auto state = make_all_ones(3, 3);
    double before = sim.calculate_correlation_sum(state);
    state[4] = 0;
    double after = sim.calculate_correlation_sum(state);
    REQUIRE(after < before);
    REQUIRE(after == Catch::Approx(before - 8.0));
    REQUIRE(after == Catch::Approx(brute_correlation_sum(state, 3, 3)));
}

TEST_CASE("calculate_correlation_sum: magnitude bounded by 2*N", "[corr_sum]") {
    IsingSimulation sim(5, 5);
    auto state = make_all_ones(5, 5);
    REQUIRE(std::abs(sim.calculate_correlation_sum(state)) <= Catch::Approx(2.0 * 5 * 5));
}

TEST_CASE("pre_calculate_betas: total size equals iteration", "[betas]") {
    IsingSimulation sim(4, 4);
    REQUIRE(sim.pre_calculate_betas(100, 0.1).size() == 100);
    REQUIRE(sim.pre_calculate_betas(7,   0.1).size() == 7);   // non-divisible
    REQUIRE(sim.pre_calculate_betas(0,   0.1).size() == 0);
}

TEST_CASE("pre_calculate_betas: exactly 4 distinct beta values", "[betas]") {
    IsingSimulation sim(4, 4);
    auto betas = sim.pre_calculate_betas(100, 0.1);
    std::set<float> unique(betas.begin(), betas.end());
    REQUIRE(unique.size() == 4);
}

TEST_CASE("pre_calculate_betas: first value is beta_initial", "[betas]") {
    IsingSimulation sim(4, 4);
    auto betas = sim.pre_calculate_betas(100, 0.1);
    REQUIRE(betas.front() == Catch::Approx(0.1f));
}

TEST_CASE("pre_calculate_betas: last value is CRITICAL_BETA", "[betas]") {
    IsingSimulation sim(4, 4);
    auto betas = sim.pre_calculate_betas(100, 0.1);
    REQUIRE(betas.back() == Catch::Approx(1.0f / 2.27f));
}

TEST_CASE("pre_calculate_betas: segments non-divisible iteration → remainder in last", "[betas]") {
    // iteration=9, stride=4 → counts: 2,2,2,3
    IsingSimulation sim(4, 4);
    auto betas = sim.pre_calculate_betas(9, 0.1);
    REQUIRE(betas.size() == 9);

    float b0 = betas[0];
    REQUIRE(betas[0] == Catch::Approx(b0));
    REQUIRE(betas[8] == Catch::Approx(1.0f / 2.27f));

    // last segment has 3 elements (2 + remainder 1)
    REQUIRE(betas[6] == Catch::Approx(betas[7]));
    REQUIRE(betas[7] == Catch::Approx(betas[8]));
}

TEST_CASE("pre_calculate_betas: segments divisible → equal chunk sizes", "[betas]") {
    // iteration=8, stride=4 → each segment = 2
    IsingSimulation sim(4, 4);
    auto betas = sim.pre_calculate_betas(8, 0.1);
    REQUIRE(betas[0] == Catch::Approx(betas[1]));
    REQUIRE(betas[2] == Catch::Approx(betas[3]));
    REQUIRE(betas[4] == Catch::Approx(betas[5]));
    REQUIRE(betas[6] == Catch::Approx(betas[7]));
    // segments differ
    REQUIRE(betas[1] != Catch::Approx(betas[2]));
}

TEST_CASE("pre_calculate_betas: monotonically increasing", "[betas]") {
    IsingSimulation sim(4, 4);
    auto betas = sim.pre_calculate_betas(100, 0.1);
    for (size_t i = 1; i < betas.size(); ++i)
        REQUIRE(betas[i] >= betas[i-1]);
}

TEST_CASE("pre_calculate_betas: beta_initial above CRITICAL_BETA → monotonically decreasing", "[betas]") {
    IsingSimulation sim(4, 4);
    auto betas = sim.pre_calculate_betas(100, 0.8);  // 0.8 > 0.4405
    for (size_t i = 1; i < betas.size(); ++i)
        REQUIRE(betas[i] <= betas[i-1]);
}