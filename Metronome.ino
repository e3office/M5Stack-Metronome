#include <M5Stack.h>
#include <Ticker.h>

#define SPEAKER (25)

#define BEEP_FREQ1 (1)
#define BEEP_FREQ2 (3)

#define COLOR_BG (BLACK)
#define COLOR_FG (WHITE)
#define COLOR_SEL (YELLOW)

#define TEMPO_MIN (15)
#define TEMPO_MAX (300)

// actual beats-1, Ex. set 7 for 8 beats max.
#define BEATS_MAX (7)

Ticker xTicker;

int iSelectedItem=0; // 0: TEMPO, 1: BEATS, 2: START
int iTempo=120; // TEMPO_MIN ... TEMPO_MAX
int iBeats=3; // 0 (Off), 1 (2 beats) ... BEATS_MAX
int iStart=0; // 0: Stopped, 1: Running

int iBeatsCount;
uint32_t ulTempoMilliseconds=500UL;

void tickerFunc(void)
{
	xTicker.once_ms(ulTempoMilliseconds,tickerFunc);

	if(iBeats!=0 && iBeatsCount==0)
	{
		digitalWrite(SPEAKER,HIGH);
		delay(BEEP_FREQ1);
		digitalWrite(SPEAKER,LOW);
		delay(BEEP_FREQ1);
		digitalWrite(SPEAKER,HIGH);
		delay(BEEP_FREQ1);
		digitalWrite(SPEAKER,LOW);
		delay(BEEP_FREQ1);
		digitalWrite(SPEAKER,HIGH);
		delay(BEEP_FREQ1);
		digitalWrite(SPEAKER,LOW);
		delay(BEEP_FREQ1);
		digitalWrite(SPEAKER,HIGH);
		delay(BEEP_FREQ1);
		digitalWrite(SPEAKER,LOW);
		delay(BEEP_FREQ1);
	}
	else
	{
		digitalWrite(SPEAKER,HIGH);
		delay(BEEP_FREQ2);
		digitalWrite(SPEAKER,LOW);
	}

	if(++iBeatsCount>iBeats) iBeatsCount=0;
}

void DrawSelection(void)
{
	switch(iSelectedItem)
	{
		case 0: // TEMPO
		{
			M5.Lcd.fillRect(24,18,18,30,COLOR_SEL);
			M5.Lcd.fillRect(42,46,255,2,COLOR_SEL);

			M5.Lcd.fillRect(24,83,18,30,COLOR_BG);
			M5.Lcd.fillRect(42,111,255,2,COLOR_BG);

			M5.Lcd.fillRect(24,148,18,30,COLOR_BG);
			M5.Lcd.fillRect(42,176,255,2,COLOR_BG);
		} break;

		case 1: // BEATS
		{
			M5.Lcd.fillRect(24,18,18,30,COLOR_BG);
			M5.Lcd.fillRect(42,46,255,2,COLOR_BG);

			M5.Lcd.fillRect(24,83,18,30,COLOR_SEL);
			M5.Lcd.fillRect(42,111,255,2,COLOR_SEL);

			M5.Lcd.fillRect(24,148,18,30,COLOR_BG);
			M5.Lcd.fillRect(42,176,255,2,COLOR_BG);
		} break;

		default: //case 2: // START
		{
			M5.Lcd.fillRect(24,18,18,30,COLOR_BG);
			M5.Lcd.fillRect(42,46,255,2,COLOR_BG);

			M5.Lcd.fillRect(24,83,18,30,COLOR_BG);
			M5.Lcd.fillRect(42,111,255,2,COLOR_BG);

			M5.Lcd.fillRect(24,148,18,30,COLOR_SEL);
			M5.Lcd.fillRect(42,176,255,2,COLOR_SEL);
		} break;
	}
}

void DispTempo(void)
{
	M5.Lcd.fillRect(202,22,40,17,COLOR_BG);
	M5.Lcd.setCursor(201,20);
	M5.Lcd.printf("%d",iTempo);
}

void DispBeats(void)
{
	M5.Lcd.fillRect(202,86,33,18,COLOR_BG);
	M5.Lcd.setCursor(201,85);
	if(iBeats==0) M5.Lcd.printf("Off");
	else M5.Lcd.printf("%d",iBeats+1);
}

