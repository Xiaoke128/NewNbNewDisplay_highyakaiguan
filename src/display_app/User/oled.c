#include "oled.h"
#include "hw_spi.h"
#include "hw_exit.h"

DispalyStoreData DisplayData = {0};

void DisplayInterTempAdd(uint8_t tempVal, uint8_t humiVal, uint32_t unix)
{
		uint8_t i = 0;
	
		for(i = 29; i > 0; i--)
		{
				DisplayData.Cj2301Temp[i] = DisplayData.Cj2301Temp[i - 1];
				DisplayData.Cj2301Humi[i] = DisplayData.Cj2301Humi[i - 1];
				DisplayData.InterTempUnix[i] = DisplayData.InterTempUnix[i - 1];
		}
		DisplayData.Cj2301Temp[0] = tempVal;
		DisplayData.Cj2301Humi[0] = humiVal;
		DisplayData.InterTempUnix[0] = unix;
		if(DisplayData.InterTempIndex < 30)
		{
				//DisplayData.InterTemp[DisplayData.InterTempIndex] = tempVal;
				//DisplayData.InterTempUnix[DisplayData.InterTempIndex] = unix;
				DisplayData.InterTempIndex++;
		}
}

void DispalyTempStoreAdd(uint8_t tempNum, int8_t tempVal, uint32_t unix)
{
		uint8_t i = 0;
	
		for(i = 29; i > 0; i--)
		{
				DisplayData.selfTemp[tempNum][i] = DisplayData.selfTemp[tempNum][i - 1];
				DisplayData.selfTempUnix[tempNum][i] = DisplayData.selfTempUnix[tempNum][i - 1];
		}
		DisplayData.selfTemp[tempNum][0] = tempVal;
		DisplayData.selfTempUnix[tempNum][0] = unix;
		if(DisplayData.selfTempIndex[tempNum][0] < 30)
		{
				//DisplayData.selfTemp[tempNum][DisplayData.selfTempIndex[tempNum][0]] = tempVal;
				//DisplayData.selfTempUnix[tempNum][DisplayData.selfTempIndex[tempNum][0]] = unix;
				DisplayData.selfTempIndex[tempNum][0] += 1;
		}
}

void DispalyHumiStoreAdd(uint8_t humiVal, uint32_t unix)
{
		uint8_t i = 0;
	
		for(i = 29; i > 0; i--)
		{
				DisplayData.selfHumi[i] = DisplayData.selfHumi[i - 1];
				DisplayData.selfHumiUnix[i] = DisplayData.selfHumiUnix[i - 1];
		}
		DisplayData.selfHumi[0] = humiVal;
		DisplayData.selfHumiUnix[0] = unix;
		if(DisplayData.selfHumiIndex < 30)
		{
				//DisplayData.selfHumi[DisplayData.selfHumiIndex] = humiVal;
				//DisplayData.selfHumiUnix[DisplayData.selfHumiIndex] = unix;
				DisplayData.selfHumiIndex++;
		}
}

