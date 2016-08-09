#include "mbed.h"




int main() {
	DigitalOut myled(PF_2);
	DigitalOut fail(PF_1);
	DigitalIn swit(PF_4,PullUp);
	int read_val;
    while(1) {

    	if(swit == 0)
    	{
    		myled = 0;
    		fail = 0;
    	}
    	else if(swit== 16)
    	{
    		myled = 1;
    		fail = 0;
    	}
    	else
    	{
    		fail = 1;
    	}
    }
}
