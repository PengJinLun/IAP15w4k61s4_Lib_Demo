#ifndef GUI_H
#define GUI_H

#include "config.h"

/*GUI����*/
void GUI_Clear(u16 color);	//����
void GUI_Point(u8 x, u16 y, u16 color);//����
void GUI_line(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)  ;//��ֱ��
void GUI_box(u8 sx,u16 sy,u8 ex,u16 ey,u16 color);//��ʵ�ľ���
void GUI_arc(u16 rx,u16 ry,u16 r,u16 color,u8 mode);			 //ָ��λ�û�Բ
void GUI_arcrectangle(u16 x,u16 y,u16 width,u16 height,u8 r,u16 upcolor,u16 downcolor);  //��Բ�Ǿ���
void GUI_sprintf_char(u8 x,u16 y,u8 value,u16 dcolor,u16 bgcolor)	;
void GUI_sprintf_hz1616(u8 x, u16  y, u8 c[2], u16 dcolor,u16 bgcolor);
void GUI_sprintf_hzstr16x(u8 x1,u16 y1,u8 *str,u16 dcolor,u16 bgcolor);
void GUI_sprintf_hz3232(u8 x, u16  y, u8 c[2], u16 dcolor,u16 bgcolor);
void GUI_sprintf_ZM3216(u8 x, u16  y, u8 c, u16 dcolor,u16 bgcolor);
void GUI_sprintf_ZMHZs(u8 x1,u16 y1,u8 *str,u16 dcolor,u16 bgcolor)	 ;



/*���峣����ɫ��*/
#define Red      0xf800	//��
#define Yellow   0xffe0	//��
#define Green    0x07e0	//��
#define Cyan     0x07ff	//��
#define Blue     0x001f	//��
#define Purple   0xf81f	//��
#define Black    0x0000	//��
#define White    0xffff	//��
#define Gray     0x7bef	//��
#define Blue1    0xa5ff //����
#define Blue2	 0x7cdf
#define Purple1  0x8a9e //����
#define Green1   0x0410 //ī��
#define Green2   0x2616
#define Blue3	 0x751E
#define Purple2  0xcd9e //����
#define Red2	 0XF260	//����
#define Red3	 0X8000	//�غ�
#define window   0XDED7
#endif
















