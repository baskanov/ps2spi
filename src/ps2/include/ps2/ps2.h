#ifndef PS2_H
#define PS2_H

unsigned char ps2_can_send(unsigned char clock_mask, unsigned char data_mask);
unsigned char ps2_can_receive(unsigned char clock_mask, unsigned char data_mask);
unsigned char ps2_send(unsigned char clock_mask, unsigned char data_mask, unsigned char byte);
unsigned char ps2_receive(unsigned char clock_mask, unsigned char data_mask, unsigned char *output);

#endif /* PS2_H */
