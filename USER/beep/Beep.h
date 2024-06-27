#ifndef __BEEP__H
#define __BEEP__H

#include "common.h"
#include "stdlib.h"	  


//定义GPIOB的位地址变量宏，位输入宏，输出宏
#define PBeep PBout(5)

void BEEP_Init(void);
void Sound(u16 frq);
void Sound2(u16 time);
void play_music(void);
void play_successful(void);
void play_failed(void);
void beep_qidong(void);

#endif
