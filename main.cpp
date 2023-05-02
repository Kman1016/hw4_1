#include "mbed.h"
#include "drivers/DigitalOut.h"

#include "erpc_simple_server.hpp"
#include "erpc_basic_codec.hpp"
#include "erpc_crc16.hpp"
#include "UARTTransport.h"
#include "DynamicMessageBufferFactory.h"
#include "TextLCD.h"
#include "lcdtext_erpc_server.h"

ep::UARTTransport uart_transport(D1, D0, 9600);
ep::DynamicMessageBufferFactory dynamic_mbf;
erpc::BasicCodecFactory basic_cf;
erpc::Crc16 crc16;
erpc::SimpleServer rpc_server;

I2C i2c_lcd(D14, D15); // SDA, SCL
//TextLCD_SPI lcd(&spi_lcd, p8, TextLCD::LCD40x4);   // SPI bus, 74595 expander, CS pin, LCD Type
TextLCD_I2C lcd(&i2c_lcd, 0x4E, TextLCD::LCD16x2);   // I2C bus, PCF8574 Slaveaddress, LCD Type
                                                     //TextLCD_I2C lcd(&i2c_lcd, 0x42, TextLCD::LCD16x2, TextLCD::WS0010);
                                                     // I2C bus, PCF8574 Slaveaddress, LCD Type, Device Type
                                                     //TextLCD_SPI_N lcd(&spi_lcd, p8, p9);
                                                     // SPI bus, CS pin, RS pin, LCDType=LCD16x2, BL=NC, LCDTCtrl=ST7032_3V3
//TextLCD_I2C_N lcd(&i2c_lcd, ST7032_SA, TextLCD::LCD16x2, NC, TextLCD::ST7032_3V3);
// I2C bus, Slaveaddress, LCD Type, BL=NC, LCDTCtrl=ST7032_3V3
// main() runs in its own thread in the OS
LCDService_service lcd_service;

void loc(int col, int row){
    lcd.locate(col, row);
}

void cls(){
    lcd.cls();
}

int row(){
    lcd.printf("row: %d\n", lcd.rows());
    return lcd.rows();
}

int col(){
    lcd.printf("column: %d\n", lcd.columns());
    return lcd.columns();
}

void put(int c){
    lcd.printf("%d",c);
}

int main(void) {

  // Initialize the rpc server
  uart_transport.setCrc16(&crc16);

  printf("Initializing server.\n");
  rpc_server.setTransport(&uart_transport);
  rpc_server.setCodecFactory(&basic_cf);
  rpc_server.setMessageBufferFactory(&dynamic_mbf);

  // Add the led service to the server
  printf("Adding LCD server.\n");
  rpc_server.addService(&lcd_service);

  // Run the server. This should never exit
  printf("Running server.\n");
  rpc_server.run();
}