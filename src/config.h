#pragma once
#include <cstdint>
#include <algorithm>


typedef uint16_t u16;

class ProbabilityRange {
public:
    ProbabilityRange(u16 from, u16 to, u16 step_number): 
                    from(from), to(to), step_number(step_number-1){};
    ~ProbabilityRange() = default;

    u16 scaled_prob_from() const { return scale(from); }
    u16 scaled_prob_to() const { return scale(to); }
    
    u16 get_from() const { return from; }
    u16 get_to() const { return to; }
    u16 prob_step() const { return (scale(to) - scale(from)) / step_number; };

private:
    const u16 max = std::numeric_limits<u16>::max();
    u16 from;
    u16 to;
    u16 step_number;
    int scale(int prob) const {
        return static_cast<u16>((static_cast<double>(prob) / 100.0) * max);
    };
};


class Configuration{
public:
    Configuration(unsigned int sim_number, ProbabilityRange prob_range);
    ~Configuration() = default;

    unsigned int get_simulation_number() const { return sim_number; }
    const ProbabilityRange& get_probability_range() const { return prob_range; }

private:
    unsigned int sim_number;
    ProbabilityRange prob_range;
};