#define input_key_0 A0
#define input_key_1 A1
#define input_key_2 A2
#define input_key_3 A3
#define input_key_4 A4
#define input_key_5 A5
#define input_key_6 2
#define input_key_7 3
#define input_key_8 4
#define input_key_9 5

#define input_pwm_line_0 6
#define input_pwm_line_1 7
#define input_pwm_line_2 8
#define input_pwm_line_3 9
#define input_pwm_line_4 10

#include <SoftwareSerial.h> //only is you use software serial. you can use hardvare serial to communicate with pc andmp3 module in the same time
#include <DFPlayer_Mini_Mp3.h>//not needed if you'll send direct command
int key_triger=1; //keypressed
int sound_level=30; //default soud level
int shift=0; //octava shifting 7*shift
int play_list=0; //default directory for playlist
int track_number=0; //default first file in folder
byte sound_shema=0;//sound shema. 0=piano 1=clavesine ....
void setup()
{
  pinMode(input_pwm_line_0, OUTPUT);
  pinMode(input_pwm_line_1, OUTPUT);
  pinMode(input_pwm_line_2, OUTPUT);
  pinMode(input_pwm_line_3, OUTPUT);
  pinMode(input_pwm_line_4, OUTPUT);
  digitalWrite(input_pwm_line_0,HIGH);
  digitalWrite(input_pwm_line_1,LOW);
  digitalWrite(input_pwm_line_2,LOW);
  digitalWrite(input_pwm_line_3,LOW);
  digitalWrite(input_pwm_line_4,LOW);
  
  pinMode(input_key_0, INPUT_PULLUP);
  pinMode(input_key_1, INPUT_PULLUP);
  pinMode(input_key_2, INPUT_PULLUP);
  pinMode(input_key_3, INPUT_PULLUP);
  pinMode(input_key_4, INPUT_PULLUP);
  pinMode(input_key_5, INPUT_PULLUP);
  pinMode(input_key_6, INPUT_PULLUP);
  pinMode(input_key_7, INPUT_PULLUP);
  pinMode(input_key_8, INPUT_PULLUP);
  pinMode(input_key_9, INPUT_PULLUP);
 Serial.begin(9600); 
 
 mp3_set_serial (Serial);	//set Serial for DFPlayer-mini mp3 module 
 mp3_set_volume (30);
}
int triger_stop=0;
void loop()
{
  
 
 //Serial.println(read_key());
 //delay(100);
 int key=read_key();
 if ((key!=100)and(key_triger==0)){
 //mp3_play_physical(key);
 parse_key(key);
 //Serial.println(key);
 key_triger=1;}
 if (key==100) key_triger=0;
 }

void parse_key(int key)
{
  switch (key)
  {
   case 42:{
   //mp3_send_cmd (0x11,1);delay(50);
   //mp3_send_cmd (0x0F,play_list,track_number);delay(50);//folder //file
   //mp3_send_cmd (0x0D);delay(50);
   //random play
   if (triger_stop==0) {
            if (play_list<0)play_list=0;
            mp3_send_cmd (0x17,0,play_list);//loop play folder
            delay(250);
            triger_stop=1;  
            } 
            else
            {
            mp3_send_cmd (0x16);//loop play folder
            delay(250);
            triger_stop=0;  
            }
               break; //music
             }
   
   case 43: {
             mp3_send_cmd (0x16);//loop play folder
            delay(200);
     track_number++;
             track_number=constrain(track_number,0,255);
             mp3_send_cmd (0x0F,play_list,track_number);
             delay(250);
             //mp3_send_cmd (0x0D);
             delay(50);mp3_send_cmd (0x01);//loop play folder
             break;//play track in folder ones
            }
   case 44: {
     mp3_send_cmd (0x16);//loop play folder
            delay(200);
             track_number--;
             track_number=constrain(track_number,0,255);
             mp3_send_cmd (0x0F,play_list,track_number);
             delay(250);
             //mp3_send_cmd (0x0D);
             delay(50);mp3_send_cmd (0x02);//loop play folder
             break;//play track in folder ones
             }
   case 47: { //change sound schema.
         mp3_send_cmd (0x16);//loop play folder
            delay(200);
             sound_shema=constrain(sound_shema++,0,6);
             if (sound_shema>=5) sound_shema=0;
             break;
           }
   case 45: {
     mp3_send_cmd (0x16);//loop play folder
            delay(200);
             if (play_list==-1){
             shift++;
             shift=constrain(shift,0,5);
            break;
           }
           else
           {
           play_list++;
           play_list=constrain(play_list,0,90);
           mp3_send_cmd (0x0F,play_list,track_number);
           
           delay(250);
           //mp3_send_cmd (0x0D);
            mp3_send_cmd (0x17,0,play_list);//loop play folder
            delay(250);
           
          
           } 
           break;//shift++
           }
   case 46: {
     mp3_send_cmd (0x16);//loop play folder
            delay(50);
             if (play_list==-1)
               {
               shift--;
               shift=constrain(shift,0,5);
               break;
               }
             else
               {
               play_list--;
               play_list=constrain(play_list,0,90);
               mp3_send_cmd (0x0F,play_list,track_number);
             // mp3_send_cmd (0x17,0,play_list);
               delay(250);
              // mp3_send_cmd (0x0D);
               mp3_send_cmd (0x17,0,play_list);//loop play folder
              delay(250);
              
               
               }
             break;//shift--
           }
   case 49: {sound_level++;sound_level=constrain(sound_level,0,30);mp3_set_volume (sound_level); delay(50);break;}//sound down
   case 48: {sound_level--;sound_level=constrain(sound_level,0,30);mp3_set_volume (sound_level); delay(50);break;}//sound up
   default:// Serial.println(key);
           {play_list=-1; mp3_play_physical(key+shift*7+100*sound_shema);
   break;}
  }
}

int read_key()
{
for (int i=6;i<11;i++){//11
 analogWrite(input_pwm_line_0,255);
 analogWrite(input_pwm_line_1,255);
 analogWrite(input_pwm_line_2,255);
 analogWrite(input_pwm_line_3,255);
 analogWrite(input_pwm_line_4,255);
 analogWrite(i,0);
 
if (digitalRead(input_key_0)==LOW) return round(i*10+0-58);//shifting of null file. on null dir first file is 001.waw on play command firs wil be 000.waw  
if (digitalRead(input_key_1)==LOW) return round(i*10+1-58);
if (digitalRead(input_key_2)==LOW) return round(i*10+2-58);
if (digitalRead(input_key_3)==LOW) return round(i*10+3-58);
if (digitalRead(input_key_4)==LOW) return round(i*10+4-58);
if (digitalRead(input_key_5)==LOW) return round(i*10+5-58);
if (digitalRead(input_key_6)==LOW) return round(i*10+6-58);
if (digitalRead(input_key_7)==LOW) return round(i*10+7-58);
if (digitalRead(input_key_8)==LOW) return round(i*10+8-58);
if (digitalRead(input_key_9)==LOW) return round(i*10+9-58);

}
return 100;
}
//   digitalWrite(4,HIGH);
//  digitalWrite(5,LOW);
//  digitalWrite(6,LOW);
//  digitalWrite(7,LOW);

