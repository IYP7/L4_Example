/**HEADER HERE*/

/***********************************************************************
* THIS FILE IS AUTOGENERATED
***********************************************************************/

/***********************************************************************
* FILE INCLUDES
***********************************************************************/
#include "Hreg_dat.h"


/***********************************************************************
* PRIVATE VARIABLES
***********************************************************************/
static uint8_t    hregSystemPowerUpReason[1];
static uint8_t    hregSystemPowerMode[1];
static uint32_t   hregSystemDate[1];
static uint32_t   hregSystemTime[1];
static uint32_t   hregFlashSize[NUM_OF_FLASH];
static uint16_t   hregFlashData[NUM_OF_FLASH];
static uint32_t   hregFlashOffset[NUM_OF_FLASH];
static uint8_t    hregGPIOValue[NUM_OF_GPIO];
static uint32_t   hregUartBufferSize[NUM_OF_UART];
static uint32_t   hregUartData[NUM_OF_UART];
static uint8_t    hregUartStatus[NUM_OF_UART];
static uint32_t   hregSpiBufferSize[NUM_OF_SPI];
static uint32_t   hregSpiData[NUM_OF_SPI];
static uint8_t    hregSpiStatus[NUM_OF_SPI];
static uint8_t    hregInterruptsState[NUM_OF_INT];
static uint16_t   hregButtonsState[NUM_OF_BUTTONS];
static uint16_t   hregLedsEffect[NUMBER_OF_LEDS];
static uint16_t   hregLedsState[NUMBER_OF_LEDS];
static uint32_t   hregSwTimerCallbackFunction[NUM_OF_SW_TIMER];
static uint32_t   hregSwTimerGetPending[NUM_OF_SW_TIMER];
static uint8_t    hregSwTimerState[NUM_OF_SW_TIMER];
static uint32_t   hregSwTimerGetElapsed[NUM_OF_SW_TIMER];
static uint32_t   hregSwTimerSet[NUM_OF_SW_TIMER];
static uint16_t   hregVirtualEEPROMRegister[NUM_OF_VIRTUAL_EEPROMS];
static uint16_t   hregVirtualEEPROMInstanceReg[NUM_OF_VIRTUAL_EEPROMS];
static uint32_t   hregVirtualEEPROMData[NUM_OF_VIRTUAL_EEPROMS];

/***********************************************************************
* HREG FUNCTION TABLES. READ AND WRITE
***********************************************************************/
const HREG_FUNC_READ_CALL onReadHregFunc[19] = 
{
	{ onReadHregSystemDate },
	{ onReadHregSystemTime },
	{ onReadHregFlashSize },
	{ onReadHregFlashData },
	{ onReadHregGPIOValue },
	{ onReadHregUartBufferSize },
	{ onReadHregUartData },
	{ onReadHregUartStatus },
	{ onReadHregSpiBufferSize },
	{ onReadHregSpiData },
	{ onReadHregSpiStatus },
	{ onReadHregInterruptsState },
	{ onReadHregButtonsState },
	{ onReadHregLedsEffect },
	{ onReadHregLedsState },
	{ onReadHregSwTimerGetPending },
	{ onReadHregSwTimerState },
	{ onReadHregSwTimerGetElapsed },
	{ onReadHregVirtualEEPROMData },
};

const HREG_FUNC_WRITE_CALL onWriteHregFunc[16] = 
{
	{ onWriteHregSystemPowerMode },
	{ onWriteHregSystemDate },
	{ onWriteHregSystemTime },
	{ onWriteHregFlashData },
	{ onWriteHregGPIOValue },
	{ onWriteHregUartBufferSize },
	{ onWriteHregUartData },
	{ onWriteHregSpiBufferSize },
	{ onWriteHregSpiData },
	{ onWriteHregInterruptsState },
	{ onWriteHregLedsEffect },
	{ onWriteHregLedsState },
	{ onWriteHregSwTimerCallbackFunction },
	{ onWriteHregSwTimerState },
	{ onWriteHregSwTimerSet },
	{ onWriteHregVirtualEEPROMData },
};


