#ifndef SLIP_H
#define SLIP_H

// Packet wrapping using the SLIP protocol
template <typename SERIAL>
class SLIPPacket {
private:
    SERIAL _serial;
    
public:
    SLIPPacket(PinName tx, PinName rx) : _serial(tx, rx) {}
    
    int checkgetc() {
        Timer timer;
        timer.start();
        
        while (!_serial.readable()) {
            if (timer.read_ms() > 1500)
                return -1;
        }
            
        int c = _serial.getc();
#ifdef DEBUG
        printf("<- 0x%02x\r\n", c);
#endif
        return c;
    }
    
    int checkputc(unsigned char c) {
        wait_us(100);
        
        while (!_serial.writeable())
            ;

#ifdef DEBUG
        printf("-> 0x%02x\r\n", c);
#endif
        return _serial.putc(c);
    }
    
    void flush() {
        while (_serial.readable())
            _serial.getc();
    }

public:
    bool req_start() {
#ifdef LED_STATUS
        led_blue = 0; // Show progress
#endif
        return !(checkputc(0xc0) < 0);
    }
    
    bool req_finish() {
#ifdef LED_STATUS
        led_blue = 1; // Show progress
#endif
        return !(checkputc(0xc0) < 0);
    }
    
    bool putc(unsigned char c) {
        switch (c) {
            case 0xdb: return checkputc(0xdb) >= 0 && checkputc(0xdd) >= 0;
            case 0xc0: return checkputc(0xdb) >= 0 && checkputc(0xdc) >= 0;
            default:   return checkputc(c) >= 0;
        }
    }
    
    bool send(const void *data, int len) {
        for (int i = 0; i < len; i++) {
            if (!putc(((unsigned char *)data)[i]))
                return false;
        }
        
        return true;
    }
 
public:
    bool resp_start() {
        return checkgetc() == 0xc0;
    }
    
    bool resp_finish() {
        return checkgetc() == 0xc0;
    }
    
    int getc() {
        int c = checkgetc();
        
        if (c < 0) {
            return -1;
        } else if (c == 0xdb) {
            c = checkgetc();
            switch (c) {
                case 0xdd: return 0xdb;
                case 0xdc: return 0xc0;
                default:   return -1;
            }
        } else {
            return c;
        }
    }
    
    bool recv(void *resp, int len) {
        for (int i = 0; i < len; i++) {
            int c = getc();
            
            if (c < 0)
                return false;
            
            if (resp)
                ((unsigned char *)resp)[i] = c;
        }
        
        return true;
    }
};

#endif
