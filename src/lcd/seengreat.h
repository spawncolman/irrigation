#ifndef SEENGREAT_H_
#define SEENGREAT_H_

#include "Arduino.h"
#include <stdint.h>

#define CUSTOMLCD_DIN_PIN 11U  // MOSI - SDI
#define CUSTOMLCD_CLK_PIN 12U  // CLK
#define CUSTOMLCD_BUSY_PIN 13U // MISO
#define CUSTOMLCD_CS_PIN 10U   // SS

#define CUSTOMLCD_DC_PIN 1U
#define CUSTOMLCD_RST_PIN 2U
#define CUSTOMLCD_TEST_PIN 42U

#define TEST_0 digitalWrite(CUSTOMLCD_TEST_PIN, LOW)
#define TEST_1 digitalWrite(CUSTOMLCD_TEST_PIN, HIGH)

#define EPD_WIDTH   200
#define EPD_HEIGHT  200

void seengreat_setup(void);

void driver_delay_us(unsigned int xus);
void driver_delay_xms(unsigned long xms);
void DELAY_S(unsigned int delaytime);
void SPI_Delay(unsigned char xrate);
void SPI_Write(unsigned char value);
void Epaper_Write_Command(unsigned char command);
void Epaper_Write_Data(unsigned char command);
// EPD
void Epaper_READBUSY(void);
void SPI_Write(uint8_t TxData);
void Epaper_Write_Command(uint8_t cmd);
void Epaper_Write_Data(uint8_t data);

void EPD_HW_Init(void);   // Electronic paper initialization
void EPD_WhiteScreen_ALL_Fast(const unsigned char *datas);
void EPD_HW_Init_GUI(void);
 

void EPD_Part_Update(void);
void EPD_Update(void);
void EPD_Update_Fast(void);


void EPD_WhiteScreen_Black(void);
void EPD_WhiteScreen_White(void);
void EPD_DeepSleep(void);
// Display
void EPD_WhiteScreen_ALL(const unsigned char *datas);
void EPD_SetRAMValue_BaseMap(const unsigned char *datas);
void EPD_Dis_Part(unsigned int x_start, unsigned int y_start,
                  const unsigned char *datas, unsigned int PART_COLUMN,
                  unsigned int PART_LINE);
void EPD_Display(unsigned char *Image);
void EPD_Dis_Part_myself(unsigned int x_startA, unsigned int y_startA,
                         const unsigned char *datasA, unsigned int x_startB,
                         unsigned int y_startB, const unsigned char *datasB,
                         unsigned int x_startC, unsigned int y_startC,
                         const unsigned char *datasC, unsigned int x_startD,
                         unsigned int y_startD, const unsigned char *datasD,
                         unsigned int x_startE, unsigned int y_startE,
                         const unsigned char *datasE, unsigned int PART_COLUMN,
                         unsigned int PART_LINE);

#endif