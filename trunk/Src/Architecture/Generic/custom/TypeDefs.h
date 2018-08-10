/*
 * TypeDefs.h
 *
 *  Created on: Jan 20, 2016
 *      Author: Jordi Castells Sala
 */
#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__

#ifndef NULL
#define NULL ((void *)0)
#endif // NULL

typedef enum eBool
{
	FALSE,
	TRUE
} tBool;

typedef tBool BOOL;

typedef eError (*FPTR)(uint8_t);

/* Print Value constants */
typedef enum ePrintValueTypes
{
	TYPE_DECIMAL,
	TYPE_HEX,

	TYPE_MAX_TYPES
} tPrintValueTypes;

/* Verbose levels for printing log messages */
typedef enum eVerboseLevels
{
	VERBOSE_ALL,
	VERBOSE_GENERAL,
	VERBOSE_EVENT,
	VERBOSE_WARNING,
	VERBOSE_ERROR,

	NUM_OF_VERBOSE_LEVELS
} tVerboseLevels;

//	|																|																|
//	|							Word 1								|							Word 0								|
//	|_______________________________________________________________|_______________________________________________________________|
//	|								|								|								|								|
//	|			Byte 3				|			Byte 2				|			Byte 1				|			Byte 0				|
//	|_______________________________|_______________________________|_______________________________|_______________________________|
//	|				|				|				|				|				|				|				|				|
//	|	Nibble 7	|	Nibble 6	|	Nibble 5	|	Nibble 4	|	Nibble 3	|	Nibble 2	|	Nibble 1	|	Nibble 0	|
//	|_______________|_______________|_______________|_______________|_______________|_______________|_______________|_______________|
//	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|
//	|b31|b30|b29|b28|b27|b26|b25|b24|b23|b22|b21|b20|b19|b18|b17|b16|b15|b14|b13|b12|b11|b10|b09|b08|b07|b06|b05|b04|b03|b02|b01|b00|
//	|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|
//	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|
//	| X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X | X |
//	|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|


	typedef struct
	{
		uint16_t word_0		:16;
		uint16_t word_1		:16;
	} TWord_Struct;

	typedef struct
	{
		uint8_t byte_0		:8;
		uint8_t byte_1		:8;
		uint8_t byte_2		:8;
		uint8_t byte_3		:8;
	} TByte_Struct;

	typedef struct
	{
		uint8_t nibble_0	:4;
		uint8_t nibble_1	:4;
		uint8_t nibble_2	:4;
		uint8_t nibble_3	:4;
		uint8_t nibble_4	:4;
		uint8_t nibble_5	:4;
		uint8_t nibble_6	:4;
		uint8_t nibble_7	:4;
	} TNibble_Struct;

	typedef struct
	{
		uint8_t b0			:1;
		uint8_t b1			:1;
		uint8_t b2			:1;
		uint8_t b3			:1;
		uint8_t b4			:1;
		uint8_t b5			:1;
		uint8_t b6			:1;
		uint8_t b7			:1;
		uint8_t b8			:1;
		uint8_t b9			:1;
		uint8_t b10			:1;
		uint8_t b11			:1;
		uint8_t b12			:1;
		uint8_t b13			:1;
		uint8_t b14			:1;
		uint8_t b15			:1;
		uint8_t b16			:1;
		uint8_t b17			:1;
		uint8_t b18			:1;
		uint8_t b19			:1;
		uint8_t b20			:1;
		uint8_t b21			:1;
		uint8_t b22			:1;
		uint8_t b23			:1;
		uint8_t b24			:1;
		uint8_t b25			:1;
		uint8_t b26			:1;
		uint8_t b27			:1;
		uint8_t b28			:1;
		uint8_t b29			:1;
		uint8_t b30			:1;
		uint8_t b31			:1;
	} TBit_Struct;


	typedef union
	{
		TWord_Struct	Word;
		TByte_Struct	Byte;
		TNibble_Struct	Nibble;
		TBit_Struct		Bit;
	}TData_Struct;

	#define b0(x)										(((TData_Struct *) &x)->Bit.b0 )
	#define b1(x)										(((TData_Struct *) &x)->Bit.b1 )
	#define b2(x)  										(((TData_Struct *) &x)->Bit.b2 )
	#define b3(x)  										(((TData_Struct *) &x)->Bit.b3 )
	#define b4(x)  										(((TData_Struct *) &x)->Bit.b4 )
	#define b5(x)  										(((TData_Struct *) &x)->Bit.b5 )
	#define b6(x)  										(((TData_Struct *) &x)->Bit.b6 )
	#define b7(x)  										(((TData_Struct *) &x)->Bit.b7 )
	#define b8(x)  										(((TData_Struct *) &x)->Bit.b8 )
	#define b9(x)  										(((TData_Struct *) &x)->Bit.b9 )
	#define b10(x) 										(((TData_Struct *) &x)->Bit.b10)
	#define b11(x) 										(((TData_Struct *) &x)->Bit.b11)
	#define b12(x) 										(((TData_Struct *) &x)->Bit.b12)
	#define b13(x) 										(((TData_Struct *) &x)->Bit.b13)
	#define b14(x) 										(((TData_Struct *) &x)->Bit.b14)
	#define b15(x) 										(((TData_Struct *) &x)->Bit.b15)
	#define b16(x)  									(((TData_Struct *) &x)->Bit.b16)
	#define b17(x)  									(((TData_Struct *) &x)->Bit.b17)
	#define b18(x)  									(((TData_Struct *) &x)->Bit.b18)
	#define b19(x)  									(((TData_Struct *) &x)->Bit.b19)
	#define b20(x)  									(((TData_Struct *) &x)->Bit.b20)
	#define b21(x)  									(((TData_Struct *) &x)->Bit.b21)
	#define b22(x)  									(((TData_Struct *) &x)->Bit.b22)
	#define b23(x)  									(((TData_Struct *) &x)->Bit.b23)
	#define b24(x)  									(((TData_Struct *) &x)->Bit.b24)
	#define b25(x)  									(((TData_Struct *) &x)->Bit.b25)
	#define b26(x) 										(((TData_Struct *) &x)->Bit.b26)
	#define b27(x) 										(((TData_Struct *) &x)->Bit.b27)
	#define b28(x) 										(((TData_Struct *) &x)->Bit.b28)
	#define b29(x) 										(((TData_Struct *) &x)->Bit.b29)
	#define b30(x) 										(((TData_Struct *) &x)->Bit.b30)
	#define b31(x) 										(((TData_Struct *) &x)->Bit.b31)

	#define nibble_0(x)									(((TData_Struct *) &x)->Nibble.nibble_0)
	#define nibble_1(x)									(((TData_Struct *) &x)->Nibble.nibble_1)
	#define nibble_2(x)									(((TData_Struct *) &x)->Nibble.nibble_2)
	#define nibble_3(x)									(((TData_Struct *) &x)->Nibble.nibble_3)
	#define nibble_4(x)									(((TData_Struct *) &x)->Nibble.nibble_4)
	#define nibble_5(x)									(((TData_Struct *) &x)->Nibble.nibble_5)
	#define nibble_6(x)									(((TData_Struct *) &x)->Nibble.nibble_6)
	#define nibble_7(x)									(((TData_Struct *) &x)->Nibble.nibble_7)

	#define byte_0(x)									(((TData_Struct *) &x)->Byte.byte_0)
	#define byte_1(x)									(((TData_Struct *) &x)->Byte.byte_1)
	#define byte_2(x)									(((TData_Struct *) &x)->Byte.byte_2)
	#define byte_3(x)									(((TData_Struct *) &x)->Byte.byte_3)

	#define word_0(x)									(((TData_Struct *) &x)->Word.word_0)
	#define word_1(x)									(((TData_Struct *) &x)->Word.word_1)


	//___________________________Data modification Macros____________________________//
	#define MON_LONG_MASK									(0xFFFFFFFF)
	#define MON_WORD_MASK									(0xFFFF)
	#define MON_BYTE_MASK									(0xFF)
	#define MON_NIBBLE_MASK									(0x0F)
	#define MON_BIT_MASK									(0x01)

    #define MON_LONG_MASK									(0xFFFFFFFF)
    #define MON_WORD_MASK									(0xFFFF)
    #define MON_BYTE_MASK									(0xFF)
    #define MON_NIBBLE_MASK									(0x0F)
    #define MON_BIT_MASK									(0x01)

    #define MON_SET_BIT(b,x,v)								( (uint32_t)( ((b)&(~(MON_BIT_MASK<<((x)<<0))))|(((v)&MON_BIT_MASK)<<((x)<<0))) )
    #define MON_GET_BIT(b,x)								( (uint8_t )( ((b)>>((x)<<0))&MON_BIT_MASK) )

    #define MON_SET_NIBBLE(n,x,v)							( (uint32_t)( ((n)&(~(MON_NIBBLE_MASK<<((x)<<2))))|(((v)&MON_NIBBLE_MASK)<<((x)<<2))) )
    #define MON_GET_NIBBLE(n,x)								( (uint8_t )( ((n)>>((x)<<2))&MON_NIBBLE_MASK) )

    #define MON_SET_BYTE(w,x,v)								( (uint32_t) ( ((w)&(~(MON_BYTE_MASK<<((x)<<3))))|(((v)&MON_BYTE_MASK)<<((x)<<3))) )
    #define MON_GET_BYTE(w,x)		 						( (uint8_t )( ((w)>>((x)<<3))&MON_BYTE_MASK) )

    #define MON_SET_WORD(l,x,v)								( (uint32_t) ( ((l)&(~(MON_WORD_MASK<<((x)<<4))))|(((v)&MON_WORD_MASK)<<((x)<<4))) )
    #define MON_GET_WORD(l,x)		 						( (uint16_t)( ((l)>>((x)<<4))&MON_WORD_MASK) )

    //*********************************************************************************************************************************************//
    #define MON_MAKENIBBLE_Bit(b3,b2,b1,b0)					( ((uint8_t )((b3)&MON_BIT_MASK)<< 3)|((uint8_t )((b2)&MON_BIT_MASK)<< 2)|((uint8_t )((b1)&MON_BIT_MASK)<< 1)|((uint8_t )((b0)&MON_BIT_MASK)<< 0) )
    #define MON_MAKEBYTE_Nibble(n1,n0)						( ((uint8_t )((n1)&MON_NIBBLE_MASK)<< 4)|((uint8_t )((n0)&MON_NIBBLE_MASK)<< 0 ) )
    #define MON_MAKEBYTE_Bit(b7,b6,b5,b4,b3,b2,b1,b0)		( MON_MAKEBYTE_Nibble(	MON_MAKENIBBLE_Bit(b7,b6,b5,b4),														\
    																		MON_MAKENIBBLE_Bit(b3,b2,b1,b0)	) )
    #define MON_MAKEWORD_Byte(b1,b0)	 					( ((uint16_t)((b1)&MON_BYTE_MASK)<< 8)|((uint16_t)((b0)&MON_BYTE_MASK)<< 0 ) )
    #define MON_MAKEWORD_Nibble(n3,n2,n1,n0)				( MON_MAKEWORD_Byte(	MON_MAKEBYTE_Nibble(n3,n2),																\
    																		MON_MAKEBYTE_Nibble(n1,n0)	) )
    #define MON_MAKEWORD_Bit(b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0)																					\
    													( MON_MAKEWORD_Byte(	MON_MAKEBYTE_Bit(b15,b14,b13,b12,b11,b10,b9,b8),										\
    																		MON_MAKEBYTE_Bit(b7,b6,b5,b4,b3,b2,b1,b0)	) )
    #define MON_MAKELONG_Word(w1,w0)						( ((uint32_t)((w1)&MON_WORD_MASK)<<16)|((uint32_t)((w0)&MON_WORD_MASK)<< 0 ) )
    #define MON_MAKELONG_Byte(b3,b2,b1,b0)					( MON_MAKELONG_Word(	MON_MAKEWORD_Byte(b3,b2),																\
    																		MON_MAKEWORD_Byte(b1,b0)	) )
    #define MON_MAKELONG_Nibble(n7,n6,n5,n4,n3,n2,n1,n0)	( MON_MAKELONG_Word(	MON_MAKEWORD_Nibble(n7,n6,n5,n4),														\
    																		MON_MAKEWORD_Nibble(n3,n2,n1,n0)	) )
    #define MON_MAKELONG_Bit(b31,b30,b29,b28,b27,b26,b25,b24,b23,b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0)					\
    													( MON_MAKELONG_Word(	MON_MAKEWORD_Bit(b31,b30,b29,b28,b27,b26,b25,b24,b23,b22,b21,b20,b19,b18,b17,b16),		\
    																		MON_MAKEWORD_Bit(b15,b14,b13,b12,b11,b10, b9, b8, b7, b6, b5, b4, b3, b2, b1, b0)	) )

#endif
