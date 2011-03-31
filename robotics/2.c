/* file: 2.c
 * programmers: bruce miller, michael schultz
 * purpose: added random variation to turn rate
 */

void main() {
    while (1) {
        fd(0);                      /* full fwd */
        fd(1);
        if (digital(14)) {          /* if right hit */
            bk(0);                  /* full back */
            bk(1);
            sleep(0.8);
            fd(1);                  /* turn left */
            if ( random(4) == 0) {  /* 1:4 random roll */
                beep();             /* audio beep */
                /* turn 0.5 - 1.5 sec */
                sleep((float) random(100)/100.0 + 0.5);
            } else {
                sleep(0.5);         /* normal turn */
            }
        }
        if (digital(15)) {          /* if left hit */
            bk(0);                  /* full back */
            bk(1);
            sleep(0.8);
            fd(0);                  /* turn right */
            if ( random(4) == 0) {  /* 1:4 random roll */
                beep();             /* audio beep */
                /* turn 0.5 - 1.5 sec */
                sleep((float) random(100)/100.0 + 0.5);
            } else {
                sleep(0.5);         /* normal turn */
            }
        }
    }
}

