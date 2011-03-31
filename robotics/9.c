int main() {
    int count = 0;
    while(count < 95) {
        fd(0);
        fd(1);
        printf("%d\n", analog(0));
        if (analog(0) > 95) {
            count++;
            sleep(0.2);
        }
    }
    ao();
}