void DisplaySignalStoreAdd(uint8_t type, uint16_t time, uint32_t unix)
{
		uint8_t i = 0;
	
		if(unix == 0)
		{
				return;
		}
		if((unix <= DisplayData.signalUnix[type][0]) || (time > 10000))
		{
				return;
		}
		for(i = 29; i > 0; i--)
		{
				DisplayData.signalTime[type][i] = DisplayData.signalTime[type][i - 1];
				DisplayData.signalUnix[type][i] = DisplayData.signalUnix[type][i - 1];
		}
		DisplayData.signalTime[type][0] = time;
		DisplayData.signalUnix[type][0] = unix;
		if(DisplayData.signalIndex[type][0] < 30)
		{
				//DisplayData.signalTime[type][DisplayData.signalIndex[type][0]] = time;
				//DisplayData.signalUnix[type][DisplayData.signalIndex[type][0]] = unix;
				DisplayData.signalIndex[type][0] += 1;
		}
		memcpy(StoreInfo.signalIndex, DisplayData.signalIndex, sizeof(StoreInfo.signalIndex));
		memcpy(StoreInfo.signalTime, DisplayData.signalTime, sizeof(StoreInfo.signalTime));
		memcpy(StoreInfo.signalUnix, DisplayData.signalUnix, sizeof(StoreInfo.signalUnix));
		StoreInfoStore();
}
#if 0
void DisalyAbnormalInterTempAdd(uint8_t tempVal, uint32_t unix)
{
		uint8_t i = 0;
	
		if(DisplayData.abnormalIterTempIndex < 30)
		{
				DisplayData.abnormalIterTemp[DisplayData.abnormalIterTempIndex] = tempVal;
				DisplayData.abnormalIterTempUnix[DisplayData.abnormalIterTempIndex] = unix;
				DisplayData.abnormalIterTempIndex += 1;
		}
		else
		{
				for(i = 0; i < 29; i++)
				{
						DisplayData.abnormalIterTemp[i] = DisplayData.abnormalIterTemp[i + 1];
						DisplayData.abnormalIterTempUnix[i] = DisplayData.abnormalIterTempUnix[i + 1];
				}
				DisplayData.abnormalIterTemp[29] = tempVal;
				DisplayData.abnormalIterTempUnix[29] = unix;
		}
}
#endif

void DisalyAbnormalHumiAdd(uint8_t humiVal, uint32_t unix)
{
		uint8_t i = 0;
	
		if(DisplayData.abnormalHumiIndex > 0)
		{
				if(unix <= DisplayData.abnormalHumiUnix[0])
				{
						return;
				}
		}
		for(i = 29; i > 0; i--)
		{
				DisplayData.abnormalHumi[i] = DisplayData.abnormalHumi[i - 1];
				DisplayData.abnormalHumiUnix[i] = DisplayData.abnormalHumiUnix[i - 1];
		}
		DisplayData.abnormalHumi[0] = humiVal;
		DisplayData.abnormalHumiUnix[0] = unix;
		if(DisplayData.abnormalHumiIndex < 30)
		{
				//DisplayData.abnormalHumi[DisplayData.abnormalHumiIndex] = humiVal;
				//DisplayData.abnormalHumiUnix[DisplayData.abnormalHumiIndex] = unix;
				DisplayData.abnormalHumiIndex += 1;
		}
}
void DisplayAnnormalTempAdd(uint8_t tempNum, int8_t tempVal, uint32_t unix)
{
		uint8_t i = 0;
	
		if(DisplayData.abnormalTempIndex > 0)
		{
				if(unix <= DisplayData.abnormalTempUnix[0])
				{
						return;
				}
		}
		for(i = 29; i > 0; i--)
		{
				DisplayData.abnormalTemp[i] = DisplayData.abnormalTemp[i - 1];
				DisplayData.abnormalTempUnix[i] = DisplayData.abnormalTempUnix[i - 1];
		}
		DisplayData.abnormalTemp[0] = tempVal;
		DisplayData.abnormalTempUnix[0] = unix;
		if(DisplayData.abnormalTempIndex < 30)
		{
				//DisplayData.abnormalTemp[DisplayData.abnormalTempIndex] = tempVal;
				//DisplayData.abnormalTempUnix[DisplayData.abnormalTempIndex] = unix;
				DisplayData.abnormalTempIndex += 1;
		}
}

