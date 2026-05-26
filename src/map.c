#include "map.h"

struct _Map maps[MAP_COUNT] = {
    {{
        0xFFFF, 0x8801, 0x8801, 0x8871, 
        0x8001, 0x8001, 0x87E1, 0xCFE1, 
        0x87F3, 0x87E1, 0x8001, 0x8001, 
        0x9E11, 0x8011, 0x8011, 0xFFFF
    },(struct _Vector){2, 2},(struct _Vector){0, 1},
      (struct _Vector){13, 13},(struct _Vector){0, 1}}
};

struct _Map * MAP_SelectRandom() {
    return &maps[0];
}
