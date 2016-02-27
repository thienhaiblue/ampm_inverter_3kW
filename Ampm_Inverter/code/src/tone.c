#include "lpc17xx.h"                        /* LPC21xx definitions */
#include "lpc17xx_pwm.h"
#include "tone.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_clkpwr.h"
#include "interrupt_priority.h"
#include <stdio.h>

#define PWM_CYCLE		1200

#define MR0_INT			1 << 0
#define MR1_INT			1 << 1
#define MR2_INT			1 << 2
#define MR3_INT			1 << 3
#define MR4_INT			1 << 8
#define MR5_INT			1 << 9
#define MR6_INT			1 << 10

#define TCR_CNT_EN		0x00000001
#define TCR_RESET		0x00000002
#define TCR_PWM_EN		0x00000008

#define PWMMR0I			1 << 0
#define PWMMR0R			1 << 1
#define PWMMR0S			1 << 2
#define PWMMR1I			1 << 3
#define PWMMR1R			1 << 4
#define PWMMR1S			1 << 5
#define PWMMR2I			1 << 6
#define PWMMR2R			1 << 7
#define PWMMR2S			1 << 8
#define PWMMR3I			1 << 9
#define PWMMR3R			1 << 10
#define PWMMR3S			1 << 11
#define PWMMR4I			1 << 12
#define PWMMR4R			1 << 13
#define PWMMR4S			1 << 14
#define PWMMR5I			1 << 15
#define PWMMR5R			1 << 16
#define PWMMR5S			1 << 17
#define PWMMR6I			1 << 18
#define PWMMR6R			1 << 19
#define PWMMR6S			1 << 20

#define PWMSEL2			1 << 2
#define PWMSEL3			1 << 3
#define PWMSEL4			1 << 4
#define PWMSEL5			1 << 5
#define PWMSEL6			1 << 6
#define PWMENA1			1 << 9
#define PWMENA2			1 << 10
#define PWMENA3			1 << 11
#define PWMENA4			1 << 12
#define PWMENA5			1 << 13
#define PWMENA6			1 << 14

#define LER0_EN			1 << 0
#define LER1_EN			1 << 1
#define LER2_EN			1 << 2
#define LER3_EN			1 << 3
#define LER4_EN			1 << 4
#define LER5_EN			1 << 5
#define LER6_EN			1 << 6


uint32_t  pointer = 0;
uint32_t octave = 0;
uint32_t duration = 0;
uint32_t note = 0;
uint32_t defaultoctave = 0;
uint32_t defaultduration = 0;
uint32_t beat_speed = 0;
uint32_t beep = 0,run_tone = 1;
uint32_t frequency_note = 440;
const uint8_t static Tone[] = {"2b,2b,2b,2p,2b,2b,2b,2p,2b,2d6,2g,2a,1b,2c6,2c6,1c6,2b,2b,1b,2b,2a,2a,2b,1a,2d6 S"};
uint8_t  melody[1000] = {"8g.,16g,a,g,c6,2b,8g.,16g,a,g,d6,2c6,8g.,16g,g6,e6,c6,b,a,8f6.,16f6,e6,c6,d6,2c6,8g.,16g,a,g,c6,2b,8g.,16g,a,g,d6,2c6,8g.,16g,g6,e6,c6,b,a,8f6.,16f6,e6,c6,d6,2c6 S"};
const uint8_t static AxelF[] = {"32p,8g,8p,16a#.,8p,16g,16p,16g,8c6,8g,8f,8g,8p,16d.6,8p,16g,16p,16g,8d#6,8d6,8a#,8g,8d6,8g6,16g,16f,16p,16f,8d,8a#,2g,4p,16f6,8d6,8c6,8a#,4g,8a#.,16g,16p,16g,8c6,8g,8f,4g,8d.6,16g,16p,16g,8d#6,86,8a#,8g,8d6,8g6,16g,16f,16p,16f,8d,8a#,2g  S"};
const uint8_t static Itchy[] = {"8c,8a5,4p,8c,8a,4p,8c,a5,8c,a5,8c,8a,4p,8p,8c,8d,8e,8p,8e,8f,8g,4p,8d,8c,4d,8f,4a#,4a,2c7 S"};
const uint8_t static MissionImpossible[] = {"16d5,16d#5,16d5,16d#5,16d5,16d#5,16d5,16d5,16d#5,16e5,16f5,16f#5,16g5,8g5,4p,8g5,4p,8a#5,8p,8c6,8p,8g5,4p,8g5,4p,8f5,8p,8p,8g5,4p,4p,8a#5,8p,8c6,8p,8g5,4p,4p,8f5,8p,8f#5,8p,8a#5,8g5,1d5 S"};
const uint8_t static JingleBell[] = {"2b,32p,2b,32p,2b,2p,2b,32p,2b,32p,2b,2p,2b,2d6,2g,2a,1b,2c6,32p,2c6,32p,1c6,2b,2b,1b,2b,2a,32p,2a,2b,1a,2d6, S"};
const uint8_t static WeWishYouAMerryChristmas[] = {"d, g, 8g, 8a, 8g, 8f#, e, c, e, a, 8a, 8b, 8a, 8g, f#, d, f#, b, 8b, 8c6, 8b, 8a, g, e, d, e, S"};

