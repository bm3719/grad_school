/* file: 1.c
 * programmers: bruce miller, michael schultz
 * purpose: bump left: turn right, bump right: turn left
 */

void main() {
    while (1) {
        fd(0);                /* full fwd */
        fd(1);
        if (digital(14)) {    /* if right hit */
            bk(0);            /* full back */
            bk(1);
            sleep(0.8);
            fd(1);            /* left turn */
            sleep(0.5);
        }
        if (digital(15)) {    /* if left hit */
            bk(0);            /* full back */
            bk(1);
            sleep(0.8);
            fd(0);            /* right turn */
            sleep(0.5);
        }
    }
}

