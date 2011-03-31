int main() {
    int accum_v = 0; 
    int touch_v = 0;
    int side_ir_v = 0;
    int off_track = 0;
    int tape_ir_v = 0;

    while(1) {
        printf("%d\n", analog(1));

        /* left touch sensor */
        if (digital(14)) {
            backup_some();
            touch_v += 60;
            accum_v = 60;
            off_track = 1;
        }
        /* right touch sensor */
        if (digital(15)) {       
            backup_some();
            touch_v -= 60;
            accum_v = -60;
            off_track = 1;
        }
        /* left side ir */
        if (analog(2) < 100) {
            side_ir_v += 10;
        }
        /* right side ir */
        if (analog(3) < 100) {
            side_ir_v -= 10;
        }

        if (!off_track) {
            /* left tape ir */
            if (analog(0) > 180) {
                backup_some();
                tape_ir_v = -30;
                vector_turn(tape_ir_v);
            } else if (analog(1) > 180) {
                /* right tape ir */
                backup_some();
                tape_ir_v = 30;
                vector_turn(tape_ir_v);
            }
        }

        /* drive straight if no object vectors present */
        if (touch_v + side_ir_v + tape_ir_v == 0) {
            motor(0, 50);
            motor(1, 50);
            /* if off tape start heading back */
            if (off_track) {
                vector_turn(0 - accum_v);
                accum_v--;
            }
        }
        /* else do turn and move */
        else {
            vector_turn(touch_v + side_ir_v + tape_ir_v);
        }

        /* decrement touch vector */
        if (touch_v > 0) {
            touch_v--;
        } else if (touch_v < 0) {
            touch_v++;
        }       
        /* decrement ir vector */
        if (side_ir_v > 0) {
            side_ir_v--;
        } else if (side_ir_v < 0) {
            side_ir_v++;
        }

        /* delay... */
        sleep(0.1);
    }
    return(0);
}

void vector_turn(int v) { 
    ao();
    if (v > 0) {
        motor(1, 50 - v);
        motor(0, 50);
    } else if (v < 0) {
        motor(1, 50);
        motor(0, 50 + v);
    } else {
        fd(1);
        fd(0);
    }
}

void backup_some() {
    ao();
    motor(1, -70);
    motor(0, -70);
    sleep(0.5);
}

void track_scan() {
}
