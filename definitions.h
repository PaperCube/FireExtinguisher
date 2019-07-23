#ifndef DEFINITIONS_FIRE_EXTINGUISHER_H
#define DEFINITIONS_FIRE_EXTINGUISHER_H

/*
 * Match function signature containing "direction" with regex quickly:
 * [*& ](\w+::)?\w+\((\w+\s+\w+\s*,\s*)*direction( \w+)?\s*\W
 */

enum direction { FORWARD, LEFTWARD, BACKWARD, RIGHTWARD };
typedef int direction_t;

#endif