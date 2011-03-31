/* file: 5.c
 * programmers: bruce miller, michael schultz
 * purpose: added turns twds light sources at random intervals
 */

float _timer;                       /* stores current time count */

void main() {
    int maxlight = -128, curlight;

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
        /* 2-6 sec random roll */
        if (timer() > (float) (random(4) + 2)) {
            if (read_left() > read_right()) {
                /* if left brighter */
                right_avoid();      /* normal right turn */
            } else if (read_right() > read_left()) {
                /* if right brighter */
                left_avoid();       /* normal left turn */
            }
            reset_timer();          /* re-init _timer */
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

/* init _timer */
void reset_timer() {
    _timer = seconds();
}

/* update current time */
float timer() {
    return seconds() - _timer;
}

/* read light on right side */
int read_right() {
    return analog(5);
}

/* read light on left side */
int read_left() {
    return analog(6);
}
