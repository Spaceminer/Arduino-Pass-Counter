#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SD.h>
#include <File.h>
LiquidCrystal_I2C lcd(0x3F,20,4);

//*********System Variables**********\\

int name = 0; //current name selected
const int right = 4; //right button input
const int middle = 5; //middle button input
int left = 6; //left button input
int input = 0; //Decoded input value (0 is nothing)
int trueInput = 0; //Used to stableize button pressing
int waitTime = 0; //Used to keep track of the amount of time the input is kept
boolean tracking; // If passes are being recorded

const int button = 3;
const int increase = 1;
const int timeOut = 4400;
int pressNum = 0;
int hasPush = 0;
int time = 0;

File currentFile;


//*********Tracking Variables*********\\
//Sherman
int shermanPasses;
int shermanTotal;
//O'kain
int okainPasses;
int okainTotal;
//Grubbs
int grubbsPasses;
int grubbsTotal;
//Hindmarsh
int hindmarshPasses;
int hindmarshTotal;
//Quick
int quickPasses;
int quickTotal;
//Smith
int smithPasses;
int smithTotal;
//Barsotii
int barsottiPasses;
int barsottiTotal;
//Edgell
int edgellPasses;
int edgellTotal;
//Club
int clubPasses;
int clubTotal;

//********Editable Variables**********\\

int timeToWait = 2500; //Time to wait before proceeding without buttons being released
int maxName = 9; //Total # of names stored on device

void setup()
{
  Serial.begin(9600); //Activate debug stream
  pinMode(right, INPUT); //Set input pins ot INPUT
  pinMode(middle, INPUT);
  pinMode(left, INPUT);
  lcd.init(); //Initiate LCD
  lcd.backlight();
  if (!SD.begin(10)) {
    Serial.println("Missing SD Card!");
    return;
  }
  lcd.setCursor(2,0);
  lcd.print("Sherman  Arduino"); //Show Title
  lcd.setCursor(2,2);
  lcd.print("Ski Pass Counter");
  SD.mkdir("1");
  SD.mkdir("2");
  currentFile = SD.open("1/name.txt", FILE_WRITE);
  currentFile.print("Barsotti");
  currentFile.close();
  currentFile = SD.open("2/name.txt", FILE_WRITE);
  currentFile.print("Sherman");
  currentFile.close();
}
  
  
void loop()
{
  if(getInput() == 0)
  {
    waitTime = 0;
  }
  else if(waitTime == timeToWait)
  {
    delay(200);
  }
  if(getInput() != 0)
  {
    while(getInput() != 0 && waitTime < timeToWait)
    {
      trueInput = getInput();
      delay(1);
      waitTime += 1;
    }
    switch(trueInput)
    {
      case 3 : //Right Button
        if(tracking == false)
        {
          scrollUp();
        }
        else
        {
          addPass();
        }
       break;
      
      case 1 : //Left Button
        if(tracking == false)
        {
          scrollDown();
        }
        else
        {
          removePass();
        }
      break;
      
      case 2 : //middle Button      
        toggleTracking();
      break;
      
      case 6 : //left and right buttons
        resetPasses();
      break;
      
      case 7 : //All three buttons
        resetAll();
      break;
    }
    Home();  //update Screen
  }
  if(tracking == true)
  {
    if(time > timeOut) { //time has elapsed. Not the correct bouys.
    time = 0;
    pressNum = 0;
    lcd.setCursor(11,1);
    lcd.print("Timed Out");
    }
    if(pressNum == 1) { //magnet detected
      time = time + 1;
      delay(1);
    }
    int isOn = digitalRead(button);
    if(isOn == 1) { 
      if(hasPush == 0) {
        pressNum = pressNum + 1;
        hasPush = 1;
        lcd.setCursor(11,1);
        lcd.print(" Detected");
      }
    }
    else {
      hasPush = 0;
    }
    if(pressNum == 2) { //2 magnet within time
      addPass();
      nameUpdate();
      pressNum = 0;
      time = 0;
    }
  }
}

int getInput()
{
  if(digitalRead(right) == 1 && digitalRead(left) == 1 && digitalRead(middle) == 1)
  {
    input = 7;
  }
  else if(digitalRead(right) == 1 && digitalRead(left) == 1)
  {
    input = 6;
  }
  else if(digitalRead(left) == 1 && digitalRead(middle) == 1)
  {
    input = 5;
  }
  else if(digitalRead(right) == 1 && digitalRead(middle) == 1)
  {
    input = 4;
  }
  else if(digitalRead(right) == 1)
  {
    input = 3;
  }
  else if(digitalRead(middle) == 1)
  {
    input = 2;
  }
  else if(digitalRead(left) == 1)
  {
    input = 1;
  }
  else
  {
    input = 0;
  }
  return input;
}

void nameUpdate()
{ 
  switch(name)
  {
    case 1 : 
      barsottiHome();
    break;
    
    case 2 : 
      edgellHome();
    break;
    
    case 3 : 
      grubbsHome();
    break;
    
    case 4 : 
      hindmarshHome();
    break;
    
    case 5 : 
      okainHome();
    break;
    
    case 6 : 
      quickHome();
    break;
    
    case 7 : 
      shermanHome();
    break;
    
    case 8 : 
      smithHome();
    break;
    
    case 9 :
      clubHome();
    break;
  }
}