void DispStart(void)
{
	M5.Lcd.fillRect(202,151,94,24,COLOR_BG);
	M5.Lcd.setCursor(201,150);
	M5.Lcd.printf(iStart? "Running" : "Stopped");
}

void setup()
{
	const bool boolLCDEnable=true;  // LCD display
	const bool boolSDEnable=false;  // SD card access
	const bool boolSerialEnable=false; // serial port, by default 115200bps
	const bool boolI2CEnable=false; // I2C bus

	M5.begin(boolLCDEnable,boolSDEnable,boolSerialEnable,boolI2CEnable);
	M5.Power.begin();

	digitalWrite(SPEAKER,LOW);
	pinMode(SPEAKER,OUTPUT);

	M5.Lcd.clear(COLOR_BG);
	M5.Lcd.setTextFont(4);
	M5.Lcd.setTextSize(1);

	M5.Lcd.setTextColor(COLOR_BG);

	M5.Lcd.fillRect(24,214,85,22,COLOR_FG);
	M5.Lcd.setCursor(42,215);
	M5.Lcd.printf("SEL");
	M5.Lcd.setCursor(43,215);
	M5.Lcd.printf("SEL");

	M5.Lcd.fillRect(118,214,85,22,COLOR_FG);
	M5.Lcd.setCursor(121,215);
	M5.Lcd.printf("DOWN");
	M5.Lcd.setCursor(122,215);
	M5.Lcd.printf("DOWN");

	M5.Lcd.fillRect(212,214,85,22,COLOR_FG);
	M5.Lcd.setCursor(236,215);
	M5.Lcd.printf("UP");
	M5.Lcd.setCursor(237,215);
	M5.Lcd.printf("UP");

	M5.Lcd.setTextColor(COLOR_FG);

	M5.Lcd.setCursor(50,20);
	M5.Lcd.printf("TEMPO");

	M5.Lcd.setCursor(50,85);
	M5.Lcd.printf("BEATS");

	M5.Lcd.setCursor(50,150);
	M5.Lcd.printf("START");

	DrawSelection();
	DispTempo();
	DispBeats();
	DispStart();
}

void SetTempo(void)
{
	ulTempoMilliseconds=60000UL/iTempo;
}

void DoUp(void)
{
	switch(iSelectedItem)
	{
		case 0: // TEMPO
		{
			if(iTempo<TEMPO_MAX)
			{
				iTempo++;
				SetTempo();
				DispTempo();
			}
		} break;

		case 1: // BEATS
		{
			if(iBeats<BEATS_MAX)
			{
				iBeats++;
				DispBeats();
			}
		} break;

		default: //case 2: // START
		{
			if(iStart)
			{
				iStart=0;
				xTicker.detach();
			}
			else
			{
				iStart=1;
				iBeatsCount=0;
				tickerFunc();
			}
			DispStart();
		} break;
	}
}

void DoDown(void)
{
	switch(iSelectedItem)
	{
		case 0: // TEMPO
		{
			if(iTempo>TEMPO_MIN)
			{
				iTempo--;
				SetTempo();
				DispTempo();
			}
		} break;

		case 1: // BEATS
		{
			if(iBeats>0)
			{
				iBeats--;
				DispBeats();
			}
		} break;

		default: //case 2: // START
		{
			if(iStart)
			{
				iStart=0;
				xTicker.detach();
			}
			else
			{
				iStart=1;
				iBeatsCount=0;
				tickerFunc();
			}
			DispStart();
		} break;
	}
}

void loop()
{
	static uint32_t ulRepDet_BtnB;
	static uint32_t ulRepDet_BtnC;

	M5.update();

	if(M5.BtnA.wasPressed())
	{
		if(++iSelectedItem>2) iSelectedItem=0;
		DrawSelection();
	}

	if(M5.BtnB.wasPressed())
	{
		ulRepDet_BtnB=400UL;
		DoDown();
	}

	if(M5.BtnB.pressedFor(ulRepDet_BtnB) && iSelectedItem==0)
	{
		ulRepDet_BtnB+=50UL;
		DoDown();
	}

	if(M5.BtnC.wasPressed())
	{
		ulRepDet_BtnC=400UL;
		DoUp();
	}

	if(M5.BtnC.pressedFor(ulRepDet_BtnC) && iSelectedItem==0)
	{
		ulRepDet_BtnC+=50UL;
		DoUp();
	}

	delay(1);
}