void DisplayAbnormalSignalAdd(uint8_t type, uint16_t time, uint32_t unix)
{
		uint8_t i = 0;
	
		if(DisplayData.abnormnalSignalIndex > 0)
		{
				if(unix <= DisplayData.abnormnalSignalUnix[0])
				{
						return;
				}
		}
		for(i = 29; i > 0; i--)
		{
				DisplayData.abnormalSignalType[i] = DisplayData.abnormalSignalType[i - 1];
				DisplayData.abnormnalSignal[i] = DisplayData.abnormnalSignal[i - 1];
				DisplayData.abnormnalSignalUnix[i] = DisplayData.abnormnalSignalUnix[i - 1];
		}
		DisplayData.abnormalSignalType[0] = type;
		DisplayData.abnormnalSignal[0] = time;
		DisplayData.abnormnalSignalUnix[0] = unix;
		if(DisplayData.abnormnalSignalIndex < 30)
		{
				//DisplayData.abnormalSignalType[DisplayData.abnormnalSignalIndex] = type;
				//DisplayData.abnormnalSignal[DisplayData.abnormnalSignalIndex] = time;
				//DisplayData.abnormnalSignalUnix[DisplayData.abnormnalSignalIndex] = unix;
				DisplayData.abnormnalSignalIndex += 1;
		}
}

static void OledGpioInit(void)
{
		//reset
		gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
		//DataCommand
		gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
		//power
		gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
	
		OLED_RESET_HIGH;
		OLED_DATA_REG;
		OLED_POWER_ON;
}

static void Write_Command(uint8_t Data)
{
		SPI_CS_LOW;
		OLED_COMMAND_REG;
		spi_send_byte(Data);
		OLED_DATA_REG;
		SPI_CS_HIGH;
		
}

