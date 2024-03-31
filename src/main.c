#include <stdio.h>
#include <termios.h>
#include <unistd.h>


int main( int argc, char* argv[] ) {

  if (argc != 1) {
    // TODO: logic for setting brightness based on a percentage, specified in arguments.
    return 0;
  }

  int c;
  static struct termios old_t, new_t;
  tcgetattr( STDIN_FILENO, &old_t);
  new_t = old_t;
  new_t.c_lflag &= ~(ICANON);
  new_t.c_lflag &= ~(ICANON | ECHO);
  tcsetattr( STDIN_FILENO, TCSANOW, &new_t);
  
  puts("w -- increase brightness");
  puts("s -- decrease brightness");
  puts("q -- quit");

  while( 1 ) {
    c = getchar();
    switch (c) {
      
      case 'q':
        tcsetattr( STDIN_FILENO, TCSANOW, &old_t);
        return 0;
        break;
      case 'w':
        // TODO: logic for increasing brightness.
        break;
      case 's':
        // TODO: logic for decreasing brightness.
        break;

    }
  }

  return 0;

}
