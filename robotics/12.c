/* bruce miller, michael schultz
 * purpose: tape following & object avoidance using potential field approach
 * notes: vector degrees are set to be equivalent to the turn degree made by 
 * vector_turn(). magnitude profiles are set seperately from the degree
 * component in code, but occur in conjunction when necessary to respond to 
 * external stimulus.
 */

int main() {
    int speed = 100;                /* magnitude component of vector */
    int vector;                     /* degree component of vector */
    int off_tape = 0;               /* object/tape mode */
    int turn_in_progress = 0;       /* delays object edge following */
    while(1) {
        motor(0, speed);            /* set magnitude */
        motor(1, speed);

        /* front object ir */
        if (analog(3) < 225) {  
            vector = 90;
            speed = 85;
            off_tape = 1;
            turn_in_progress = 1;
        } else if (analog(2) > 240 && off_tape) {
            /* side object ir too far */
            vector = -10;
        } else if (analog(2) < 180 && off_tape) {
            /* side object ir too close */
            vector = 18;
        }
        /* returns to tape following */
        if (off_tape && !turn_in_progress) {
            /* right tape ir */
            if (analog(1) > 160) {
                speed = 100;
                beep();
                vector = 70;
                off_tape = 0;
            }

        }

        /* tape following code */
        if (!off_tape && !turn_in_progress)
        {
            /* right tape ir */
            if (analog(1) > 160) {
                vector = 60;
            }
            /* left tape ir */
            if (analog(0) > 160) {
                vector = -60;
            }
        }

        /* reset the vector */
        if (vector != 0) {
            vector_turn(vector);
            vector = 0;
            turn_in_progress = 0;
        }
    }
}

/* handles directional component of vector */
void vector_turn(int vector) {
    /* converts vector into a float for sleep() */
    float sleep_t = ((float) vector) / ((float) 100);
    ao();
    printf("%f\n", sleep_t);
    /* positive vector */
    if (vector > 0) {
        motor(0, 65);
        motor(1, -40);
        sleep(sleep_t);
    } else if (vector < 0) {
        /* negative vector */
        motor(1, 65);
        motor(0, -40);
        sleep(0.0 - sleep_t);
    }
}