BEEP_TYPE buzzer;

uint8_t buff_dummy[1024];




uint32_t pwm_timer = 0;
extern uint32_t beep;

void PWM1_IRQHandler (void)
{
    uint32_t regVal;
    regVal = LPC_PWM1->IR;
    if ( regVal & MR0_INT )
    {
		if(pwm_timer) pwm_timer--;
		else if(beep)
		{
			DeTone();
		}	
    }
	LPC_PWM1->IR |= regVal;		/* clear interrupt flag on match 0 */
}
void InitBuzzer( uint32_t cycle )
{
	PINSEL_CFG_Type PinCfg;

	buzzer.beep_start = 0;

    PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 2;
	PinCfg.Pinnum = 2;
	PINSEL_ConfigPin(&PinCfg);

    LPC_PWM1->TCR = TCR_RESET;		/* Counter Reset */ 
		
    LPC_PWM1->PR = 0x00;		/* count frequency:Fpclk */
    LPC_PWM1->MCR = /*PWMMR0I | */ PWMMR0R;	/* interrupt on PWMMR0, reset on PWMMR0, reset 
				TC if PWM0 matches */				
    LPC_PWM1->MR0 = cycle;
	LPC_PWM1->MR3 = cycle/2;	

    /* PWM5 latch enabled */
    LPC_PWM1->LER = LER0_EN | LER3_EN;
/* preemption = 1, sub-priority = 0 */
	NVIC_SetPriority(PWM1_IRQn, ((0x01<<3)|PRIORITY_PWM1));
	/* Enable interrupt for timer 0 */
//	NVIC_EnableIRQ(PWM1_IRQn);
	LPC_PWM1->TCR = TCR_CNT_EN | TCR_PWM_EN;
}

void SetPWM( uint32_t cycle,uint32_t duty)
{	
	LPC_PWM1->MR0 = cycle;		
    LPC_PWM1->MR3 = cycle*duty/100;
    /* The LER will be cleared when the Match 5 takes place, in order to
    load and execute the new value of match registers */
    LPC_PWM1->LER = LER0_EN | LER3_EN;
}

void StartPWM ( void )
{
    /* PWM5 single edge, PWM5 enable */
    LPC_PWM1->PCR = PWMENA3;
    LPC_PWM1->TCR = TCR_CNT_EN | TCR_PWM_EN;	/* counter enable, PWM enable */
}

void StopPWM ( void )
{
    LPC_PWM1->PCR = 0;
    LPC_PWM1->TCR = 0x2;		/* Stop all PWMs */
}



