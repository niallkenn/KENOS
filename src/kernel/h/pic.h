#include "portio.h"
#include "definitions.h"

class Pic {
    public:
        Pic() = delete;

        static void initialize();
        static void send_eoi(uint8_t irq);
};