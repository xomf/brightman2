#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "utils.h"
#include "blmgr.h"

int main( int argc, char* argv[] ) {

    // TODO: Implement backlight switching, maybe
    backlights_t* backlights = get_backlights();

    if (!backlights) {
        return 1;
    }

    size_t bl_id = 0;

    if (argc > 1) {
        size_t level = atoi(argv[1]);
        if (level >= 0 && level <= 100) {
            set_backlight_level(&backlights->items[bl_id], level);
            free_backlights(backlights);
            return 0;
        } else {
            free_backlights(backlights);
            eprintf("ERROR: Invalid level\n");
            return 1;
        }
    }

    static struct termios old_t, new_t;
    tcgetattr( STDIN_FILENO, &old_t);
    new_t = old_t;
    new_t.c_lflag &= ~(ICANON);
    new_t.c_lflag &= ~(ICANON | ECHO);
    tcsetattr( STDIN_FILENO, TCSANOW, &new_t);

    puts("w -- increase brightness");
    puts("s -- decrease brightness");
    puts("q -- quit");

    size_t step = 5;
    size_t level = 0;
    char c;
    while( 1 ) {
        c = getchar();
        switch (c) {
            case 'q':
                tcsetattr( STDIN_FILENO, TCSANOW, &old_t);
                free_backlights(backlights);
                return 0;
                break;
            case 'w':
                level = get_backlight_level(&backlights->items[bl_id]);
                if (level + step <= 100 ) {
                    set_backlight_level(&backlights->items[bl_id], level + step);
                }
                break;
            case 's':
                level = get_backlight_level(&backlights->items[bl_id]);
                if (level >= step ) {
                    set_backlight_level(&backlights->items[bl_id], level - step);
                }
                break;
        }
    }
}
