#include "definitions.h"

direction left_of(direction_t d) { return (direction)((d + 1) % 4); }

direction right_of(direction_t d) { return (direction)((d + 3) % 4); }

direction opposite(direction_t d) { return (direction)((d + 2) % 4); }