#ifndef __DS18B20_DRV_H__
#define __DS18B20_DRV_H__


unsigned char ds18b20_init(void);
void ow_reset(void)			  ;
unsigned char ds18B20_check(void);
unsigned char ds18b20_init(void) ;
void write_byte(unsigned char val);
unsigned char read_byte(void);
float Read_Temp(void);

#endif