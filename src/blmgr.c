#define _XOPEN_SOURCE 700
#define _GNU_SOURCE // stupid fucking vscode, always got me searching through libc
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <math.h>

#include "utils.h"
#include "blmgr.h"


backlights_t* get_backlights() {

    backlights_t* backlights = malloc(sizeof(backlights_t));
    DIR* dp = opendir("/sys/class/backlight/");

    if (!dp) {
        eprintf("ERROR: Couldn't open /sys/class/backlight/ ???????????\n");
        free(backlights);
        return NULL;
    }

    struct dirent* ep; 

    while ((ep = readdir (dp)) != NULL) {
        if (
            strcmp(ep->d_name, ".") == 0 ||
            strcmp(ep->d_name, "..") == 0
        ) {
            continue;
        }
        
        if (!(
            ep->d_type == DT_DIR || 
            ep->d_type == DT_LNK
        )) {
            eprintf("ERROR: /sys/class/backlight/%s is not a dir: %d\n", ep->d_name, ep->d_type);
            continue;
        }

        char bl_brightness_fn[256] = {0};
        char bl_max_brightness_fn[256] = {0};

        snprintf(bl_brightness_fn, 256, "/sys/class/backlight/%s/brightness", ep->d_name);
        snprintf(bl_max_brightness_fn, 256, "/sys/class/backlight/%s/max_brightness", ep->d_name);

        if (access(bl_brightness_fn, F_OK) != 0) {
            eprintf("ERROR: %s is not a file\n", bl_brightness_fn);
            continue;
        }

        if (access(bl_max_brightness_fn, F_OK) != 0) {
            eprintf("ERROR: %s is not a file\n", bl_max_brightness_fn);
            continue;
        }

        backlight_t backlight = {0};

        FILE* max_brightness_fd = fopen(bl_max_brightness_fn, "r");
        backlight.brightness_fd = fopen(bl_brightness_fn, "r+");
        backlight.name = ep->d_name;

        if (!max_brightness_fd) {
            eprintf("ERROR: couldnt open %s\n", bl_brightness_fn);
            continue;
        }

        if (!backlight.brightness_fd) {
            eprintf("ERROR: couldnt open %s, are you running as root?\n", bl_brightness_fn);
            continue;
        }

        fscanf(max_brightness_fd, "%d", &backlight.max_brightness);
        printf("Added backlight '%s'\n", backlight.name);
        bm_da_append(backlights, backlight);
    }

    if (backlights->count == 0) {
        eprintf("ERROR: No suitable backlights found\n");
        free(backlights);
        return NULL;
    }

    closedir(dp);
    return backlights;
}

backlight_t* get_backlight(backlights_t* backlights, char* name) {
    for (size_t i = 0; i < backlights->count; i++) {
        backlight_t backlight = backlights->items[i];
        if (strcmp(backlight.name, name) == 0) {
            return &backlights->items[i];
        }
    }
    return NULL;
}

uint8_t get_backlight_level(backlight_t* backlight) {
    size_t current_level_raw = 0;
    fseek(backlight->brightness_fd, 0, SEEK_SET);
    fscanf(backlight->brightness_fd, "%d", &current_level_raw);
    
    return (current_level_raw / (float_t)backlight->max_brightness) * 100;
}

void set_backlight_level(backlight_t* backlight, uint8_t level) {
    size_t step = backlight->max_brightness / 100;
    fseek(backlight->brightness_fd, 0, SEEK_SET);
    fprintf(backlight->brightness_fd, "%d", step * level);
}


void free_backlights(backlights_t* backlights) {
    for (size_t i = 0; i < backlights->count; i++) {
        backlight_t backlight = backlights->items[i];
        close(backlight.brightness_fd);
    }
    free(backlights->items);
    free(backlights);
}