/***********************************************************************
* HREG GROUP TABLES
***********************************************************************/
const HREG_GROUP_INT_CALL apiFuncHregGroup[10] = 
{
	{ initHregSystem , stopHregSystem , startHregSystem , sleepHregSystem , wakeHregSystem},
	{ initHregFlash , stopHregFlash , startHregFlash , sleepHregFlash , wakeHregFlash},
	{ initHregGPIO , stopHregGPIO , startHregGPIO , sleepHregGPIO , wakeHregGPIO},
	{ initHregUart , stopHregUart , startHregUart , sleepHregUart , wakeHregUart},
	{ initHregSpi , stopHregSpi , startHregSpi , sleepHregSpi , wakeHregSpi},
	{ initHregInterrupts , stopHregInterrupts , startHregInterrupts , sleepHregInterrupts , wakeHregInterrupts},
	{ initHregButtons , stopHregButtons , startHregButtons , sleepHregButtons , wakeHregButtons},
	{ initHregLeds , stopHregLeds , startHregLeds , sleepHregLeds , wakeHregLeds},
	{ initHregSwTimer , stopHregSwTimer , startHregSwTimer , sleepHregSwTimer , wakeHregSwTimer},
	{ initHregVirtualEEPROM , stopHregVirtualEEPROM , startHregVirtualEEPROM , sleepHregVirtualEEPROM , wakeHregVirtualEEPROM},
};


/***********************************************************************
* HREG STATES TABLES
***********************************************************************/
HREG_GROUP_STATE hregGroupsState[10] = 
{
	{ HREG_GROUP_DECLARED },
	{ HREG_GROUP_DECLARED },
	{ HREG_GROUP_DECLARED },
	{ HREG_GROUP_DECLARED },
	{ HREG_GROUP_DECLARED },
	{ HREG_GROUP_DECLARED },
	{ HREG_GROUP_DECLARED },
	{ HREG_GROUP_DECLARED },
	{ HREG_GROUP_DECLARED },
	{ HREG_GROUP_DECLARED },
};


/***********************************************************************
* HREG TABLES
***********************************************************************/
const HREG tableHregSystem[4] = 
{
	{
		/*Data*/	&hregSystemPowerUpReason,
		/*MaxVal*/	SYSTEM_MAX_POWER_UP_REASONS,
		/*MinVal*/	SYSTEM_FIRST_POWER_UP,
		/*Init*/	SYSTEM_FIRST_POWER_UP,
		/*onRead*/	HREG_NOP,
		/*onWrite*/	HREG_NOP,
		/*GroupId*/	HREG_GROUP_INDEX_SYSTEM,
		/*NVMId*/	NVM_NOP,
	},
	{
		/*Data*/	&hregSystemPowerMode,
		/*MaxVal*/	SYSTEM_MAX_POWER_MODES,
		/*MinVal*/	SYSTEM_EXECUTE_BOOTLOADER,
		/*Init*/	0,
		/*onRead*/	HREG_NOP,
		/*onWrite*/	ON_WRITE_HREG_INDEX_SYSTEM_POWER_MODE,
		/*GroupId*/	HREG_GROUP_INDEX_SYSTEM,
		/*NVMId*/	NVM_NOP,
	},
	{
		/*Data*/	&hregSystemDate,
		/*MaxVal*/	0xFFFFFFFF,
		/*MinVal*/	0,
		/*Init*/	0,
		/*onRead*/	ON_READ_HREG_INDEX_SYSTEM_DATE,
		/*onWrite*/	ON_WRITE_HREG_INDEX_SYSTEM_DATE,
		/*GroupId*/	HREG_GROUP_INDEX_SYSTEM,
		/*NVMId*/	NVM_NOP,
	},
	{
		/*Data*/	&hregSystemTime,
		/*MaxVal*/	0xFFFFFFFF,
		/*MinVal*/	0,
		/*Init*/	0,
		/*onRead*/	ON_READ_HREG_INDEX_SYSTEM_TIME,
		/*onWrite*/	ON_WRITE_HREG_INDEX_SYSTEM_TIME,
		/*GroupId*/	HREG_GROUP_INDEX_SYSTEM,
		/*NVMId*/	NVM_NOP,
	},
};

