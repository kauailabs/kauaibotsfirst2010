#ifndef ARDUINO_H
#define ARDUINO_h

typedef unsigned char byte;
typedef unsigned char uint8_t;

void delay( int ms );
long millis();

float customround( float valToRound );

#define min(x,y) (((y) < (x)) ? (y) : (x))
#define max(x,y) (((y) > (x)) ? (y) : (x))
#endif
