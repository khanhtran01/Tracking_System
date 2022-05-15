#include "main.h"
#include <string.h>
// Noi khai bao hang so
#define     LED     PORTD
#define     ON      1
#define     OFF     0

#define     INIT_SYSTEM        255
#define     ENTER_PASSWORD     1
#define     CHECK_PASSWORD     2
#define     UNLOCK             3
#define     WRONG_PASSWORD     4
#define     CHANGETIME         5 
#define     WARNING            6 
#define     GRE1               7
#define     YEL1               8
#define     GRE2               9
#define     YEL2               10
#define     LOG_OUT            11
#define     TIME_CHANGED       12
#define PORTD_OUT   PORTD
#define TRISD_OUT   TRISD

#define clock_pin   0
#define data_pin    1
#define latch_pin   2

void init_output(void);
// Noi khai bao bien toan cuc
unsigned char arrayMapOfOutput [8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
unsigned char statusOutput[8] = {0,0,0,0,0,0,0,0};
// Khai bao cac ham co ban IO
void init_system(void);
void delay_ms(int value);
void OpenOutput(int index);
void CloseOutput(int index);
void TestOutput(void);
void ReverseOutput(int index);
void Test_KeyMatrix();
//Chuong trinh Password Door
unsigned char arrayMapOfNumber [16] = {1,2,3,'A',4,5,6,'B',
                                       7,8,9,'C','*',0,'E','D'};
unsigned char arrayMapOfPassword [5][4]= {
  {1,2,3,4},
  {2,4,1,2},
  {3,3,3,3},
  {4,8,6,8},
  {5,'A','B','C'},
};
unsigned char arrayPassword[4];
unsigned char statusPassword = INIT_SYSTEM;
unsigned char indexOfNumber = 0;
unsigned char timeDelay = 0;
unsigned char timeDelay2 = 0;
void App_PasswordDoor();
unsigned char CheckPassword();
unsigned char isButtonNumber();
unsigned char numberValue;
unsigned char isButtonEnter();
unsigned char isLogOutEnter();
unsigned char isButton1();
unsigned char isButton2();
unsigned char isButton3();
unsigned char isButton4();
//unsigned char isRA4();
//unsigned char isRA5();
//unsigned char isRE0();
//unsigned char isRE1();
unsigned char isEmer();
unsigned char isSlow();
unsigned char isWeather();
unsigned char isChangedTime();
void UnlockDoor();
void LockDoor();
void setTimeOnline();
//int readTemp();
//int readSound();
// Den giao thong
void Phase1_GreenOn();
void Phase1_YellowOn();
void Phase1_RedOn();
void Phase1_Off();

void Phase2_GreenOn();
void Phase2_YellowOn();
void Phase2_RedOn();
void Phase2_Off();

//#define     INIT_SYSTEM         255
#define     PHASE1_GREEN        0
#define     PHASE1_YELLOW       1
#define     PHASE2_GREEN        2
#define     PHASE2_YELLOW       3
// states for Traffic system
#define     AUTO                0
#define     EDIT                1
#define     RED_WARNING         2
#define     YELLOW_WARNING      3   

unsigned char statusOfLight = INIT_SYSTEM;
unsigned char timeOfGreenPhase1 = 10;
unsigned char timeOfYellowPhase1 = 3;
unsigned char timeOfGreenPhase2 = 10;
unsigned char timeOfYellowPhase2 = 3;
unsigned char timeOfLight1 = 0;
unsigned char timeOfLight2 = 0;
unsigned char cntOfLight = 0;
//NHAN THAY DOO THOI GIAN O DAY
char cmdRec[30] = "";
char cmdRec_length =  0;
char flagNewCmdRec = 0; 

char cmd_SET_TIME[13]="!set-cuTime-";
//-----------------------------------//
void Auto_FSM();
void UartDataReceiveProcess();
void UartDataReceiveProcess_ElectronicDeviceControl();


unsigned int seg(unsigned int num);
void shiftout(int data1, int data2);
//-----------------------------------------------------------------
void Traffic_System();
unsigned char isButtonSystem();
unsigned char state_system = AUTO;
////////////////////////////////////////////////////////////////////
//Hien thuc cac chuong trinh con, ham, module, function duoi cho nay
////////////////////////////////////////////////////////////////////
void main(void)
{
	unsigned int k = 0;
	init_system();
    delay_ms(1000);
	while (1)
	{
            //while (!flag_timer3);
            //flag_timer3 = 0;
            //scan_key_matrix_with_uart(); // 8 button
            DisplayDataReceive();
            //UartDataReceiveProcess();
            uart_isr();
            scan_key_matrix();
            Traffic_System();
            //setTimeOnline();
            //App_PasswordDoor();
            DisplayLcdScreenOld(); //Output process 14ms vs 10ms with no timer       
	}
}
// Hien thuc cac module co ban cua chuong trinh
void delay_ms(int value)
{
	int i,j;
	for(i=0;i<value;i++)
		for(j=0;j<238;j++);
}

void init_output(void)
{
   TRISD_OUT = 0x00;
    PORTD_OUT = 0x00;
}

void init_system(void)
{
    init_output();
    lcd_init();
    init_adc();
    //init_key_matrix_with_uart_i2c();
    init_key_matrix();
    init_interrupt();
    //init_i2c();
    init_uart();
    lcd_clear();
    LcdClearS();
    delay_ms(500);
    init_timer0(4695);//dinh thoi 1ms sai so 1%
    init_timer3(46950);//dinh thoi 10ms
    SetTimer0_ms(2);
    SetTimer3_ms(50); //Chu ky thuc hien viec xu ly input,proccess,output
    //PORTAbits.RA0 = 1;
    TRISAbits.RA3 = 0; //den xanh 2
    //PORTAbits.RA3 = 0;
    TRISAbits.RA6 = 0; //den do 
    PORTAbits.RA6 = 0;
    TRISAbits.RA1 = 1; //nhan tin hieu Emergency - INPUT
   // PORTAbits.RA1 = 0;
    TRISAbits.RA2 = 1;//time changed
    PORTAbits.RA2 = 0;
    // For ESP8266
    TRISAbits.RA4 = 1; //nhan tin hieu Slow - INPUT
    //PORTAbits.RA4 = 0;
    TRISAbits.RA5 = 1;//nhan tin hieu thoi tiet - INPUT
    PORTAbits.RA5 = 0;
    //TRISEbits.RE0 = 1; //nhan biet co change Time - INPUT

    
    Phase1_Off();
    Phase2_Off();
}

void OpenOutput(int index)
{
	if (index >= 0 && index <= 7)
	{
		PORTD_OUT = PORTD_OUT | arrayMapOfOutput[index];
	}


}

void CloseOutput(int index)
{
	if (index >= 0 && index <= 7)
	{
		PORTD_OUT = PORTD_OUT & ~arrayMapOfOutput[index];
	}
}

void ReverseOutput(int index)
{
    if (statusOutput[index]  == ON)
    {
        CloseOutput(index);
        statusOutput[index] = OFF;
    }
    else
    {
        OpenOutput(index);
        statusOutput[index] = ON;
    }
}

void TestOutput(void)
{
	int k;
	for (k=0;k<14 ;k++ )
	{
		OpenOutput(k);
		delay_ms(500);
		CloseOutput(k);
		delay_ms(500);
	}
}

//------------------------------------------------ TRAFFIC SYSTEM
void Traffic_System() 
{
    DisplayDataReceive();
    uart_isr_simulate_machine();
   // int timeDelay = 0;
//    int currTemp;
//    currTemp = readTemp()*0.488;
    switch (state_system)
    {
        case AUTO:
            Auto_FSM();
            if(isButtonSystem()) //Press *
            {
                state_system = EDIT;
                
            }
            if(isEmer()==1)
            {
                state_system = RED_WARNING;
            }
            if(isSlow()==1)
            {
                state_system = YELLOW_WARNING;
            }
            if(isChangedTime()){
                state_system = TIME_CHANGED;
                //setTimeOnline();
            }
            break;
        case EDIT:
            App_PasswordDoor();
            if(isButtonSystem())
            {
                state_system = AUTO;
            }
            break;
        case RED_WARNING:
            Phase1_RedOn();
            Phase2_RedOn();
           
            LcdPrintStringS(0,0,"  EMERGENCY     ");   
            LcdPrintStringS(1,0,"  SITUATION!    ");
            if(isEmer()== 0)
            {
                state_system = AUTO;
            }
            break;
        case YELLOW_WARNING:
            Phase1_YellowOn();
            Phase2_YellowOn();
           
            LcdPrintStringS(0,0,"SPEED LIMIT:    ");   
            LcdPrintStringS(1,0,"           5KM/H");
            if(isSlow()==0)
            {
                state_system = AUTO;
            }
            break;
        case TIME_CHANGED:
            setTimeOnline();
            LcdPrintStringS(0,0,"TIME UPDATED        ");   
            LcdPrintStringS(1,0,"SUCCESSFULLY!       ");
            timeDelay2++;
            if(timeDelay2 >= 250){
                state_system = AUTO;
            }
            break;
        default:
            state_system = AUTO;
            break;
    }
}

void App_PasswordDoor()
{
//    int currTemp;
//    currTemp = (readTemp()/10)*0.488;
    //LcdPrintNum(1,0,currTemp);
    
    switch (statusPassword)
    {
        case INIT_SYSTEM:
            LcdPrintStringS(0,0,"PRESS B FOR PASS");   
            //PORTAbits.RA3 = 1; //cua dang khoa
            //PORTAbits.RA4 = 0;
            //LcdPrintStringS(1,0,"                ");
            //LcdPrintNum(1,0,currTemp);
            LockDoor();
            if (isButtonEnter())
            {
                indexOfNumber = 0;
                timeDelay = 0;
                statusPassword = ENTER_PASSWORD;
            }
            //if(PORTAbits.RA5 == 1) PORTAbits.RA1 = 1;
            //else PORTAbits.RA1 = 0;
            //currTemp = (readTemp()/10)*0.488;
            //if(PORTEbits.RE0 == 1) statusPassword = UNLOCK;
            //UartSendNum((long)currTemp);
            break;
        case ENTER_PASSWORD:
            LcdPrintStringS(0,0,"ENTER PASSWORD  ");
           // PORTAbits.RA3 = 1;
            //PORTAbits.RA4 = 0;
            timeDelay ++;
            if (isButtonNumber())
            {
                LcdPrintStringS(1,indexOfNumber,"*");
                arrayPassword [indexOfNumber] = numberValue;
                indexOfNumber ++;
                timeDelay = 0;
            }
            if (indexOfNumber >= 4)
                statusPassword = CHECK_PASSWORD;
            if (timeDelay >= 100)
                statusPassword = INIT_SYSTEM;
            if (isButtonEnter()) //Press # //press B
                statusPassword = INIT_SYSTEM;
            if(isLogOutEnter()){
                statusPassword = LOG_OUT;
            }
            //if(PORTAbits.RA5 == 1) PORTAbits.RA1 = 1;
            //else PORTAbits.RA1 = 0;
            //currTemp = (readTemp()/10)*0.488;
            //UartSendNum((long)currTemp);
            break;
        case CHECK_PASSWORD:
            timeDelay = 0;
            //PORTAbits.RA3 = 1;
            //PORTAbits.RA4 = 0;
            if (CheckPassword())
                statusPassword = UNLOCK;
            else
                statusPassword = WRONG_PASSWORD;
            
            //if(PORTAbits.RA5 == 1) PORTAbits.RA1 = 1;
            //else PORTAbits.RA1 = 0;
            
            //currTemp = (readTemp()/10)*0.488;
            //UartSendNum((long)currTemp);
            break;
        case UNLOCK:
            timeDelay ++;
            //PORTAbits.RA3 = 0;
            //PORTAbits.RA4 = 1;//cua dang mo
            LcdPrintStringS(0,0,"1.CHANGE TIME         ");
            LcdPrintStringS(1,0,"2.WARNING            ");
            if(isButton1()) statusPassword = CHANGETIME;
            if(isButton2()) statusPassword = WARNING;
//            UnlockDoor();
           if (timeDelay >= 400)
               statusPassword = INIT_SYSTEM;
            if(isLogOutEnter()){
                statusPassword = LOG_OUT;
                LcdPrintStringS(0,0,"                ");
                 LcdPrintStringS(1,0,"               ");
            }
            //if(PORTEbits.RE0 == 0) statusPassword = INIT_SYSTEM;
            
            //if(PORTAbits.RA5 == 1) PORTAbits.RA1 = 1;
            //else PORTAbits.RA1 = 0;
            //currTemp = (readTemp()/10)*0.488;
            //UartSendNum((long)currTemp);
            break;
        case CHANGETIME:
            LcdPrintStringS(0,0,"1.GRE1    2.GRE2");
            LcdPrintStringS(1,0,"3.YEL1    4.YEL2");
            if(isButton1()) statusPassword = GRE1;
            if(isButton2()) statusPassword = GRE2;
            if(isButton3()) statusPassword = YEL1;
            if(isButton4()) statusPassword = YEL2;
            if(isButtonEnter()) statusPassword = UNLOCK;
            if(isLogOutEnter()){
                statusPassword = LOG_OUT;
                LcdPrintStringS(0,0,"                ");
                 LcdPrintStringS(1,0,"               ");
            }
            
            break;
        case WARNING:
            LcdPrintStringS(0,0,"  WARNING!      ");
            LcdPrintStringS(1,0,"DANGER!!!          ");
            Phase2_RedOn();
            Phase1_RedOn();
            if(isButtonEnter()) statusPassword = UNLOCK;
            if(isLogOutEnter()){
                statusPassword = LOG_OUT;
                LcdPrintStringS(0,0,"                ");
                 LcdPrintStringS(1,0,"               ");
            }
           
            break;
        case GRE1:
            LcdPrintStringS(0,0,"TIME_GREEN_1:   ");
            LcdPrintNumS(0,14,timeOfGreenPhase1);
            LcdPrintStringS(1,0," 1.+        2.- ");
            if(isButton1()) timeOfGreenPhase1++;
            if(isButton2()) timeOfGreenPhase1--;
            if(isButtonEnter()) statusPassword = CHANGETIME;
            if(isLogOutEnter()){
                statusPassword = LOG_OUT;
                 LcdPrintStringS(0,0,"                   ");
                 LcdPrintStringS(1,0,"               ");
            }
            //PORTAbits.RA3 = 0;
            break;
        case YEL1:
            LcdPrintStringS(0,0,"TIME_YELLOW_1:  ");
            LcdPrintNumS(0,14,timeOfYellowPhase1);     
            LcdPrintStringS(1,0," 1.+        2.- ");
            if(isButton1()) timeOfYellowPhase1++;
            if(isButton2()) timeOfYellowPhase1--;
            if(isButtonEnter()) statusPassword = CHANGETIME;
            if(isLogOutEnter()){
                statusPassword = LOG_OUT;
                LcdPrintStringS(0,0,"                    ");
                 LcdPrintStringS(1,0,"               ");
            }
           //PORTAbits.RA3 = 0;
            break;
        case GRE2:
            LcdPrintStringS(0,0,"TIME_GREEN_2:   ");
            LcdPrintNumS(0,14,timeOfGreenPhase2);          
            LcdPrintStringS(1,0," 1.+        2.- ");
            if(isButton1()) timeOfGreenPhase2++;
            if(isButton2()) timeOfGreenPhase2--;
            if(isButtonEnter()) statusPassword = CHANGETIME;
            if(isLogOutEnter()){
                statusPassword = LOG_OUT;
                LcdPrintStringS(0,0,"                ");
                 LcdPrintStringS(1,0,"               ");
            }
           // PORTAbits.RA3 = 1;
           
            break;
        case YEL2:
            LcdPrintStringS(0,0,"TIME_YELLOW_2:  ");
            LcdPrintNumS(0,14,timeOfYellowPhase2);             
            LcdPrintStringS(1,0," 1.+        2.- ");
            if(isButton1()) timeOfYellowPhase2++;
            if(isButton2()) timeOfYellowPhase2--;
            if(isButtonEnter()) statusPassword = CHANGETIME;
            if(isLogOutEnter()){
                statusPassword = LOG_OUT;
                LcdPrintStringS(0,0,"                ");
                 LcdPrintStringS(1,0,"               ");
            }
            PORTAbits.RA3 = 0;
            
            break;
        case WRONG_PASSWORD:
            timeDelay ++;
            LcdPrintStringS(0,0,"PASSWORD WRONG  ");
            LcdPrintStringS(1,0,"                ");
            if (timeDelay >= 200)
                statusPassword = INIT_SYSTEM;
            break;
        case LOG_OUT:
            LcdPrintStringS(0,0,"LOG OUT ?          ");
            LcdPrintStringS(1,0,"1.YES     2.NO");
            if(isButton1()){
                statusPassword = INIT_SYSTEM;
            }
            if(isButton2()){
                statusPassword = UNLOCK;
            }
           
            break;
        default:
           
            break;

    }
}


unsigned char CheckPassword()
{
    unsigned char i,j;
    unsigned result = 1;
    for (i=0;i<5;i++)
    {
        result = 1;
        for (j=0;j<4;j++)
        {
            if (arrayPassword[j] != arrayMapOfPassword[i][j])
                result = 0;
        }
        if (result == 1)
            return (i+1);
    }

}
unsigned char isButtonNumber()
{
    unsigned char i;
    for (i = 0; i<=15; i++)
        if (key_code[i] == 1)
        {
            numberValue = arrayMapOfNumber[i];
            return 1;
        }
    return 0;
}
unsigned char isLogOutEnter(){
    //if(key_code[3] == 1){ //button A
    if(key_code[6] == 1){ //press 6
        return 1;
    }else{
        return 0;
    }
}
unsigned char isButtonEnter()
{
    //if (key_code[14] == 1) // press #
    if (key_code[7] == 1) //press B
        return 1;
    else
        return 0;
}

unsigned char isButtonSystem() //Press *
{
    //if (key_code[12] == 1)
    if (key_code[3] == 1) //press A
        return 1;
    else
        return 0;
}

unsigned char isButton1()
{
    if (key_code[0] == 1)
        return 1;
    else
        return 0;
}

unsigned char isButton2()
{
    if (key_code[1] == 1)
        return 1;
    else
        return 0;
}

unsigned char isButton3()
{
    if (key_code[2] == 1)
        return 1;
    else
        return 0;
}

unsigned char isButton4()
{
    //if (key_code[3] == 1)
    if (key_code[4] == 1)
        return 1;
    else
        return 0;
}

void UnlockDoor()
{
    OpenOutput(0);
}
void LockDoor()
{
    CloseOutput(0);
}

void Phase1_GreenOn()
{
//    CloseOutput(3);
//    OpenOutput(4);
    CloseOutput(3);
CloseOutput(4);
    OpenOutput(5);
    //PORTAbits.RA3 = 0;
    //PORTAbits.RA6 = 0;
}

void Phase1_YellowOn()
{
//    OpenOutput(3);
//    CloseOutput(4);
      CloseOutput(3);
CloseOutput(5);
    OpenOutput(4);
    //PORTAbits.RA3 = 0;
    //PORTAbits.RA6 = 0;
}
void Phase1_RedOn()
{
//    CloseOutput(3);
//    CloseOutput(4);
     CloseOutput(4);
    CloseOutput(5);
    OpenOutput(3);
    //PORTAbits.RA3 = 0;
   // PORTAbits.RA6 = 0;
}
void Phase1_Off()
{
//    OpenOutput(3);
//    OpenOutput(4);
     CloseOutput(3);
    CloseOutput(5);
    CloseOutput(4);
    //PORTAbits.RA6 = 0;
}

void Phase2_GreenOn()
{
//    CloseOutput(5);
//    OpenOutput(6);
         CloseOutput(6);
    CloseOutput(7);
    //PORTAbits.RA3 = 1;
    //PORTAbits.RA6 = 0;
}

void Phase2_YellowOn()
{
//    OpenOutput(5);
//    CloseOutput(6);
CloseOutput(6);
    //PORTAbits.RA3 = 0;
    //PORTAbits.RA6 = 0;
    OpenOutput(7);    
}

void Phase2_RedOn()
{
//    CloseOutput(5);
//    CloseOutput(6);
     CloseOutput(7);
    //PORTAbits.RA3 = 0;
    //PORTAbits.RA6 = 1;
    OpenOutput(6);
    //PORTAbits.RA6 = 0;
}
void Phase2_Off()
{
//    OpenOutput(5);
//    OpenOutput(6);
      CloseOutput(6);
    CloseOutput(7);
   //PORTAbits.RA3 = 0;
   //PORTAbits.RA6 = 0;
}
//int temp = get_adc_value();



void Auto_FSM()
{
    //int currTemp;
    //char* nothing;
    //if(UartGetString(nothing,'0')){
     //     UartSendString(nothing);
    //}
    //char* temperature;
    //currTemp = readTemp()/10;
    //currTemp = 34;
    //currTemp = readTemp()*0.488;
    if(isButtonSystem())
    {
        state_system = EDIT;
        //return;
    }
    cntOfLight = (cntOfLight + 1)%20;
    if (cntOfLight == 0)
    {
        timeOfLight1--;
        timeOfLight2--;
    }  
//    if(isChangedTime())
//    {
////        timeOfGreenPhase1 = 20;
////        timeOfYellowPhase1 = 6;
////        timeOfGreenPhase2 = 10;
////        timeOfYellowPhase2 = 3;
//        LcdPrintStringS(0,0,"TIME1 APPLIED");
//        LcdPrintStringS(1,0,"                    ");
//        //delay_ms(2000);
//    }
    
//    else
//    {
//        timeOfGreenPhase1 = 10;
//        timeOfYellowPhase1 = 3;
//        timeOfGreenPhase2 = 10;
//        timeOfYellowPhase2 = 3;
//        
//    }
    
    shiftout(timeOfLight1,timeOfLight2);
    
    switch (statusOfLight)
    {
        case PHASE1_GREEN:
            Phase1_GreenOn();
            Phase2_RedOn();
            
                LcdPrintStringS(0,0,"PHASE1_GREEN:   ");
                LcdPrintNumS(0,14,timeOfLight1);
                LcdPrintStringS(1,0,"PHASE2_RED  :   ");
                
                LcdPrintNumS(1,14,timeOfLight2);
            
            if (timeOfLight1 == 0)
            {
                statusOfLight = PHASE1_YELLOW;
                PORTAbits.RA6 = 1;
                timeOfLight1 = timeOfYellowPhase1;
                timeOfLight2 = timeOfYellowPhase1;
            }
            PORTAbits.RA3 = 0;
            break;
        case PHASE1_YELLOW:
            Phase1_YellowOn();
            Phase2_RedOn();
            
                LcdPrintStringS(0,0,"PHASE1_YELLOW:  ");
                LcdPrintNumS(0,14,timeOfLight1);
                LcdPrintStringS(1,0,"PHASE2_RED   :  ");
                LcdPrintNumS(1,14,timeOfLight2);
                //PORTAbits.RA6 = 1;
            
            if (timeOfLight1 == 0)
            {
                statusOfLight = PHASE2_GREEN;
                PORTAbits.RA6 = 0;
                timeOfLight1 = timeOfGreenPhase2 + timeOfYellowPhase2;
                timeOfLight2 = timeOfGreenPhase2;
            }
            PORTAbits.RA3 = 0;
            break;
        case PHASE2_GREEN:
            Phase2_GreenOn();
            Phase1_RedOn();
           
                LcdPrintStringS(0,0,"PHASE1_RED  :   ");
                LcdPrintNumS(0,14,timeOfLight1);
                LcdPrintStringS(1,0,"PHASE2_GREEN:   ");
                LcdPrintNumS(1,14,timeOfLight2);
               //PORTAbits.RA6 = 0;
            if (timeOfLight2 == 0)
            {
                statusOfLight = PHASE2_YELLOW;
                PORTAbits.RA6 = 0;
                timeOfLight2 = timeOfYellowPhase2;
                timeOfLight1 = timeOfYellowPhase2;
               // PORTEbits.RE2 = 0;
            }
           PORTAbits.RA3 = 1;
            break;
        case PHASE2_YELLOW:
            Phase2_YellowOn();
            Phase1_RedOn();
            
                LcdPrintStringS(0,0,"PHASE1_RED   :  ");
                LcdPrintNumS(0,14,timeOfLight1);
                LcdPrintStringS(1,0,"PHASE2_YELLOW:  ");
                LcdPrintNumS(1,14,timeOfLight2);
               //PORTAbits.RA6 = 0;
            if (timeOfLight2 == 0)
            {
                statusOfLight = PHASE1_GREEN;
                PORTAbits.RA6 = 1;
                timeOfLight1 = timeOfGreenPhase1;
                timeOfLight2 = timeOfGreenPhase1 + timeOfYellowPhase1;
            }
                PORTAbits.RA3 = 0;
            break;
        default:
            statusOfLight = PHASE1_GREEN;
           
            break;
    }
}

//void UartDataReceiveProcess()
//{
//    if(flagOfDataReceiveComplete == 1)
//    {
//        flagOfDataReceiveComplete = 0;
//        if (dataReceive[0] == 0)
//        {
//            timeOfGreenPhase1 = dataReceive[1];
//            timeOfYellowPhase1 = dataReceive[2];
//            timeOfGreenPhase2 = dataReceive[3];
//            timeOfYellowPhase2 = dataReceive[4];
//        }
//    }
//    LcdPrintNumS(0,15,statusReceive);
//}

unsigned int seg(unsigned int num) {
  switch (num) {
    case 0 : return 0xc0;
    case 1 : return 0xf9; 
    case 2 : return 0xa4;
    case 3 : return 0xb0;
    case 4 : return 0x99;
    case 5 : return 0x92;
    case 6 : return 0x82;
    case 7 : return 0xF8;
    case 8 : return 0x80;
    case 9 : return 0x90;
    }
}

void shiftout(int data1, int data2)
{
    unsigned int led0 = seg(data1/10);
    unsigned int led1 = seg(data1%10);
    unsigned int led2 = seg(data2/10);
    unsigned int led3 = seg(data2%10);
    unsigned int order = 0x01;
    unsigned int j;
    unsigned int number;
    unsigned char i;
    for(i=0; i<4; i++)
    {
        number = order;
        for(j = 0x80; j > 0; j = j >> 1) 
        {
            if(number & j){
                OpenOutput(data_pin); // xung cao
            }
            else
            {
                CloseOutput(data_pin); //xung thap
            }
            /*tao xung clock day data*/
            OpenOutput(clock_pin);
            CloseOutput(clock_pin);
        }
        /*tao xung ket thuc day data*/
        OpenOutput(latch_pin);
        CloseOutput(latch_pin);
        //delay_ms(100);
        if(i==0) number=led0;
        if(i==1) number=led1;
        if(i==2) number=led2;
        if(i==3) number=led3;
        for(j = 0x80; j > 0; j = j >> 1) 
        {
            if(number & j){
                OpenOutput(data_pin); // xung cao
            }
            else
            {
                CloseOutput(data_pin); //xung thap
            }
            /*tao xung clock day data*/
            OpenOutput(clock_pin);
            CloseOutput(clock_pin);
        }
        /*tao xung ket thuc day data*/
        OpenOutput(latch_pin);
        CloseOutput(latch_pin);
        order = order << 1;
        delay_ms(10);
    }
}

void setTimeOnline(){
    flagNewCmdRec = UartGetString(cmdRec,&cmdRec_length);
    if(flagNewCmdRec == 1)
    {
        flagNewCmdRec = 0 ; 
        //LcdPrintStringS(1,0,"ok!!!             ");
        // !set-cuTime-[40,5,35,5]!
            //if(str_cmp(cmdRec,cmd_SET_TIME,12) == 0)
            //{
//                UartSendString("!OK!\n");
//                UartSendString("!");
//                UartSendString("OK");
//                UartSendString("!");
//                UartSendString("\n");
                timeOfGreenPhase1 = (int)(cmdRec[13]-'0')*10+(cmdRec[14]-'0');
                timeOfYellowPhase1 = ((int)cmdRec[16]-'0');
                timeOfGreenPhase2 = (int)(cmdRec[18]-'0')*10+(cmdRec[19]-'0');
                timeOfYellowPhase2 = (int)(cmdRec[21]-'0');
                
            //}
    }
    return;
}
//TRISAbits.RA3 = 0; //thong bao la dang co den do - OUTPUT
//    PORTAbits.RA3 = 0;
//    TRISAbits.RA1 = 1; //nhan tin hieu Emergency - INPUT
//
//    // For ESP8266
//    TRISAbits.RA4 = 1; //nhan tin hieu Slow - INPUT
//    TRISAbits.RA5 = 1;//nhan tin hieu thoi tiet - INPUT
//    TRISEbits.RE0 = 1; //nhan biet co change Time - INPUT
unsigned char isEmer(){
    if(PORTAbits.RA1 == 1){
        return 1;
    }
    return 0;
}
unsigned char isSlow(){
    if(PORTAbits.RA4 == 1){
        return 1;
    }
    return 0;
}
unsigned char isWeather(){
    if(PORTAbits.RA5 == 1){
        return 1;
    }
    return 0;
}
unsigned char isChangedTime(){
    if(PORTAbits.RA2 == 1){
        return 1;
    }
    return 0;
}