const HREG tableHregFlash[3] = 
{
	{
		/*Data*/	&hregFlashSize,
		/*MaxVal*/	0xFFFFFFFF,
		/*MinVal*/	0,
		/*Init*/	0,
		/*onRead*/	ON_READ_HREG_INDEX_FLASH_SIZE,
		/*onWrite*/	HREG_NOP,
		/*GroupId*/	HREG_GROUP_INDEX_FLASH,
		/*NVMId*/	NVM_NOP,
	},
	{
		/*Data*/	&hregFlashData,
		/*MaxVal*/	0xFFFF,
		/*MinVal*/	0,
		/*Init*/	0,
		/*onRead*/	ON_READ_HREG_INDEX_FLASH_DATA,
		/*onWrite*/	ON_WRITE_HREG_INDEX_FLASH_DATA,
		/*GroupId*/	HREG_GROUP_INDEX_FLASH,
		/*NVMId*/	NVM_NOP,
	},
	{
		/*Data*/	&hregFlashOffset,
		/*MaxVal*/	0xFFFFFFFF,
		/*MinVal*/	0,
		/*Init*/	0,
		/*onRead*/	HREG_NOP,
		/*onWrite*/	HREG_NOP,
		/*GroupId*/	HREG_GROUP_INDEX_FLASH,
		/*NVMId*/	NVM_NOP,
	},
};

const HREG tableHregGPIO[1] = 
{
	{
		/*Data*/	&hregGPIOValue,
		/*MaxVal*/	GPIO_TOGGLE,
		/*MinVal*/	GPIO_LOW,
		/*Init*/	GPIO_LOW,
		/*onRead*/	ON_READ_HREG_INDEX_GPIO_VALUE,
		/*onWrite*/	ON_WRITE_HREG_INDEX_GPIO_VALUE,
		/*GroupId*/	HREG_GROUP_INDEX_GPIO,
		/*NVMId*/	NVM_NOP,
	},
};

const HREG tableHregUart[3] = 
{
	{
		/*Data*/	&hregUartBufferSize,
		/*MaxVal*/	0xFFFFFFFF,
		/*MinVal*/	0,
		/*Init*/	0,
		/*onRead*/	ON_READ_HREG_INDEX_UART_BUFFER_SIZE,
		/*onWrite*/	ON_WRITE_HREG_INDEX_UART_BUFFER_SIZE,
		/*GroupId*/	HREG_GROUP_INDEX_UART,
		/*NVMId*/	NVM_NOP,
	},
	{
		/*Data*/	&hregUartData,
		/*MaxVal*/	0xFFFFFFFF,
		/*MinVal*/	0,
		/*Init*/	0,
		/*onRead*/	ON_READ_HREG_INDEX_UART_DATA,
		/*onWrite*/	ON_WRITE_HREG_INDEX_UART_DATA,
		/*GroupId*/	HREG_GROUP_INDEX_UART,
		/*NVMId*/	NVM_NOP,
	},
	{
		/*Data*/	&hregUartStatus,
		/*MaxVal*/	NUM_OF_RET_ERRORS,
		/*MinVal*/	0,
		/*Init*/	0,
		/*onRead*/	ON_READ_HREG_INDEX_UART_STATUS,
		/*onWrite*/	HREG_NOP,
		/*GroupId*/	HREG_GROUP_INDEX_UART,
		/*NVMId*/	NVM_NOP,
	},
};

const HREG tableHregSpi[3] = 
{
	{
		/*Data*/	&hregSpiBufferSize,
		/*MaxVal*/	0xFFFFFFFF,
		/*MinVal*/	0,
		/*Init*/	0,
		/*onRead*/	ON_READ_HREG_INDEX_SPI_BUFFER_SIZE,
		/*onWrite*/	ON_WRITE_HREG_INDEX_SPI_BUFFER_SIZE,
		/*GroupId*/	HREG_GROUP_INDEX_SPI,
		/*NVMId*/	NVM_NOP,
	},
	{
		/*Data*/	&hregSpiData,
		/*MaxVal*/	0xFFFFFFFF,
		/*MinVal*/	0,
		/*Init*/	0,
		/*onRead*/	ON_READ_HREG_INDEX_SPI_DATA,
		/*onWrite*/	ON_WRITE_HREG_INDEX_SPI_DATA,
		/*GroupId*/	HREG_GROUP_INDEX_SPI,
		/*NVMId*/	NVM_NOP,
	},
	{
		/*Data*/	&hregSpiStatus,
		/*MaxVal*/	NUM_OF_RET_ERRORS,
		/*MinVal*/	0,
		/*Init*/	0,
		/*onRead*/	ON_READ_HREG_INDEX_SPI_STATUS,
		/*onWrite*/	HREG_NOP,
		/*GroupId*/	HREG_GROUP_INDEX_SPI,
		/*NVMId*/	NVM_NOP,
	},
};

