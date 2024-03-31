
#ifndef _H_BLMGR
#define _H_BLMGR

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

typedef struct {
    char* name;
    size_t max_brightness;
    FILE* brightness_fd;
} backlight_t;

typedef struct {
    backlight_t* items;
    size_t count;
    size_t capacity;
} backlights_t;

backlights_t* get_backlights();
backlight_t* get_backlight(backlights_t* backlights, char* name);
void free_backlights(backlights_t* backlights);
uint8_t get_backlight_level(backlight_t* backlight);
void set_backlight_level(backlight_t* backlight, uint8_t level);

#endif