#include "mbed.h"




int main() {
	DigitalOut myled(PF_2);
    while(1) {
        myled = 1;
        wait(0.001);
        myled = 0;
        wait(0.001);
    }
}
