#ifndef DEFINITIONS_H
#define DEFINITIONS_H


typedef unsigned char byte;
typedef unsigned short int byte_2;

#define RAM_FA 0x0000
#define RAM_LA 0x1fff
#define IO_FA  0x2000
#define IO_LA  0x3fff

enum ADR
{
    ZP , //zero page
    ZPX , //zero page X
    ZPY , //zero page Y
    ABS , //absolute
    ABSX , //absolute X
    ABSY , //absolute Y
    IND , //indirect 
    IMP , //implied
    ACC , //accoumulator
    IMM , //immediate
    REL , //relative 
    INDX , //indexed indirect
    INDY //indirect indexed
};


#endif