/* file: 4.c
 * programmers: bruce miller, michael schultz
 * purpose: modify turn rate to follow walls
 */

void main() {
    while (1) {
        fd(0);                      /* full fwd */
        fd(1);
        if (digital(14)) {          /* if right hit */
            if ( random(4) == 0) {  /* 1:4 random roll */
                beep();             /* audio beep */
                random_avoid();     /* random right/left turn */
            } else {
                left_avoid();       /* normal left turn */
            }
        }
        if (digital(15)) {          /* if left hit */
            if ( random(4) == 0) {  /* 1:4 random roll */
                beep();             /* audio beep */
                random_avoid();     /* random right/left turn */
            } else {
                right_avoid();      /* normal right turn */
            }
        }
    }
}

/* turn random direction/duration */
void random_avoid() {
    bk(0);                          /* full back */
    bk(1);
    sleep(0.8);
    fd(random(1));                  /* random direction */
    sleep((float) random(100)/100.0 + 0.5); /* random duration */
}

/* normal left turn */
void left_avoid() {
    bk(0);                          /* full back */
    bk(1);
    sleep(0.8);
    fd(1);                          /* turn left */
    sleep(0.5);
}

/* normal right turn */
void right_avoid() {
    bk(0);                          /* full back */
    bk(1);
    sleep(0.8);
    fd(0);                          /* turn right */
    sleep(0.5);
}