void toggleTracking()
{
  if(tracking == false)
  {
    tracking = true;
  }
  else
  {
    tracking = false;
  }
  Serial.println(tracking);
  delay(200);
}

void scrollUp()
{
  if(name < maxName)
  {
    name += 1;
  }
  else
  {
    name = 1;
  }
}

void scrollDown()
{
  if(name > 1)
  {
    name -= 1;
  }
  else
  {
    name = maxName;
  }
}

void addPass()
{
  switch(name)
  {
    case 1 : 
      barsottiPasses += 1;
      barsottiTotal += 1;
    break;
    
    case 2 : 
      edgellPasses += 1;
      edgellTotal += 1;
    break;
    
    case 3 : 
      grubbsPasses += 1;
      grubbsTotal += 1;
    break;
    
    case 4 : 
      hindmarshPasses += 1;
      hindmarshTotal += 1;
    break;
    
    case 5 : 
      okainPasses += 1;
      okainTotal += 1;
    break;
    
    case 6 : 
      quickPasses += 1;
      quickTotal += 1;
    break;
    
    case 7 : 
      shermanPasses += 1;
      shermanTotal += 1;
    break;
    
    case 8 : 
      smithPasses += 1;
      smithTotal += 1;
    break;
    
    case 9 :
      clubPasses += 1;
      clubTotal += 1;
    break;
  }
}

void removePass()
{
  switch(name)
  {
    case 1 : 
      if(barsottiPasses > 0)
      {
        barsottiPasses -= 1;
        barsottiTotal -= 1;
      }
    break;
    
    case 2 : 
      if(edgellPasses > 0)
      {
        edgellPasses -= 1;
        edgellTotal -= 1;
      }
    break;
    
    case 3 : 
      if(grubbsPasses > 0)
      {
        grubbsPasses -= 1;
        grubbsTotal -= 1;
      }
    break;
    
    case 4 : 
      if(hindmarshPasses > 0)
      {
        hindmarshPasses -= 1;
        hindmarshTotal -= 1;
      }
    break;
    
    case 5 : 
      if(okainPasses > 0)
      {
        okainPasses -= 1;
        okainTotal -= 1;
      }
    break;
    
    case 6 : 
      if(quickPasses > 0)
      {
        quickPasses -= 1;
        quickTotal -= 1;
      }
    break;
    
    case 7 : 
      if(shermanPasses > 0)
      {
        shermanPasses -= 1;
        shermanTotal -= 1;
      }
    break;
    
    case 8 : 
      if(smithPasses > 0)
      {
        smithPasses -= 1;
        smithTotal -= 1;
      }
    break;
    
    case 9 : 
      if(clubPasses > 0)
      {
        clubPasses -= 1;
        clubTotal -= 1;
      }
    break;
  }
}

void resetPasses()
{
  switch(name)
  {
    case 1 : 
      barsottiPasses = 0;
      passDisplayZero();
      waitUntilNoInput();
    break;
    
    case 2 : 
      edgellPasses = 0;
      passDisplayZero();
      waitUntilNoInput();
    break;
    
    case 3 : 
      grubbsPasses = 0;
      passDisplayZero();
      waitUntilNoInput();
    break;
    
    case 4 : 
      hindmarshPasses = 0;
      passDisplayZero();
      waitUntilNoInput();
    break;
    
    case 5 : 
      okainPasses = 0;
      passDisplayZero();
      waitUntilNoInput();
    break;
    
    case 6 : 
      quickPasses = 0;
      passDisplayZero();
      waitUntilNoInput();
    break;
    
    case 7 : 
      shermanPasses = 0;
      passDisplayZero();
      waitUntilNoInput();
    break;
    
    case 8 : 
      smithPasses = 0;
      passDisplayZero();
      waitUntilNoInput();
    break;
    
    case 9 : 
      clubPasses = 0;
      passDisplayZero();
      waitUntilNoInput();
    break;
  }
}

void passDisplayZero()
{
  lcd.setCursor(7,1);
  lcd.print("0   *Reset*  "); //blank space to erase the rest of the line
}

void waitUntilNoInput()
{
  while(getInput() != 0)
  {
    //Elevator Music
  }
}

void resetAll()
{
  switch(name)
  {
    case 8 : 
    //Reset Barsotti
      barsottiPasses = 0;
      barsottiTotal = 0;
    //Reset edgell
      edgellPasses = 0;
      edgellTotal = 0;
    //Reset Grubbs
      grubbsPasses = 0;
      grubbsTotal = 0;
    //Reset Hindmarsh
      hindmarshPasses = 0;
      hindmarshTotal = 0;
    //Reset O'kain
      okainPasses = 0;
      okainTotal = 0;
    //Reset Quick
      quickPasses = 0;
      quickTotal = 0;
    //Reset Sherman
      shermanPasses = 0;
      shermanTotal = 0;
    //Reset Smith
      smithPasses = 0;
      smithTotal = 0;
    //Reset Club
      clubPasses = 0;
      clubTotal = 0;
    //Display Message and Wait
      allResetDisplay();
      waitUntilNoInput();
  }
}

