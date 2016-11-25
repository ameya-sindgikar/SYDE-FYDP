/*
  GUINO DASHBOARD TEMPLATE FOR THE ARDUINO. 
 Done by Mads Hobye as a part of Instructables (AIR Program) & Medea (PhD Student).
 Licens: Creative Commons — Attribution-ShareAlike
 
 It should be used with the GUINO Dashboard app.
 
 More info can be found here: www.hobye.dk
 
 # This is your main template to edit.
 */

// Dummy example variables
int size = 0;
int width = 230;
int amplitude = 20;
int ledLight = 0;
int max = 0;
int min = 0;
int pause = 0;
int buttonId = 0;
int r = 75;
int g = 75;
int b = 110;
int graphValue = 0;
int fixedGraph = 0;
int count = 0;
int defaultacceleration[] = {50, 100, 100, 50, 30};

int rotaryRID,rotaryGID,rotaryBID,rotaryRID2,rotaryGID2,rotaryBID2;
int flexLabelId =0;
float angle = 0;
void setup(){
  pinMode(13, OUTPUT);
  digitalWrite(13,LOW);
  // Start the guino dashboard interface.
  // The number is your personal key for saving data. This should be unique for each sketch
  // This key should also be changed if you change the gui structure. Hence the saved data vill not match.
  gBegin(34236); 


}


void loop(){
  // **** Main update call for the guino
  guino_update();

  fixedGraph = random(200);
  //Pause button on gui stop the program for 2 seconds.
  if(pause)
  {
    gUpdateLabel(flexLabelId, "YOU PAUSED THE PROGRAM");
    delay(2000);
    gUpdateLabel(flexLabelId, "PROGRAM RUNNING AGAIN");
    pause = false;
    gUpdateValue(&pause);
  }

  // Update the graph value
    // Radians 0 to 6.28
  angle = angle  +(1.0f/(float)width);
  if(angle > 6.28)
  {
    angle = 0;
  }
  if (count == 4){
    count = 0;
    }
  graphValue = defaultacceleration[count];
  
  
  // Attach a led to pin 11 to control it.
  analogWrite(11,graphValue);

  // Led at pin 13 blinks according to the sine wave below.
  digitalWrite(13,graphValue > 0);

  count++;

  // Send the value to the gui.
  gUpdateValue(&graphValue);
  gUpdateValue(&fixedGraph);
  delay(500);


}

// This is where you setup your interface 
void gInit()
{

  gAddLabel("SENSORS",2);
  gAddSpacer(1);
  gAddSlider(323,500,"ALTITUDE",&ledLight);
  gAddSlider(25,180,"PITCH",&width);
   gAddSlider(3,50,"ROLL", &amplitude);
   

  gAddSlider(0,255,"IMPACT",&ledLight); 
  // The rotary sliders 
  gAddLabel("G FORCE",2);
  gAddSpacer(1);

  rotaryRID = gAddRotarySlider(0,2,"X",&r);
  rotaryGID = gAddRotarySlider(0,2,"Y",&g);
  rotaryBID = gAddRotarySlider(0,2,"Z",&b);
  
  gAddLabel("Failures Detected This Flight",2);

  rotaryRID2 = gAddRotarySlider(0,10,"Impact",&r);
  rotaryGID2 = gAddRotarySlider(0,10,"Prop",&g);
  rotaryBID2 = gAddRotarySlider(0,10,"Motor",&b);


  gAddSpacer(1);
  flexLabelId = gAddLabel("FAILURE DETECTED!",2);
  gAddSpacer(1);

  gAddColumn();

  gAddLabel("GRAPHS",1);
  gAddSpacer(1);

  // Last parameter in moving graph defines the size 10 = normal
  gAddMovingGraph("Failure Detection",0,100, &graphValue, 20);
  gAddSlider(-100,100,"G Force",&graphValue);
  gAddFixedGraph("Failure Detection",-100,100,100,&graphValue,20);
  // The graphs take up two columns we are going to add two
  gAddColumn();

  // Add more stuff here.
  gSetColor(r,g,b); // Set the color of the gui interface.
}

// Method called everytime a button has been pressed in the interface.
void gButtonPressed(int id)
{
  if(buttonId == id)
  {
    amplitude= 100; 
    gUpdateValue(&amplitude);
  }
}

void gItemUpdated(int id)
{
  if(rotaryRID == id || rotaryGID == id || rotaryBID == id)
  {
    gSetColor(r,g,b);
  }
}








