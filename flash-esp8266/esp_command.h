#ifndef ESP_COMMAND_H
#define ESP_COMMAND_H

#include "../flash-esp8266/slip.h"


#define FLASH_BLOCK_SIZE 0x400
#define RESET_WAIT_MS 250

enum esp_commands {
    ESP_FLASH_START     = 0x02,
    ESP_FLASH_DATA      = 0x03,
    ESP_FLASH_FINISH    = 0x04,
    ESP_RAM_START       = 0x05,
    ESP_RAM_DATA        = 0x07,
    ESP_RAM_FINISH      = 0x06,
    ESP_SYNC_FRAME      = 0x08,
    ESP_WRITE_REG       = 0x09,
    ESP_READ_REG        = 0x0a,
    ESP_SPI_CONFIG      = 0x0b,
};




// Commands follow the following format
// request:
// [- 0x00 -|- command -|- size -|- value -|-- body --]
//     1          1         2         4        size
//
// response:
// [- 0x01 -|- command -|- size -|- value -|-- body --|- status -|- error -]
//     1          1         2         4     size-error     1          1
//
template <typename SERIAL>
class ESPCommand {
private:
    SLIPPacket<SERIAL> _slip;
    DigitalIn _button;
    
public:
    ESPCommand(PinName tx, PinName rx, PinName button) : 
            _slip(tx, rx), _button(button){}
    
private:
    bool command_start(char cmd, uint16_t len) {
        len -= 4; // First word not included in length
        
        return _slip.req_start() &&
               _slip.putc(0x00) &&
               _slip.putc(cmd) &&
               _slip.send(&len, 2);
    }
    
    bool command_flush() {
        uint16_t len;

        return _slip.resp_start() &&
               _slip.getc() == 0x01 &&
               _slip.getc() >= 0 &&
               _slip.recv(&len, 2) &&
               _slip.recv(0, 4+len-2) &&
               _slip.getc() == 0x00 &&
               _slip.getc() >= 0 &&
               _slip.resp_finish();
    }
    
    bool command_finish() {
        return _slip.req_finish() && 
               command_flush();
    }
    
public:
    bool sync() {
#ifdef LED_STATUS
        led_green = 0; // Show progress
#endif
        while (true) {
            if (sync_frame()) {
#ifdef LED_STATUS
                led_green = 1; // Show progress
#endif
                return true;
            }
        }
    }

    bool sync_frame() {
        
        // Flush serial line
        _slip.flush();
        
        printf("\r\nPower cycle ESP, put into FW Update mode, push user button\r\n");
        int temp_button = _button;
        while( temp_button == _button){;} // wait for button press
        printf("\r\nContinuing Now\r\n");
        
        // Send sync frame
        uint32_t x = 0;
        
        if (!(command_start(ESP_SYNC_FRAME, 2*4 + 32) &&
              _slip.send(&x, 4) &&
              _slip.putc(0x07) &&
              _slip.putc(0x07) &&
              _slip.putc(0x12) &&
              _slip.putc(0x20)))
            return false;
            
        for (int i = 0; i < 32; i++) {
            if (!_slip.putc(0x55))
                return false;
        }
        
        if (!command_finish())
            return false;
        
        for (int i = 0; i < 7; i++) {
            if (!command_flush())
                return false;
        }
        
        return true;
    }
 
    bool flash_start(uint32_t blocks, uint32_t block_size, uint32_t address) {
        uint32_t x = 0;
        uint32_t size = blocks * block_size;
        
        return command_start(ESP_FLASH_START, 5*4) &&
               _slip.send(&x, 4) &&
               _slip.send(&size, 4) &&
               _slip.send(&blocks, 4) &&
               _slip.send(&block_size, 4) &&
               _slip.send(&address, 4) && 
               command_finish();
    }
    
    bool flash_data(const unsigned char *data, uint32_t len, uint32_t n) {
        uint32_t zero = 0;
        
        uint32_t x = 0xef;
        for (int i = 0; i < len; i++) {
            x ^= data[i];
        }
        
        return command_start(ESP_FLASH_DATA, 5*4 + len) &&
               _slip.send(&x, 4) &&
               _slip.send(&len, 4) &&
               _slip.send(&n, 4) &&
               _slip.send(&zero, 4) &&
               _slip.send(&zero, 4) &&
               _slip.send(data, len) &&
               command_finish();
    }

    bool flash_finish() {
        uint32_t x = 0;
        
        return command_start(ESP_FLASH_FINISH, 2*4) &&
               _slip.send(&x, 4) &&
               _slip.send(&x, 4) &&
               command_finish();
    }
    
    bool flash_write(uint32_t address, const unsigned char *data, uint32_t size) {
        uint32_t blocks = (size-1)/FLASH_BLOCK_SIZE + 1;
        
        printf("Synching...\r\n");
        if (!sync())
            error("Sync error!");
        
        printf("Starting transfer 0x%05x - 0x%05x\r\n", address, address + blocks*FLASH_BLOCK_SIZE);
        if (!flash_start(blocks, FLASH_BLOCK_SIZE, address))
            return false;
            
        for (int i = 0; i < blocks; i++) {
            printf("Flashing address 0x%05x\r\n", address + i*FLASH_BLOCK_SIZE);
            if (!flash_data(&data[i*FLASH_BLOCK_SIZE], FLASH_BLOCK_SIZE, i))
                return false;
        }
        
        printf("Finishing transfer\r\n");
        if (!flash_finish())
            return false;
            
        wait_ms(250);
        return true;
    }
};

#endif
