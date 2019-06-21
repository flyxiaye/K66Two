#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#define ROW1 C5
#define ROW2 C6
#define ROW3 C7

#define COL1 C3
#define COL2 C2
#define COL3 C1

int Key_Scan(void);
int Key_Check(void);
void INTR_Keyboard_Init(void);
extern char g_Key;


#endif