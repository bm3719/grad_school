/* bruce miller, michael schultz
 * purpose: maze mapping using shaft-encoding 
 */

void main() { 
    int done = 0, i, cur_dist;
    int turn[50];
    int dist[50];

    for (i = 0; i < 50; i++) {
        turn[i] = 0;
        dist[i] = 0;
    }

    i = 0;
    cur_dist = 0;

    /* go straight until wall hit */
    while (analog(0) > 30 ) {
        fd(0);
        fd(1);
        if (digital(14)) {
            cur_dist++;
        }                       /* drive straight some */
        fd(0);
        fd(1);
        if (digital(14)) {
            cur_dist++;
            sleep(0.1);
        }
    }

    /* do right turn */
    right_turn();

    /* record first turn */
    turn[i] = 90;
    dist[i] = cur_dist;
    /* drive straight some */
    fd(0);
    fd(1);
    if (digital(14)) {
        cur_dist++;
        sleep(0.1);
    }

    i++;
    cur_dist = 0;

    while(!done) {
        /* ramming speed */
        fd(0);
        fd(1);

        /* wall following */
        printf ("%d\n", analog(0));
        if (analog(2) > 240) {
            left_turn10();
            turn[i] = -10;
            dist[i] = cur_dist;
            i++;
            cur_dist = 0;
            /* drive straight some */
            fd(0);
            fd(1);
            if (digital(14)) {
                cur_dist++;
                sleep(0.4);
            }
        } else if (analog(2) < 180) {
            right_turn10();
            turn[i] = 10;
            dist[i] = cur_dist;
            i++;
            cur_dist = 0;
            /* drive straight some */
            fd(0);
            fd(1);
            if (digital(14)) {
                cur_dist++;
                sleep(0.4);
            }
        }
                
        /* 90 degree turns if front ir */
        if (analog(0) < 30) {
            right_turn();
            turn[i] = 90;
            dist[i] = cur_dist;
            i++;
            cur_dist = 0;
        }

        /* record distance travelled since last turn  */
        if (digital(14)) {
            cur_dist++;
            sleep(0.1);
        } 
                
        /* reset turn counter if over limit */
        if ( i > 49 ) {
            i = 0;
        }

        /* if light detected */ 
        if (analog(4) < 80) {
            done = 1;
        }
    }
}

/* 10 degree right turn */
void right_turn10() {
    ao();
    motor(0, 52);
    motor(1, -52);
    sleep(0.22);
}

/* 10 degree left turn */
void left_turn10() {
    ao();
    motor(1, 52);
    motor(0, -52);
    sleep(0.22);
}

/* 90 degree right turn */
void right_turn() {
    ao();
    motor(0, 52);
    motor(1, -52);
    sleep(1.95);
}

/* 90 left turn */
void left_turn() {
    ao();
    motor(1, 52);
    motor(0, -52);
    sleep(1.95);
}
