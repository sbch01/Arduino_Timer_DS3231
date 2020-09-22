/*
Start Date: 29.08.2020
Source Web: https://howtomechatronics.com/tutorials/arduino/arduino-ds3231-real-time-clock-tutorial
DS3231 Library Document Web: http://www.rinkydinkelectronics.com/library.php?id=73
*/

/*
* Arduino DS3231 Real Time Clock Module Tutorial
*
* Crated by Dejan Nedelkovski,
* www.HowToMechatronics.com
*
* DS3231 Library made by Henning Karlsen which can be found and downloaded from his website, www.rinkydinkelectronics.com.
*
*/
#include <DS3231.h>
#include <LiquidCrystal.h> // includes the LiquidCrystal Library 
#include <WeekTimer.h> //My custom library for week timer functions

//Define custom name of Buttons
#define DOWN_BUTTON 10
#define UP_BUTTON 9
#define SELECT_BUTTON 8

//Define custom name of Outputs
#define TMR1_OUT 14 //A0 pin
#define TMR2_OUT 15 //A1 pin

//Define constant for logic control
#define ON 1
#define OFF 0

/*************************Inicialazing Objects ************************/
  Time arduTime; //Create object arduTime wich save time in arduino
  DS3231  dsTime(SDA, SCL); //Create object wich 
  //Parameters: (rs, enable, d4, d5, d6, d7) 
  LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Creates an LCD object. 
  WeekTimer WT[3][4];
 /*********************************************************************/
  

  /****************** Global Variables ********************************/
  //
  enum CursAddr {Home = 0, HOUR_ADR, MINUTES_ADR, SECONDS_ADR, 
                 DAY_ADR, MONTH_ADR, YEAR_ADR, DOW_ADR, MON_ADR,
                 TUE_ADR, WED_ADR, THU_ADR, FRI_ADR, SAT_ADR, SUN_ADR,
                 ONH_ADR, ONM_ADR, OFFH_ADR, OFFM_ADR} CursPlace = Home;
  //               
  enum Display {MAIN_DISP = 0, TMR1_L1, TMR1_L2, TMR1_L3, TMR2_L1, TMR2_L2, 
                                            TMR2_L3} DisplayPage = MAIN_DISP;
  
  /********************************************************************/

  



void setup() { 

  /************************Initial Timers**********************/
  WT[0][0].SetWeekPlanStr("MTWTFSS");//TMR1 lyer 1 
  WT[0][0].SetOnTime(18,30);
  WT[0][0].SetOffTime(22,00);
  
  WT[0][1].SetWeekPlanStr("-----SS");//TMR1 lyer 2 
  WT[0][1].SetOnTime(12,30);
  WT[0][1].SetOffTime(14,35);

  WT[0][2].SetWeekPlanStr("-------");//TMR1 lyer 3 
  WT[0][2].SetOnTime(12,30);
  WT[0][2].SetOffTime(14,35);

  WT[1][0].SetWeekPlanStr("MTWTFSS");//TMR2 lyer 1 
  WT[1][0].SetOnTime(20,00);
  WT[1][0].SetOffTime(22,30);
  
  WT[1][1].SetWeekPlanStr("-------");//TMR2 lyer 2 
  WT[1][1].SetOnTime(11,00);
  WT[1][1].SetOffTime(12,45);

  WT[1][2].SetWeekPlanStr("-------");//TMR2 lyer 3 
  WT[1][2].SetOnTime(20,00);
  WT[1][2].SetOffTime(22,30);
  
  /********************** EOF Initial Timers ******************/

   
  CursPlace = Home; //Set Cursor in home position
  
  /*****Config Pins and define custom names of pins *****/
  //Inputs
  pinMode(DOWN_BUTTON, INPUT_PULLUP);
  pinMode(UP_BUTTON, INPUT_PULLUP);
  pinMode(SELECT_BUTTON, INPUT_PULLUP);
  
  //Outputs
  pinMode(TMR1_OUT, OUTPUT);
  pinMode(TMR2_OUT, OUTPUT);
  /******************************************************/
  
  dsTime.begin(); // Initialize the dsTime object
  lcd.begin(20,4); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display } 

  // Aproximate 8 seconds to upload scatch
  // The following lines can be uncommented to set the date and time
  //dsTime.setDOW(SUNDAY);     // Set Day-of-Week to SUNDAY
  //dsTime.setTime(20, 4, 0);     // Set the time to 12:00:00 (24hr format)
  //dsTime.setDate(29, 8, 2020);   // Set the date to January 1st, 2014

}


