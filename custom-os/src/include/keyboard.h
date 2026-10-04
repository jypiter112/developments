#ifndef KEYBOARD_H
#define KEYBOARD_H

char kbd_poll(void); /* non-blocking, returns 0 if no key */
char kbd_getc(void); /* blocking */

#endif
