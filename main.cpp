#include "mbed.h"




int main() {
	DigitalOut myled(PF_1);
    while(1) {
        myled = 1;
        wait(10000);
        myled = 0;
        wait(1000);
    }
}