void loop() { 
  
  //SELECT BUTTON REACTION
  //===========================================================================================================
    if (!digitalRead(SELECT_BUTTON)){

      if(DisplayPage == MAIN_DISP){ 
          CursPlace = CursPlace + 1;
          while(!digitalRead(SELECT_BUTTON));
           
            if(CursPlace > DOW_ADR){ //Write all arduTime Data and send it to DS3231
                CursPlace = Home;
                dsTime.setTime(arduTime.hour, arduTime.min, arduTime.sec); 
                dsTime.setDate(arduTime.date, arduTime.mon, arduTime.year);
                dsTime.setDOW();  //This way automaticly correct Day of week            
            }
      }
      
      else{ 
          if(CursPlace < DOW_ADR) CursPlace = DOW_ADR;
          CursPlace = CursPlace + 1;
          while(!digitalRead(SELECT_BUTTON));
            
            if(CursPlace > OFFM_ADR){ //Write Configured Settings in EEPROM
                CursPlace = Home;
            
            }
      }
  }

  //UP BUTTON REACTION
  //===========================================================================================================
  if (!digitalRead(UP_BUTTON)){

      if(CursPlace == Home){
        DisplayPage = DisplayPage + 1;  
        if(DisplayPage > TMR2_L3) DisplayPage = MAIN_DISP;
        while(!digitalRead(UP_BUTTON));
        lcd.clear();
      }
         
      if (DisplayPage == MAIN_DISP) {
             
         if(CursPlace == YEAR_ADR){//Curssor is pointed on the Month
         arduTime.year++;//Incrase day of month
         if(arduTime.year > 2099)arduTime.year = 2000;
         }
         //-----------------------------------------------------
         if(CursPlace == MONTH_ADR){//Curssor is pointed on the Month
         arduTime.mon++;//Incrase day of month
         if(arduTime.mon > 12)arduTime.mon = 0; //HERE WE NEED MORE VERIFICATIONS!!!
         }
         //-----------------------------------------------------
         if(CursPlace == DAY_ADR){//Curssor is pointed on the Day
         arduTime.date++;//Incrase day of month
         if(arduTime.date > 31)arduTime.date = 0; //HERE WE NEED MORE VERIFICATIONS!!!
         }
         //-----------------------------------------------------
          if(CursPlace == HOUR_ADR){ //Curssor is pointed to Hour
          arduTime.hour++; //Incrase Hour
          if(arduTime.hour > 23)arduTime.hour = 0;//Check If Hour is reached 23 and turn back to 0 
         }
         //-----------------------------------------------------

         if(CursPlace == MINUTES_ADR){ //Curssor is pointed to Minutes
          arduTime.min++; //Incrase Minutes
          if(arduTime.min > 59)arduTime.min = 0;//Check If Minutes is reached 59 and turn back to 0 
         }
         //-----------------------------------------------------

         if(CursPlace == SECONDS_ADR){ //Curssor is pointed to Seconds
          arduTime.sec++; //Incrase Minutes
          if(arduTime.sec > 59)arduTime.sec = 0;//Check If Seconds is reached 59 and turn back to 0 
         }
         //-----------------------------------------------------
    
        if(CursPlace == DOW_ADR){ //Curssor is pointed to Day Of Week
          arduTime.dow++; //Incrase Day Of Week
          if(arduTime.dow > SUNDAY)arduTime.dow = MONDAY;//Check Day If is SunDay turn Back to Monday
          dsTime.setDOW(arduTime.dow);//Write to back to DS3231          
         }
         //-----------------------------------------------------

       while(!digitalRead(UP_BUTTON));
       MainDisplay();

      }
      if (DisplayPage == TMR1_L1) {
       WeekT_Display(1,1); 
      }
      if (DisplayPage == TMR1_L2) {
       WeekT_Display(1,2); 
      }
      if (DisplayPage == TMR1_L3) {
         WeekT_Display(1,3); 
      }
      if (DisplayPage == TMR2_L1) {
         WeekT_Display(2,1); 
      }
      if (DisplayPage == TMR2_L2) {
         WeekT_Display(2,2); 
      }
      if (DisplayPage == TMR2_L3) {
         WeekT_Display(2,3); 
      }
  }

  //DOWN BUTTON REACTION
  //===========================================================================================================
  if (!digitalRead(DOWN_BUTTON)){
     
     if(CursPlace == Home){
        if(DisplayPage == MAIN_DISP) DisplayPage = TMR2_L3;
        else DisplayPage = DisplayPage - 1;
        while(!digitalRead(DOWN_BUTTON));
        lcd.clear();
      }
    if (DisplayPage == MAIN_DISP) {
      
         if(CursPlace == YEAR_ADR){ //Curssor is pointed to Year
          if(arduTime.year == 2000) arduTime.year = 2099;//Check If Year is reached 2000 and turn back to 2099 
          else arduTime.year--;//Decrase Month
         }
         //-----------------------------------------------------
         if(CursPlace == MONTH_ADR){ //Curssor is pointed to Month
          if(!arduTime.mon) arduTime.mon = 12;//Check If Month is reached 0 and turn back to 12 
          else arduTime.mon--;//Decrase Month
         }//HERE WE NEED MORE VERIFICATIONS!!!
         //-----------------------------------------------------
         if(CursPlace == DAY_ADR){ //Curssor is pointed to Day
          if(!arduTime.date) arduTime.date = 31;//Check If Day is reached 0 and turn back to 31 
          else arduTime.date--;//Decrase Day
         }//HERE WE NEED MORE VERIFICATIONS!!!
         //-----------------------------------------------------
         if(CursPlace == HOUR_ADR){ //Curssor is pointed to Hour
          if(!arduTime.hour) arduTime.hour = 23;//Check If Hour is reached 0 and turn back to 23 
          else arduTime.hour--;//Decrase Hour
         }
         //-----------------------------------------------------
         if(CursPlace == MINUTES_ADR){ //Curssor is pointed to Minutes
          if(!arduTime.min)arduTime.min = 59;//Check If Minutes is reached 0 and turn back to 59 
          else arduTime.min--; //Decrase Minutes            
         }
         //-----------------------------------------------------
         if(CursPlace == SECONDS_ADR){ //Curssor is pointed to Seconds
          if(!arduTime.sec)arduTime.sec = 59;//Check If Seconds is reached 0 and turn back to 59 
          else arduTime.sec--; //Decrase Seconds      
         }
         //-----------------------------------------------------
         if(CursPlace == DOW_ADR){ //Curssor is pointed to Day Of Week
          arduTime.dow--; //Decrase Day Of Week
          if(arduTime.dow < MONDAY)arduTime.dow = SUNDAY; //Check Day If is Monday turn back to SunDay 
          dsTime.setDOW(arduTime.dow);//Write to back to DS3231          
         }
         //------------------------------------------------------
         
       while(!digitalRead(DOWN_BUTTON)); //Wait for relase the button
       delay(20);//Debaunce relase
       MainDisplay();
     }
    if (DisplayPage == TMR1_L1) {
       WeekT_Display(1,1); 
    }
    if (DisplayPage == TMR1_L2) {
       WeekT_Display(1,2); 
    }
    if (DisplayPage == TMR1_L3) {
       WeekT_Display(1,3); 
    }
    if (DisplayPage == TMR2_L1) {
       WeekT_Display(2,1); 
    }
    if (DisplayPage == TMR2_L2) {
       WeekT_Display(2,2); 
    }
    if (DisplayPage == TMR2_L3) {
       WeekT_Display(2,3); 
    }
   }
 
  
      
       //Turn Off curssor when rotate all parameters
       if(CursPlace == Home){
        lcd.noBlink();
        if(DisplayPage == MAIN_DISP) MainDisplay(); //Update Main display      
       }
       
       else{
       lcd.blink();
       }
       
       //Place Curssor on the Time
       //--------------------------------------------
       if(CursPlace == HOUR_ADR){
        lcd.setCursor(7,0);
       }
       if(CursPlace == MINUTES_ADR){
        lcd.setCursor(10,0);
       }
       if(CursPlace == SECONDS_ADR){
        lcd.setCursor(13,0);
       }

       //Place Curssor on the Date
       //--------------------------------------------
       if(CursPlace == DAY_ADR){
        lcd.setCursor(7,1);
       }
       if(CursPlace == MONTH_ADR){
        lcd.setCursor(10,1);
       }
       if(CursPlace == YEAR_ADR){
        lcd.setCursor(15,1);
       }

       //Place Curssor on the Day Of Week
       //-------------------------------------------
       if(CursPlace == DOW_ADR){
        lcd.setCursor(6,2);
       }

       //Place Curssor on the Monday
       //-------------------------------------------
       if(CursPlace == MON_ADR){
        lcd.setCursor(6,1);
       }

       //Place Curssor on the Tuesday
       //-------------------------------------------
       if(CursPlace == TUE_ADR){
        lcd.setCursor(7,1);
       }

       //Place Curssor on the Wednesday
       //-------------------------------------------
       if(CursPlace == WED_ADR){
        lcd.setCursor(8,1);
       }

       //Place Curssor on the Thursday
       //-------------------------------------------
       if(CursPlace == THU_ADR){
        lcd.setCursor(9,1);
       }

       //Place Curssor on the Friday
       //-------------------------------------------
       if(CursPlace == FRI_ADR){
        lcd.setCursor(10,1);
       }

       //Place Curssor on the Saturday
       //-------------------------------------------
       if(CursPlace == SAT_ADR){
        lcd.setCursor(11,1);
       }

       //Place Curssor on the Saturday
       //-------------------------------------------
       if(CursPlace == SUN_ADR){
        lcd.setCursor(12,1);
       }

       //Place Curssor on the On Hour parameter
       //-------------------------------------------
       if(CursPlace == ONH_ADR){
        lcd.setCursor(6,2);
       }

       //Place Curssor on the On Minutes parameter
       //-------------------------------------------
       if(CursPlace == ONM_ADR){
        lcd.setCursor(9,2);
       }

       //Place Curssor on the Off Hour parameter
       //-------------------------------------------
       if(CursPlace == OFFH_ADR){
        lcd.setCursor(6,3);
       }

       //Place Curssor on the Off Minutes parameter
       //-------------------------------------------
       if(CursPlace == OFFM_ADR){
        lcd.setCursor(9,3);
       }
       
   //Here place code for checking some of timers Out
   //==================================================================
   if(CursPlace == Home)arduTime = dsTime.getTime(); //Get current time from DS3231
   
   if(WT[0][0].Check(arduTime.dow, arduTime.hour, arduTime.min)||
      WT[0][1].Check(arduTime.dow, arduTime.hour, arduTime.min)||
      WT[0][2].Check(arduTime.dow, arduTime.hour, arduTime.min))
   
            digitalWrite(TMR1_OUT, HIGH);
    else
            digitalWrite(TMR1_OUT, LOW);

    if(WT[1][0].Check(arduTime.dow, arduTime.hour, arduTime.min)||
       WT[1][1].Check(arduTime.dow, arduTime.hour, arduTime.min)||
       WT[1][2].Check(arduTime.dow, arduTime.hour, arduTime.min))
   
            digitalWrite(TMR2_OUT, HIGH);
    else
            digitalWrite(TMR2_OUT, LOW);
   
   //==================================================================
   //End of code checking timers
   
   
 
   //delay(200);
   
}//End of Main Loop bracket
/*************************************************************************************/
/*                                 END OF MAIN LOOP                                  */
/*************************************************************************************/