void allResetDisplay()
{
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("--**Totals reset**--");
}

void openFileByNumber(String fileName)
{
  String tempString = String(name);
  char fileToOpen[15];
  char tempArray[10];
  tempString.toCharArray(fileToOpen,15);
  fileName.toCharArray(tempArray, 10);
  strcat(fileToOpen, "/");
  strcat(fileToOpen, tempArray);
  strcat(fileToOpen, ".txt");
  currentFile = SD.open(fileToOpen, FILE_READ);
}

void Home()
{
  lcd.clear();
  openFileByNumber("name");
  while(currentFile.available()) 
  {
    lcd.print(currentFile.read());
    Serial.print(currentFile.read());
  }
  currentFile.close();
  lcd.setCursor(0,1);
  lcd.print("Passes:");
  openFileByNumber("passes");
  while(currentFile.available()) 
  {
    lcd.print(currentFile.read());
  }
  currentFile.close();
  lcd.setCursor(0,2);
  lcd.print("Total:");
  openFileByNumber("total");
  while(currentFile.available()) 
  {
    lcd.print(currentFile.read());
  }
  currentFile.close();
  if(tracking == true)
  {
    lcd.setCursor(12,0);
    lcd.print("Tracking");
  }
}

void shermanHome()
{
  lcd.clear();
  lcd.print("Sherman");
  lcd.setCursor(0,1);
  lcd.print("Passes:");
  lcd.print(shermanPasses);
  lcd.setCursor(0,2);
  lcd.print("Total:");
  lcd.print(shermanTotal);
  if(tracking == true)
  {
    lcd.setCursor(12,0);
    lcd.print("Tracking");
  }
}

void okainHome()
{
  lcd.clear();
  lcd.print("Okain");
  lcd.setCursor(0,1);
  lcd.print("Passes:");
  lcd.print(okainPasses);
  lcd.setCursor(0,2);
  lcd.print("Total:");
  lcd.print(okainTotal);
  if(tracking == true)
  {
    lcd.setCursor(12,0);
    lcd.print("Tracking");
  }
}

void grubbsHome()
{
  lcd.clear();
  lcd.print("Grubbs");
  lcd.setCursor(0,1);
  lcd.print("Passes:");
  lcd.print(grubbsPasses);
  lcd.setCursor(0,2);
  lcd.print("Total:");
  lcd.print(grubbsTotal);
  if(tracking == true)
  {
    lcd.setCursor(12,0);
    lcd.print("Tracking");
  }
}

void hindmarshHome()
{
  lcd.clear();
  lcd.print("Hindmarsh");
  lcd.setCursor(0,1);
  lcd.print("Passes:");
  lcd.print(hindmarshPasses);
  lcd.setCursor(0,2);
  lcd.print("Total:");
  lcd.print(hindmarshTotal);
  if(tracking == true)
  {
    lcd.setCursor(12,0);
    lcd.print("Tracking");
  }
}

void quickHome()
{
  lcd.clear();
  lcd.print("Quick");
  lcd.setCursor(0,1);
  lcd.print("Passes:");
  lcd.print(quickPasses);
  lcd.setCursor(0,2);
  lcd.print("Total:");
  lcd.print(quickTotal);
  if(tracking == true)
  {
    lcd.setCursor(12,0);
    lcd.print("Tracking");
  }
}

void smithHome()
{
  lcd.clear();
  lcd.print("Smith");
  lcd.setCursor(0,1);
  lcd.print("Passes:");
  lcd.print(smithPasses);
  lcd.setCursor(0,2);
  lcd.print("Total:");
  lcd.print(smithTotal);
  if(tracking == true)
  {
    lcd.setCursor(12,0);
    lcd.print("Tracking");
  }
}

void barsottiHome()
{
  lcd.clear();
  lcd.print("Barsotti");
  lcd.setCursor(0,1);
  lcd.print("Passes:");
  lcd.print(barsottiPasses);
  lcd.setCursor(0,2);
  lcd.print("Total:");
  lcd.print(barsottiTotal);
  if(tracking == true)
  {
    lcd.setCursor(12,0);
    lcd.print("Tracking");
  }
}

void edgellHome()
{
  lcd.clear();
  lcd.print("Edgell");
  lcd.setCursor(0,1);
  lcd.print("Passes:");
  lcd.print(edgellPasses);
  lcd.setCursor(0,2);
  lcd.print("Total:");
  lcd.print(edgellTotal);
  if(tracking == true)
  {
    lcd.setCursor(12,0);
    lcd.print("Tracking");
  }
}

void clubHome()
{
  lcd.clear();
  lcd.print("Club");
  lcd.setCursor(0,1);
  lcd.print("Passes:");
  lcd.print(clubPasses);
  lcd.setCursor(0,2);
  lcd.print("Total:");
  lcd.print(clubTotal);
  if(tracking == true)
  {
    lcd.setCursor(12,0);
    lcd.print("Tracking");
  }
}