const HREG tableHregInterrupts[1] = 
{
	{
		/*Data*/	&hregInterruptsState,
		/*MaxVal*/	IRQ_ENABLE,
		/*MinVal*/	IRQ_DISABLE,
		/*Init*/	IRQ_DISABLE,
		/*onRead*/	ON_READ_HREG_INDEX_INTERRUPTS_STATE,
		/*onWrite*/	ON_WRITE_HREG_INDEX_INTERRUPTS_STATE,
		/*GroupId*/	HREG_GROUP_INDEX_INTERRUPTS,
		/*NVMId*/	NVM_NOP,
	},
};

const HREG tableHregButtons[1] = 
{
	{
		/*Data*/	&hregButtonsState,
		/*MaxVal*/	0XFFFF,
		/*MinVal*/	0,
		/*Init*/	0,
		/*onRead*/	ON_READ_HREG_INDEX_BUTTONS_STATE,
		/*onWrite*/	HREG_NOP,
		/*GroupId*/	HREG_GROUP_INDEX_BUTTONS,
		/*NVMId*/	NVM_NOP,
	},
};

const HREG tableHregLeds[2] = 
{
	{
		/*Data*/	&hregLedsEffect,
		/*MaxVal*/	NUM_OF_LED_EFFECT,
		/*MinVal*/	0,
		/*Init*/	0,
		/*onRead*/	ON_READ_HREG_INDEX_LEDS_EFFECT,
		/*onWrite*/	ON_WRITE_HREG_INDEX_LEDS_EFFECT,
		/*GroupId*/	HREG_GROUP_INDEX_LEDS,
		/*NVMId*/	NVM_NOP,
	},
	{
		/*Data*/	&hregLedsState,
		/*MaxVal*/	LED_NUM_OF_STATES,
		/*MinVal*/	0,
		/*Init*/	0,
		/*onRead*/	ON_READ_HREG_INDEX_LEDS_STATE,
		/*onWrite*/	ON_WRITE_HREG_INDEX_LEDS_STATE,
		/*GroupId*/	HREG_GROUP_INDEX_LEDS,
		/*NVMId*/	NVM_NOP,
	},
};

const HREG tableHregSwTimer[5] = 
{
	{
		/*Data*/	&hregSwTimerCallbackFunction,
		/*MaxVal*/	0xFFFFFFFF,
		/*MinVal*/	0,
		/*Init*/	0,
		/*onRead*/	HREG_NOP,
		/*onWrite*/	ON_WRITE_HREG_INDEX_SWTIMER_CALLBACK_FUNCTION,
		/*GroupId*/	HREG_GROUP_INDEX_SWTIMER,
		/*NVMId*/	NVM_NOP,
	},
	{
		/*Data*/	&hregSwTimerGetPending,
		/*MaxVal*/	0xFFFFFFFF,
		/*MinVal*/	0,
		/*Init*/	0,
		/*onRead*/	ON_READ_HREG_INDEX_SWTIMER_GET_PENDING,
		/*onWrite*/	HREG_NOP,
		/*GroupId*/	HREG_GROUP_INDEX_SWTIMER,
		/*NVMId*/	NVM_NOP,
	},
	{
		/*Data*/	&hregSwTimerState,
		/*MaxVal*/	SW_TIMER_NUM_OF_STATES,
		/*MinVal*/	0,
		/*Init*/	0,
		/*onRead*/	ON_READ_HREG_INDEX_SWTIMER_STATE,
		/*onWrite*/	ON_WRITE_HREG_INDEX_SWTIMER_STATE,
		/*GroupId*/	HREG_GROUP_INDEX_SWTIMER,
		/*NVMId*/	NVM_NOP,
	},
	{
		/*Data*/	&hregSwTimerGetElapsed,
		/*MaxVal*/	0xFFFFFFFF,
		/*MinVal*/	0,
		/*Init*/	0,
		/*onRead*/	ON_READ_HREG_INDEX_SWTIMER_GET_ELAPSED,
		/*onWrite*/	HREG_NOP,
		/*GroupId*/	HREG_GROUP_INDEX_SWTIMER,
		/*NVMId*/	NVM_NOP,
	},
	{
		/*Data*/	&hregSwTimerSet,
		/*MaxVal*/	0xFFFFFFFF,
		/*MinVal*/	0,
		/*Init*/	0,
		/*onRead*/	HREG_NOP,
		/*onWrite*/	ON_WRITE_HREG_INDEX_SWTIMER_SET,
		/*GroupId*/	HREG_GROUP_INDEX_SWTIMER,
		/*NVMId*/	NVM_NOP,
	},
};