/*************************************************************************************/
/*                                 Custom Function START                             */
/*************************************************************************************/

  
void WeekT_Display(uint8_t Timer, uint8_t TimerLyer){
  
  lcd.setCursor(0,0);
  lcd.print("Timer: ");
  lcd.print(Timer);
  lcd.print(" Lyer: ");
  lcd.print(TimerLyer);

  lcd.setCursor(0,1);
  lcd.print("Days: ");
  lcd.print(WT[Timer-1][TimerLyer-1].GetWeekPlanStr());

  lcd.setCursor(0,2);
  lcd.print("On:  ");
  if(WT[Timer-1][TimerLyer-1].OnHour<10)
  lcd.print("0");
  lcd.print(WT[Timer-1][TimerLyer-1].OnHour, DEC);
  lcd.print(":");
  if(WT[Timer-1][TimerLyer-1].OnMinutes<10)
  lcd.print("0");
  lcd.print(WT[Timer-1][TimerLyer-1].OnMinutes, DEC);
  
  lcd.setCursor(0,3);
  lcd.print("Off: ");
  if(WT[Timer-1][TimerLyer-1].OffHour<10)
  lcd.print("0");
  lcd.print(WT[Timer-1][TimerLyer-1].OffHour, DEC);
  lcd.print(":");
  if(WT[Timer-1][TimerLyer-1].OffMinutes<10)
  lcd.print("0");
  lcd.print(WT[Timer-1][TimerLyer-1].OffMinutes, DEC);
  
}