void Play1Note(uint32_t note,uint32_t duration,uint32_t octave)
{	uint32_t i;
	defaultoctave = octave;
	defaultduration = duration;
	beat_speed = 300;
	pwm_timer = 0;
	pointer = 0;
	beep = 1;
	run_tone = 1;
	for(i = 0;i < 4; i++)
	{
	   melody[i] = ' ';
	}
	melody[0] = note;
	SetPWM(100,100);
	StartPWM();

}	  
void RunTone(uint32_t melody_number)
{
	pwm_timer = 0;
	if(beep == 0)
	{
		beep = 1;
		run_tone = 1;
		pointer = 0;
		LoadMelody(melody_number);
		SetPWM (100,100);
		StartPWM();
	}
	else
	{	
		run_tone = 0;
	}
}
void LoadMelody(uint32_t melody_number)
{	uint32_t i=0,j=0;
	uint8_t temp = 0;
	switch(melody_number)
	{
		case 0:
			do
			{
				temp = AxelF[i];
				if(temp != ' ')
				{
					melody[j] = temp;
					j++;
				}
			i++;
			}
			while(temp != 'S');
			defaultoctave = 5;
			defaultduration = 4;
			beat_speed = 125;
			break;
		case 1:
			for(i = 0;(temp = MissionImpossible[i]) != 'S';i++)
			{ 
				if(temp != ' ')
				{
					melody[j] = temp;
					j++;
				}	
			}
			defaultoctave = 6;
			defaultduration = 4;
			beat_speed = 150;
			break;
		case 2:
			for(i = 0;(temp = Itchy[i]) != 'S';i++)
			{
				if(temp != ' ')
				{
					melody[j] = temp;
					j++;
				}
			}
			defaultoctave = 6;
			defaultduration = 8;
			beat_speed = 198;
			break;
		case 3:
			for(i = 0;(temp = Tone[i]) != 'S';i++)
			{
				if(temp != ' ')
				{
					melody[j] = temp;
					j++;
				}
			}
			defaultoctave = 5;
			defaultduration = 4;
			beat_speed = 200;
			break;
		case 4:
			for(i = 0;(temp = JingleBell[i]) != 'S';i++)
			{  if(temp != ' ')
				{
					melody[j] = temp;
					j++;
				}	
			}
			defaultoctave = 5;
			defaultduration = 4;
			beat_speed = 355;							
			break;
		case 5:
			for(i = 0;(temp = WeWishYouAMerryChristmas[i]) != 'S';i++)
			{  if(temp != ' ')
				{
					melody[j] = temp;
					j++;
				}	
			}
			defaultoctave = 5;
			defaultduration = 4;
			beat_speed = 125;							  
			break;
		default :
				break;
	}
	i = j+3;
	for(;j<i;j++)
		{
			melody[j] = ' ';
		}	
}
void DeTone(void)
{ 
if(((pointer == 0) || (melody[pointer++] == ',')) && run_tone) 
 {
        octave = defaultoctave; 	/* Set Default Octave */

        if ((melody[pointer] == '3') && (melody[pointer+1] == '2'))
		{
            duration = 32;
            pointer += 2;
        }
        else if ((melody[pointer] == '1') && (melody[pointer+1] == '6')) 
		{
            duration = 16;
            pointer += 2;	
        }
        else if (melody[pointer] == '8') 
		{
            duration = 8;
            pointer++;
        }
        else if (melody[pointer] == '4')
		 {
            duration = 4;
            pointer++;
        }
        else if (melody[pointer] == '2')
		 {
            duration = 2;
            pointer++;
        }
        else if (melody[pointer] == '1')
		{
            duration = 1;
            pointer++;
        } 
		else duration = defaultduration;
        if (melody[pointer + 1] == '#')
		{

            /* Process Sharps */

            switch (melody[pointer])
			{
                case 'a' : note = 53629;	/*60e6*(1/466.1638)	==	A#*/
                           break;
                case 'c' : note = 45097;		/*60e6*(1/554.3653)	==	C#*/
                           break;
                case 'd' : note = 40176;		   /*60e6*(1/622.2540)	==	D#*/
                           break;
                case 'f' : note = 33784;		   /*60e6*(1/739.9888)	==	F#*/
                           break;
                case 'g' : note = 30098;		  /*60e6*(1/830.6094)	==	G#*/
                           break;
            }
            pointer +=2;

        } 
		else 
		{

            switch (melody[pointer])
			{
                case 'a' : note = 56818;	 /*60e6*(1/440)	== A*/
                           break;			 
                case 'b' : note = 50620;	 /*60e6*(1/493.88)	==	B*/
                           break;
                case 'c' : note = 47778;	 /*60e6*(1/523.2511)	==	C*/
                           break;
                case 'd' : note = 42565;	/*60e6*(1/587.3295)	==	D*/
                           break;
                case 'e' : note = 37921;    /*60e6*(1/659.2551)	==	E*/
                           break;					   
                case 'f' : note = 35793;		 /*60e6*(1/698.4565)	==	F*/
                           break;
                case 'g' : note = 31888;		 /*60e6*(1/783.9909)	==	B*/
                           break;
                case 'p' : note = 0;
                           break;

            }
            pointer++;
        }

        if (melody[pointer] == '.') 
		{
            /* Duration 1.5x */
            duration = duration + 128;
            pointer++;
        }
        if (melody[pointer] == '4') 
		{
            octave = 4;
            pointer++;
        } 
		else if (melody[pointer] == '5') 
		{
            octave = 5;
            pointer++;
        } 
		else     if (melody[pointer] == '6')
		{
            octave = 6;
            pointer++;
        }
		 else     if (melody[pointer] == '7') 
		 {
            octave = 7;
            pointer++;
        } 

        if (melody[pointer] == '.') 
		{
            /* Duration 1.5x */
            duration = duration + 128;
            pointer++;
        }

        PlayNote(note, octave, duration);
}
else
{
	beep = 0;
	pointer = 0;
	StopPWM();	
}
}
void PlayNote(uint32_t note,uint32_t octave,uint32_t duration )
{		uint32_t duration_t1;

    /* Process octave */
    switch (octave) 
	{
        case 4 : /* Do nothing */
             break;
        case 5 : /* %2 */
             note = note >> 1; 
             break;
        case 6 : /* %4 */
             note = note >> 2;
             break;
        case 7 : /* %8 */
             note = note >> 4;
             break;
    }   
    duration_t1 = (duration & 0x0000007F);
    /* If duration is 1.5x add .5 to duration */
    if (duration & 0x00000080) duration_t1 = (duration_t1 + (duration_t1 >> 1));  
	if(note) 
	{ 
		duration_t1 = 60e6 / (float)(note * (float)((float)beat_speed / 60) * duration_t1);
		pwm_timer = duration_t1;
		SetPWM(note,50);	
	}
	else
	{
		note = 10000;
		duration_t1 = 60e6 / (float)(note * (float)((float)beat_speed / 60) * duration_t1);
		pwm_timer = duration_t1;
		SetPWM (note,100);
	}
}
