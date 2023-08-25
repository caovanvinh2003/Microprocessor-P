#include<16f887.h>
#fuses intrc_io
#use delay(clock=8M)
#define     lcd_e       pin_e1
#define     lcd_rw      0
#define     lcd_rs      pin_e0
#define     output_lcd  output_d
#define     led8        pin_c7
#define     led7        pin_a7
#define     led6        pin_a6
#define     led5        pin_a5
#define     led4        pin_a3
#define     led3        pin_a2
#define     led2        pin_a1
#define     led1        pin_a0
#include<TV_LCD.c>
#include<TV_KEY4X4.c>
#bit TM1IF =0x0c.0
signed int8 T0,GHD=0, dem=0, mp, SAVE, chuc, dv, d=0, a=0, NS, Nang_Suat, TRAN=0;
unsigned int16 bdn1=0;
unsigned int8 giay=0, phut=0, gio=0;
const unsigned int8 ma7doan[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
void xuatma7doan()   
{  
   output_c(ma7doan[giay/1%10]); output_low(led8); delay_ms(1); output_high(led8);
   output_c(ma7doan[giay/10%10]); output_low(led7); delay_ms(1); output_high(led7);
   output_c(0xff-64); output_low(led6); delay_ms(1); output_high(led6);
   output_c(ma7doan[phut/1%10]); output_low(led5); delay_ms(1); output_high(led5);
   output_c(ma7doan[phut/10%10]); output_low(led4); delay_ms(1); output_high(led4);
   output_c(0xff-64); output_low(led3); delay_ms(1); output_high(led3);
   output_c(ma7doan[gio/1%10]); output_low(led2); delay_ms(1); output_high(led2);
   output_c(ma7doan[gio/10%10]); output_low(led1); delay_ms(1); output_high(led1);
}


void KIEMTRABANPHIM()
{
if (d==0)
{
mp=-1;
mp=key_4x4();
if (mp==0) {chuc=0; a=1;}
if (mp==1) {chuc=1; a=1;}
if (mp==2) {chuc=2; a=1;}
if (mp==3) {chuc=3; a=1;}
if (mp==4) {chuc=4; a=1;}
if (mp==5) {chuc=5; a=1;}
if (mp==6) {chuc=6; a=1;}
if (mp==7) {chuc=7; a=1;}
if (mp==8) {chuc=8; a=1;}
if (mp==9) {chuc=9; a=1;}
if (mp!=-1) d=1;
}

if (d==1)
{
mp=-1;
mp=key_4x4();
if (mp==0) {dv=0; a=1;}
if (mp==1) {dv=1; a=1;}
if (mp==2) {dv=2; a=1;}
if (mp==3) {dv=3; a=1;}
if (mp==4) {dv=4; a=1;}
if (mp==5) {dv=5; a=1;}
if (mp==6) {dv=6; a=1;}
if (mp==7) {dv=7; a=1;}
if (mp==8) {dv=8; a=1;}
if (mp==9) {dv=9; a=1;}
if (mp!=-1) d=0;
}
if (a==1) GHD=chuc*10+dv;
else GHD=20;
if (mp==15) dem++;
}

void DEMSP()
{

KIEMTRABANPHIM();
if (dem==0)
   {
   set_timer0(0); // �oc ket qua dem ve
   lcd_command(0x80);
   printf(lcd_data,"SAN PHAM:%d  CAI ",T0);
   lcd_command(0xC0);
   printf(lcd_data,"GIOI HAN:%d  CAI ",GHD);
   }
   
   if (dem==1)
   {
   T0= get_timer0(); // �oc ket qua dem ve
   if(T0>GHD) {set_timer0(1); tran++;} // khong che GHD
   lcd_command(0x80);
   printf(lcd_data,"SAN PHAM:%d  CAI ",T0);
   lcd_command(0xC0);
   printf(lcd_data,"GIOI HAN:%d  CAI ",GHD);
   }
   
   if (dem==2)
   {
   save=T0;
  
   lcd_command(0xd4+12);
   printf(lcd_data,"SAVED:%d ",SAVE);
   set_timer0(save);
   }
   
   if (dem==3)
   {
   T0= get_timer0(); // �oc ket qua dem ve
   if(T0>GHD) {set_timer0(1); tran++;} // khong che GHD
   lcd_command(0x80);
   printf(lcd_data,"SAN PHAM:%d  CAI ",T0);
   lcd_command(0xC0);
   printf(lcd_data,"GIOI HAN:%d  CAI ",GHD);
   lcd_command(0xd4+12);
   printf(lcd_data,"       ");
   }
}

void thoigian()
{
  if(TM1IF)
  { 
  set_timer1(3036);
            bdn1++;
            TM1IF=0;
            if(bdn1==4) 
                {
                bdn1=0; 
                giay++;
                NS=get_timer0();
                Nang_Suat=(NS+tran*GHD);
                if (giay==60)
                   {giay=0;
                    phut++;
                    tran=0;
                    Nang_Suat=0;
                    set_timer0(0);
                        if(phut==60)
                        {
                           phut=0;
                           gio++;
                              if(gio==24) gio=0;
                        }
                   }
                }
    }
lcd_command(0x94);
printf(lcd_data,"NANG SUAT:%d  Cai",Nang_Suat);
}
void main()
{
set_tris_a(0x10); //input A4
set_tris_b(1);
set_tris_c(0);
set_tris_d(0);
set_tris_e(0);
lcd_setup();
setup_timer_0(T0_EXT_H_TO_L|T0_DIV_1);//Chon canh tich cuc va bo chia truoc
set_timer0(0); //dat truoc so dem
setup_timer_1(T1_internal|T1_DIV_BY_8); //Cau h�nh cho timer 0
set_timer1(3036); 

   while(true)
   {  

      thoigian();    
      xuatma7doan();

DEMSP();
lcd_command(0xd4);
printf(LCD_data,"Phim F: %d ", dem);
   }
   
}