void MainDisplay(){


   if (CursPlace == Home) arduTime = dsTime.getTime();
   
   lcd.setCursor(0,0);
   lcd.print("Time: ");
   if (arduTime.hour < 10) lcd.print("0");
   lcd.print(arduTime.hour); lcd.print(":");
   if (arduTime.min < 10) lcd.print("0"); 
   lcd.print(arduTime.min);  lcd.print(":");
   if (arduTime.sec < 10) lcd.print("0"); 
   lcd.print(arduTime.sec);

   
   lcd.setCursor(0,1);
   lcd.print("Date: ");
   if (arduTime.date < 10)lcd.print("0");
   lcd.print(arduTime.date); lcd.print("/"); 
   if (arduTime.mon < 10) lcd.print("0");
   lcd.print(arduTime.mon);  lcd.print("/"); 
   lcd.print(arduTime.year);
   
   //lcd.print(dsTime.getDateStr(FORMAT_SHORT,FORMAT_LITTLEENDIAN,'/'));

  
   
   lcd.setCursor(0,2);
   lcd.print("Day:  ");
   lcd.print(dsTime.getDOWStr(/*FORMAT_SHORT*/));
   lcd.print("    ");
   
   lcd.setCursor(0,3);
   lcd.print("Temp: ");
   lcd.print(dsTime.getTemp());
   lcd.print(" C");
  
}
/*************************************************************************************/
/*                                 Custom Function END                             */
/*************************************************************************************/
