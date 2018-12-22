/* SDT-example-sensor-ccs811
 * 
 * Copyright (c) 2018 Sigma Delta Technologies Inc.
 * 
 * MIT License
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "mbed.h"
#include "CCS811.h"

/* Serial */
#define BAUDRATE 9600
Serial serial_pc(USBTX, USBRX, BAUDRATE);

/* Sensor */
CCS811 ccs811(I2C1_SDA, I2C1_SCL);
DigitalOut do_ccs811_wake(SPI1_SS0, 0);     // nWAKE pin is an active low input and should be asserted by the host prior to an I²C transaction and held low throughout.
uint16_t eco2, tvoc;
char status = 0;



void callbackCCS811(void) {
    status = ccs811.getStatus(false);       // 0x98 : There is a new data to be read
    while(!(status & 0x08));                // This bit is cleared when data is read on the I²C interface
    ccs811.getData(&eco2, &tvoc);
    serial_pc.printf("eCO2 reading :%dppm, TVOC reading :%dppb\r\n", eco2, tvoc);
    wait(1);
}

int main(void) {
    serial_pc.printf("< Sigma Delta Technologies Inc. >\n\r");

    serial_pc.printf("Status before init() : ");
    status = ccs811.getStatus(true);        // 0x10 : not ready to working
    
    ccs811.init();
    serial_pc.printf("Status after init() : ");
    status = ccs811.getStatus(true);        // 0x90 : reports everything working
    while(!(status & 0x90));
    serial_pc.printf("\n");
    wait(1);

    while(true) {
        wait(1);
        callbackCCS811();
    }
}
