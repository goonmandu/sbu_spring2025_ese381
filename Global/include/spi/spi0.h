#ifndef SPI0_H_
#define SPI0_H_

void init_spi0(void);
void init_spi0_SerLCD(void);
void init_spi0_LM74(void);
uint16_t read_spi0_LM74(void);
void send_byte_SerLCD(uint8_t data);
void select_slave_SerLCD();
void deselect_slave_SerLCD();
void select_slave_LM74();
void deselect_slave_LM74();
uint8_t exchange_byte(uint8_t data);
uint8_t normalize_range(uint8_t from_min, uint8_t from_val, uint8_t from_max, uint8_t result_min, uint8_t result_max);
void set_backlight_rgb(uint8_t r, uint8_t g, uint8_t b);

#endif /* SPI0_H_ */