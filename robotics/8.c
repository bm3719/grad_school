int main() {
    while(1) {
        motor(0, 25);
        motor(1, 25);
        printf("%d\n", analog(0));
        if (analog(0) > 180) {
            right_turn();
        }
        if (analog(1) > 180) {
            left_turn();
        }
    }
}

void right_turn() {
    ao();
    while(1) {
        motor(0, 50);
        if (analog(1) > 180) {
            ao();
            motor(0, -50);
            sleep(0.5);
            break;
        }
    }
}

void left_turn() {
    ao();
    while(1) {
        motor(1, 50);
        if (analog(0) > 180) {
            ao();
            motor(1, -50);
            sleep(0.5);
            break;
        }
    }
}


