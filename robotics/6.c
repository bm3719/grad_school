int main() {
    int numticks5ft = 11;
    int counter = 0;
    while(counter < numticks5ft) {
        fd(1);
        fd(0);
        if (digital(15)) {
            beep();
            counter++;
            sleep(0.2);
        }
    }
    sleep(0.5);
    ao();
}
