int main() {
    int count = 0;
    while(1) {
        motor(0, 100);
        motor(1, 40);   
        printf("%d\n", analog(0));
        if (analog(0) < 100) {
            right_turn();
        }
        if (analog(0) > 240) {
            left_turn();
        }
        if (analog(1) < 180) {
            big_right_turn();
        }
    }
    ao();
}

void right_turn() {
    ao();
    motor(1, 50);
    sleep(0.4);
}

void big_right_turn() {
    ao();
    motor( 1, 50);
    motor( 0, -50);
    sleep(1.5);
}

void left_turn() {
    ao();
    motor(0, 50);
    sleep(0.4);
}