const HREG tableHregVirtualEEPROM[3] = 
{
	{
		/*Data*/	&hregVirtualEEPROMRegister,
		/*MaxVal*/	0xFFFF,
		/*MinVal*/	0,
		/*Init*/	0,
		/*onRead*/	HREG_NOP,
		/*onWrite*/	HREG_NOP,
		/*GroupId*/	HREG_GROUP_INDEX_VIRTUALEEPROM,
		/*NVMId*/	NVM_NOP,
	},
	{
		/*Data*/	&hregVirtualEEPROMInstanceReg,
		/*MaxVal*/	0xFFFF,
		/*MinVal*/	0,
		/*Init*/	0,
		/*onRead*/	HREG_NOP,
		/*onWrite*/	HREG_NOP,
		/*GroupId*/	HREG_GROUP_INDEX_VIRTUALEEPROM,
		/*NVMId*/	NVM_NOP,
	},
	{
		/*Data*/	&hregVirtualEEPROMData,
		/*MaxVal*/	0xFFFFFFFF,
		/*MinVal*/	0,
		/*Init*/	0,
		/*onRead*/	ON_READ_HREG_INDEX_VIRTUALEEPROM_DATA,
		/*onWrite*/	ON_WRITE_HREG_INDEX_VIRTUALEEPROM_DATA,
		/*GroupId*/	HREG_GROUP_INDEX_VIRTUALEEPROM,
		/*NVMId*/	NVM_NOP,
	},
};

const HREG_TABLE_INFO hregTableList[10]= {
	{ tableHregSystem       , sizeof(tableHregSystem)/sizeof(HREG)         , 1                   , HREG_GROUP_TYPE_HAL },
	{ tableHregFlash        , sizeof(tableHregFlash)/sizeof(HREG)          , NUM_OF_FLASH        , HREG_GROUP_TYPE_HAL },
	{ tableHregGPIO         , sizeof(tableHregGPIO)/sizeof(HREG)           , NUM_OF_GPIO         , HREG_GROUP_TYPE_HAL },
	{ tableHregUart         , sizeof(tableHregUart)/sizeof(HREG)           , NUM_OF_UART         , HREG_GROUP_TYPE_HAL },
	{ tableHregSpi          , sizeof(tableHregSpi)/sizeof(HREG)            , NUM_OF_SPI          , HREG_GROUP_TYPE_HAL },
	{ tableHregInterrupts   , sizeof(tableHregInterrupts)/sizeof(HREG)     , NUM_OF_INT          , HREG_GROUP_TYPE_HAL },
	{ tableHregButtons      , sizeof(tableHregButtons)/sizeof(HREG)        , NUM_OF_BUTTONS      , HREG_GROUP_TYPE_DEVICE_DRIVER },
	{ tableHregLeds         , sizeof(tableHregLeds)/sizeof(HREG)           , NUMBER_OF_LEDS      , HREG_GROUP_TYPE_DEVICE_DRIVER },
	{ tableHregSwTimer      , sizeof(tableHregSwTimer)/sizeof(HREG)        , NUM_OF_SW_TIMER     , HREG_GROUP_TYPE_DEVICE_DRIVER },
	{ tableHregVirtualEEPROM, sizeof(tableHregVirtualEEPROM)/sizeof(HREG)  , NUM_OF_VIRTUAL_EEPROMS, HREG_GROUP_TYPE_EEPROM },
};

const HREG_FUNC hregFunc = {
	readHregTableStd,
	writeHregTableStd,
};

