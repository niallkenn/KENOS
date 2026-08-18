#include "definitions.h"

constexpr uint32_t PIT_BASE_FREQUENCY = 1193182;

constexpr uint16_t PIT_CHANNEL_0 = 0x40;
constexpr uint16_t PIT_COMMAND   = 0x43;

extern volatile uint32_t timer_ticks;

class Pit {
    public:
        Pit() = delete;

        static void initialize(uint32_t frequency);
};