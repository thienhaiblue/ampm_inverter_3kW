#ifndef __TONE_H
#define __TONE_H

#ifdef __cplusplus
extern "C" {
#endif



#define Note_A 56818	 /*60e6*(1/440)		== A*/			 
#define Note_B 50620	 /*60e6*(1/493.88)	==	B*/
#define Note_C 47778	 /*60e6*(1/523.2511)==	C*/
#define Note_D 42565	/*60e6*(1/587.3295)	==	D*/
#define Note_E 37921  /*60e6*(1/659.2551)	==	E*/					   
#define Note_F 35793	/*60e6*(1/698.4565)	==	F*/
#define Note_G 31888	/*60e6*(1/783.9909)	==	G*/

typedef struct {
   uint32_t beep_speed;
   uint32_t beep_note;
   uint32_t beep_start;
}
BEEP_TYPE;
extern BEEP_TYPE buzzer;

void StartPWM ( void );
void StopPWM ( void );
void SetPWM( uint32_t cycle,uint32_t duty);


void InitBuzzer( uint32_t cycle );
void RunTone(uint32_t melody_number);
void PlayNote(uint32_t note,uint32_t octave,uint32_t duration );
void DeTone(void);
void LoadMelody(uint32_t melody_number);
void Play1Note(uint32_t note,uint32_t duration,uint32_t octave);

#ifdef __cplusplus
}
#endif

#endif /* end __PWM_H */
/****************************************************************************
**                            End Of File
****************************************************************************/