static void Write_Data(uint8_t Data)
{
		SPI_CS_LOW;
		OLED_DATA_REG;
		spi_send_byte(Data);
		OLED_DATA_REG;
		SPI_CS_HIGH;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Instruction Setting
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Set_Start_Column(unsigned char d)
{
	Write_Command(0x00+d%16);		// Set Lower Column Start Address for Page Addressing Mode
						//   Default => 0x00
	Write_Command(0x10+d/16);		// Set Higher Column Start Address for Page Addressing Mode
						//   Default => 0x10
}


void Set_Addressing_Mode(unsigned char d)
{
	Write_Command(0x20);			// Set Memory Addressing Mode
	Write_Command(d);			//   Default => 0x02
						//     0x00 => Horizontal Addressing Mode
						//     0x01 => Vertical Addressing Mode
						//     0x02 => Page Addressing Mode
}


void Set_Column_Address(unsigned char a, unsigned char b)
{
	Write_Command(0x21);			// Set Column Address
	Write_Command(a);			//   Default => 0x00 (Column Start Address)
	Write_Command(b);			//   Default => 0x7F (Column End Address)
}


void Set_Page_Address(unsigned char a, unsigned char b)
{
	Write_Command(0x22);			// Set Page Address
	Write_Command(a);			//   Default => 0x00 (Page Start Address)
	Write_Command(b);			//   Default => 0x07 (Page End Address)
}


void Set_Start_Line(unsigned char d)
{
	Write_Command(0x40|d);			// Set Display Start Line
						//   Default => 0x40 (0x00)
}


void Set_Contrast_Control(unsigned char d)
{
	Write_Command(0x81);			// Set Contrast Control
	Write_Command(d);			//   Default => 0x7F
}


void Set_Charge_Pump(unsigned char d)
{
	Write_Command(0x8D);			// Set Charge Pump
	Write_Command(d);			//   Default => 0x10
						//     0x10 => Disable Charge Pump
						//     0x14 => Enable Charge Pump
}


void Set_Segment_Remap(unsigned char d)
{
	Write_Command(d);			// Set Segment Re-Map
						//   Default => 0xA0
						//     0xA0 => Column Address 0 Mapped to SEG0
						//     0xA1 => Column Address 0 Mapped to SEG127
}


void Set_Entire_Display(unsigned char d)
{
	Write_Command(d);			// Set Entire Display On / Off
						//   Default => 0xA4
						//     0xA4 => Normal Display
						//     0xA5 => Entire Display On
}


void Set_Inverse_Display(unsigned char d)
{
	Write_Command(d);			// Set Inverse Display On/Off
						//   Default => 0xA6
						//     0xA6 => Normal Display
						//     0xA7 => Inverse Display On
}


void Set_Multiplex_Ratio(unsigned char d)
{
	Write_Command(0xA8);			// Set Multiplex Ratio
	Write_Command(d);			//   Default => 0x3F (1/64 Duty)
}


void Set_Display_On_Off(unsigned char d)	
{
	Write_Command(d);			// Set Display On/Off
						//   Default => 0xAE
						//     0xAE => Display Off
						//     0xAF => Display On
}


void Set_Start_Page(unsigned char d)
{
	Write_Command(0xB0|d);			// Set Page Start Address for Page Addressing Mode
						//   Default => 0xB0 (0x00)
}


void Set_Common_Remap(unsigned char d)
{
	Write_Command(d);			// Set COM Output Scan Direction
						//   Default => 0xC0
						//     0xC0 => Scan from COM0 to 63
						//     0xC8 => Scan from COM63 to 0
}


void Set_Display_Offset(unsigned char d)
{
	Write_Command(0xD3);			// Set Display Offset
	Write_Command(d);			//   Default => 0x00
}


void Set_Display_Clock(unsigned char d)
{
	Write_Command(0xD5);			// Set Display Clock Divide Ratio / Oscillator Frequency
	Write_Command(d);			//   Default => 0x80
						//     D[3:0] => Display Clock Divider
						//     D[7:4] => Oscillator Frequency
}


void Set_Precharge_Period(unsigned char d)
{
	Write_Command(0xD9);			// Set Pre-Charge Period
	Write_Command(d);			//   Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
						//     D[3:0] => Phase 1 Period in 1~15 Display Clocks
						//     D[7:4] => Phase 2 Period in 1~15 Display Clocks
}


void Set_Common_Config(unsigned char d)
{
	Write_Command(0xDA);			// Set COM Pins Hardware Configuration
	Write_Command(d);			//   Default => 0x12
						//     Alternative COM Pin Configuration
						//     Disable COM Left/Right Re-Map
}


void Set_VCOMH(unsigned char d)
{
	Write_Command(0xDB);			// Set VCOMH Deselect Level
	Write_Command(d);			//   Default => 0x20 (0.77*VCC)
}


void Set_NOP()
{
	Write_Command(0xE3);			// Command for No Operation
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Global Variables
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel		((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fill_RAM(unsigned char Data)
{
unsigned char i,j;

	for(i=0;i<8;i++)
	{
		Set_Start_Page(i);
		Set_Start_Column(0x02);

		for(j=0;j<128;j++)
		{
			Write_Data(Data);
		}
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Partial or Full Screen)
//
//    a: Start Page
//    b: End Page
//    c: Start Column
//    d: Total Columns
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fill_Block(unsigned char Data, unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
unsigned char i,j;
	
	for(i=a;i<(b+1);i++)
	{
		Set_Start_Page(i);
		Set_Start_Column(c);

		for(j=0;j<d;j++)
		{
			Write_Data(Data);
		}
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Checkboard (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Checkerboard()
{
unsigned char i,j;
	
	for(i=0;i<8;i++)
	{
		Set_Start_Page(i);
		Set_Start_Column(0x00);

		for(j=0;j<64;j++)
		{
			Write_Data(0x55);
			Write_Data(0xaa);
		}
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Frame (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Frame(void)
{
unsigned char i,j;
	
	Set_Start_Page(0x00);
	Set_Start_Column(XLevel);

	for(i=0;i<Max_Column;i++)
	{
		Write_Data(0x01);
	}

	Set_Start_Page(0x07);
	Set_Start_Column(XLevel);

	for(i=0;i<Max_Column;i++)
	{
		Write_Data(0x80);
	}

	for(i=0;i<8;i++)
	{
		Set_Start_Page(i);

		for(j=0;j<Max_Column;j+=(Max_Column-1))
		{
			Set_Start_Column(XLevel+j);

			Write_Data(0xFF);
		}
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Pattern (Partial or Full Screen)
//
//    a: Start Page
//    b: End Page
//    c: Start Column
//    d: Total Columns
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_Pattern(unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
unsigned char *Src_Pointer;
unsigned char i,j;

	Src_Pointer=Data_Pointer;
	for(i=a;i<(b+1);i++)
	{
		Set_Start_Page(i);
		Set_Start_Column(c);

		for(j=0;j<d;j++)
		{
			Write_Data(*Src_Pointer);
			Src_Pointer++;
		}
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Chinese (Partial or Full Screen)
//
//    StartPage: Start Page
//    StartColum: Start Column
//    NumHanzi: Numbert of hanzi
//		mode: 0 normal dispaly, 1 reverse display
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void ShowChinese(const uint8_t *Data_Pointer, uint8_t StartPage, uint8_t StartColum, uint8_t NumHanzi, uint8_t mode)
{
		uint8_t i,j,k;

		StartColum += 2;
		for(i = 0; i < NumHanzi; i++)
		{
				for(j = 0; j < 2; j++)
				{
						Set_Start_Page(StartPage + j);
						Set_Start_Column(StartColum + 16 * i);
						for(k = 0; k < 16; k++)
						{
								if(!mode)
								{
										Write_Data(*Data_Pointer);
								}
								else
								{
										Write_Data(~(*Data_Pointer));
								}
								Data_Pointer++;
						}
				}
		}
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show ascii (Partial or Full Screen)
//
//    StartPage: Start Page
//    StartColun: Start Column
//    NumChar: Numbert of char, max 20
//		mode: 0 normal dispaly, 1 reverse display
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void ShowChar(uint8_t *ShowStr, uint8_t NumChar, uint8_t StartPage, uint8_t StartColum, uint8_t mode)
{
		uint8_t i,j,k;
	
		StartColum += 2;
#if 0
		uint8_t TempCode[20][16] = {0};
		uint8_t *CodeSrc = &TempCode[0][0];
		
		for(i = 0; i < NumChar; i++)
		{
				for(j = 0; j < sizeof(AsciiIndex); j++)
				{
						if(ShowStr[i] == AsciiIndex[j])
						{
								memcpy(&TempCode[i][0], &AsciiCode[j][0], 16);
								break;
						}
				}
		}
#endif
		for(i = 0; i < NumChar; i++)
		{
				for(j = 0; j < 2; j++)
				{
						Set_Start_Page(StartPage + j);
						Set_Start_Column(StartColum + 8 * i);
						for(k = 0; k < 8; k++)
						{
								if(!mode)
								{
										Write_Data(*ShowStr);
								}
								else
								{
										Write_Data(~(*ShowStr));
								}
								ShowStr++;
						}
				}
		}
}

void MechProShowScrolling(uint8_t dataSrc[2][280], uint16_t numColun, uint8_t startPage, uint16_t index)
{
		uint8_t i,j;
		uint8_t temp = 0;
	
		if(numColun >= 128) 
		{
				temp = (numColun - index) > 128 ? 128 : (numColun - index);
				//temp = (index + 128) > MechProDataCloseCodeLen ? MechProDataCloseCodeLen : (index + 128);
				Set_Start_Page(startPage);
				Set_Start_Column(0x02);
				for(i = 0; i < temp; i++)
				{
						Write_Data(dataSrc[0][i + index]);
				}
				if(i < 128)
				{
						for(j = 0; j < (128 - i); j++)
						{
								Write_Data(dataSrc[0][j]);
						}
				}
				Set_Start_Page(startPage + 1);
				Set_Start_Column(0x02);
				for(i = 0; i < temp; i++)
				{
						Write_Data(dataSrc[1][i + index]);
				}
				if(i < 128)
				{
						for(j = 0; j < (128 - i); j++)
						{
								Write_Data(dataSrc[1][j]);
						}
				}
		}
		else
		{
				Set_Start_Page(startPage);
				Set_Start_Column(0x02);
				for(i = 0; i < numColun; i++)
				{
						Write_Data(dataSrc[0][i]);
				}
				for(i = numColun; i < 128; i++)
				{
						Write_Data(0x00);
				}
				Set_Start_Page(startPage + 1);
				Set_Start_Column(0x02);
				for(i = 0; i < numColun; i++)
				{
						Write_Data(dataSrc[1][i]);
				}
				for(i = numColun; i < 128; i++)
				{
						Write_Data(0x00);
				}
		}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Onep page full
//
//    a: Page number
//		b: full or clear
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void DisplayOnePage(unsigned char a, unsigned char b)
{
	unsigned char i;
	uint8_t tempData = 0;

	if(b)
	{
		tempData = 0xFF;
	}
	else
	{
		tempData = 0x00;
	}
	Set_Start_Page(a);
	Set_Start_Column(0);
	for(i = 0; i < 128; i++)
	{
		Write_Data(tempData);
	}
	Set_Start_Page(a + 1);
	Set_Start_Column(0);
	for(i = 0; i < 128; i++)
	{
		Write_Data(tempData);
	}
}

void Write8_8_char(uint8_t data, uint8_t page, uint8_t colum, uint8_t num)
{
		uint8_t i = 0;
		
		Set_Start_Page(page);
		Set_Start_Column(colum);
		for(i = 0; i < num; i++)
		{
				Write_Data(data);
		}		
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Vertical / Fade Scrolling (Partial or Full Screen)
//
//    a: Scrolling Direction
//       "0x00" (Upward)
//       "0x01" (Downward)
//    b: Set Top Fixed Area
//    c: Set Vertical Scroll Area
//    d: Set Numbers of Row Scroll per Step
//    e: Set Time Interval between Each Scroll Step
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Vertical_Scroll(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e)
{
unsigned int i,j;	

	Write_Command(0xA3);			// Set Vertical Scroll Area
	Write_Command(b);			//   Default => 0x00 (Top Fixed Area)
	Write_Command(c);			//   Default => 0x40 (Vertical Scroll Area)

	switch(a)
	{
		case 0:
			for(i=0;i<c;i+=d)
			{
				Set_Start_Line(i);
				for(j=0;j<e;j++)
				{
					delay_1us(200);
				}
			}
			break;
		case 1:
			for(i=0;i<c;i+=d)
			{
				Set_Start_Line(c-i);
				for(j=0;j<e;j++)
				{
					delay_1us(200);
				}
			}
			break;
	}
	Set_Start_Line(0x00);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Continuous Horizontal Scrolling (Partial or Full Screen)
//
//    a: Scrolling Direction
//       "0x00" (Rightward)
//       "0x01" (Leftward)
//    b: Define Start Page Address
//    c: Define End Page Address
//    d: Set Time Interval between Each Scroll Step in Terms of Frame Frequency
//    e: delay_1ms Time
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Horizontal_Scroll(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e)
{
	Write_Command(0x26|a);			// Horizontal Scroll Setup
	Write_Command(0x00);			//           => (Dummy Write for First Parameter)
	Write_Command(b);
	Write_Command(d);
	Write_Command(c);
	Write_Command(0x2F);			// Activate Scrolling
	delay_1ms(e);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Continuous Vertical / Horizontal / Diagonal Scrolling (Partial or Full Screen)
//
//    a: Scrolling Direction
//       "0x00" (Vertical & Rightward)
//       "0x01" (Vertical & Leftward)
//    b: Define Start Row Address (Horizontal / Diagonal Scrolling)
//    c: Define End Page Address (Horizontal / Diagonal Scrolling)
//    d: Set Top Fixed Area (Vertical Scrolling)
//    e: Set Vertical Scroll Area (Vertical Scrolling)
//    f: Set Numbers of Row Scroll per Step (Vertical / Diagonal Scrolling)
//    g: Set Time Interval between Each Scroll Step in Terms of Frame Frequency
//    h: delay_1ms Time
//    * d+e must be less than or equal to the Multiplex Ratio...
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Continuous_Scroll(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f, unsigned char g, unsigned char h)
{
	Write_Command(0xA3);			// Set Vertical Scroll Area
	Write_Command(d);			//   Default => 0x00 (Top Fixed Area)
	Write_Command(e);			//   Default => 0x40 (Vertical Scroll Area)

	Write_Command(0x29+a);			// Continuous Vertical & Horizontal Scroll Setup
	Write_Command(0x00);			//           => (Dummy Write for First Parameter)
	Write_Command(b);
	Write_Command(g);
	Write_Command(c);
	Write_Command(f);
	Write_Command(0x2F);			// Activate Scrolling
	delay_1ms(h);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Deactivate Scrolling (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Deactivate_Scroll()
{
	Write_Command(0x2E);			// Deactivate Scrolling
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Fade In (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fade_In()
{
unsigned int i;	

	Set_Display_On_Off(0xAF);
	for(i=0;i<(Brightness+1);i++)
	{
		Set_Contrast_Control(i);
		delay_1us(200);
		delay_1us(200);
		delay_1us(200);
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Fade Out (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fade_Out()
{
unsigned int i;	

	for(i=(Brightness+1);i>0;i--)
	{
		Set_Contrast_Control(i-1);
		delay_1us(200);
		delay_1us(200);
		delay_1us(200);
	}
	Set_Display_On_Off(0xAE);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Sleep Mode
//
//    "0x00" Enter Sleep Mode
//    "0x01" Exit Sleep Mode
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Sleep(unsigned char a)
{
	switch(a)
	{
		case 0:
			Set_Display_On_Off(0xAE);
			Set_Entire_Display(0xA5);
			break;
		case 1:
			Set_Entire_Display(0xA4);
			Set_Display_On_Off(0xAF);
			break;
	}
}


void Oled_ConfigInit(void)
{
		delay_1ms(100);
		OLED_RESET_LOW;
		delay_1ms(100);
		OLED_RESET_HIGH;
		delay_1ms(100);

		Set_Display_On_Off(0xAE);		// Display Off (0xAE/0xAF)
		Set_Display_Clock(0x80);		// Set Clock as 100 Frames/Sec	
		Set_Multiplex_Ratio(0x3F);		// 1/64 Duty (0x0F~0x3F)
		Set_Display_Offset(0x00);		// Shift Mapping RAM Counter (0x00~0x3F)
		Set_Start_Line(0x00);			// Set Mapping RAM Display Start Line (0x00~0x3F)
		Set_Charge_Pump(0x10);			// Disable Embedded DC/DC Converter (0x10/0x14)
		Set_Addressing_Mode(0x02);		// Set Page Addressing Mode (0x00/0x01/0x02)
		Set_Segment_Remap(0xA1);		// Set SEG/Column Mapping (0xA0/0xA1)
		Set_Common_Remap(0xC8);			// Set COM/Row Scan Direction (0xC0/0xC8)
		Set_Common_Config(0x12);		// Set Sequential Configuration (0x02/0x12)
		Set_Contrast_Control(Brightness);	// Set SEG Output Current
		Set_Precharge_Period(0x22);		// Set Pre-Charge as 2 Clocks & Discharge as 2 Clocks
		Set_VCOMH(0x40);			// Set VCOM Deselect Level
		Set_Entire_Display(0xA4);		// Disable Entire Display On (0xA4/0xA5)
		Set_Inverse_Display(0xA6);		// Disable Inverse Display On (0xA6/0xA7)

		Fill_RAM(0x00);				// Clear Screen

		Set_Display_On_Off(0xAF);		// Display On (0xAE/0xAF)
}

void OledInit(void)
{
		OledGpioInit();
		SpiInit();
		Oled_ConfigInit();
		//TestFun();
}
