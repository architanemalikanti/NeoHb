#include <stdlib.h>

const int photoDetectorPin = A2;

#define ColorPin660 3

#define ColorPin940 4

double I_RED[15];
float avgRed;

double I_IR[15];
float avgIR;

void setup() {
  pinMode(ColorPin660, OUTPUT);
  pinMode(ColorPin940, OUTPUT);
  pinMode(photoDetectorPin, INPUT);
  // Activate the serial port to send and receive the data to and from Android app.
  Serial.begin(9600);


  //begin getting i0 values for red light (660 nm)
   digitalWrite(ColorPin660, HIGH);

   //calculate the average of all the elements in this array
   for (int i=0; i < 15; i++) {
   
      delay(1000);
     
      avgRed = avgRed + analogRead(photoDetectorPin);
     
   }
  avgRed = avgRed/15;
 
  //Turn off the RED light
  digitalWrite(ColorPin660, LOW);

  //print this i0 red average value on the serial monitor:
  Serial.print("avgRed : ");
  Serial.print(avgRed);
  Serial.print("\n");


  //begin getting i0 values for IR light (940 nm)
  digitalWrite(ColorPin940, HIGH);

    //calculate the average of all the elements in this array
    for (int i=0; i < 15; i++) {
     
    delay(1000);
   
    avgIR = avgIR + analogRead(photoDetectorPin);
   
    }
 
  //get the average
  avgIR = avgIR / 15;

   //Turn off the IR light
  digitalWrite(ColorPin940, LOW);

  Serial.print("avgIR : ");
  Serial.print(avgIR);
  Serial.print("\n");
}

void loop() {
  double absorptionRED[15];
  double absorptionIR[15];

//accepting the I values command from the app:

  while (Serial.available()){ //wait for the signal from app, forever.
  // Read from serial port setup to get input from bluetooth android app
  char command  = Serial.read();
 
  if (command == '1') {
   
    //begin getting i values for red light (660 nm)
     
   digitalWrite(ColorPin660, HIGH);
     
   for (int i=0; i < 15; i++) {

   delay(1000);

  //these are all our i values, stored in one array, for RED LIGHT.
   I_RED[i] = analogRead(photoDetectorPin);
   }
       digitalWrite(ColorPin660, LOW);

       //now use for loop to calculate the absorption of each element in the array, remember that A = log(i0/i)
       
        for (int i = 0; i < 15; i++){

           //i is all the values under I_RED[i]
           //i0 is avgRed.
            I_RED[i] = log10( avgRed / I_RED[i] );
           
        }

       //now, with your new array, use quicksort algorithm to sort the elements
       
        for (int i = 0; i <= 15; i++){


        for (int j = i; j > 0; j--){


            if (absorptionRED[j-1]>absorptionRED[j]){
                //swap! use the temp variable
                double temp = absorptionRED[j];
                absorptionRED[j] = absorptionRED[j-1];
                absorptionRED[j-1] = temp;

            }
        }
    }


       //now pull out the biggest element (last element) and then pull our your smallest element
       //subtract both biggest and smallest, and this new value is the absorbtion of deoxygenated hemoglobin.


        double DeoxyHbAbsorption = absorptionRED[14] - absorptionRED[0];

                 Serial.print(" DeoxyHbAbsorption : ");
                 Serial.print(DeoxyHbAbsorption);
                 Serial.print("\n");  

       //now, being getting i values for IR light (940 nm)
       digitalWrite(ColorPin940, HIGH);
       
            for (int i=0; i < 15; i++) {
             
              delay(1000);

              //these are our I values, stored in an array, fro IR LIGHT.
              I_IR[i] = analogRead(photoDetectorPin);
            }

           
       digitalWrite(ColorPin940, LOW);
        //now use for loop to calculate the absorption of each element in the array, remember that A = log(i0/i)

            for (int i = 0; i < 15; i++){

           //i is all the values under I_IR[i]
           //i0 is avgIR.
            absorptionIR[i] = -log10( I_IR[i] / avgIR );
           
        }
       

       //now, with your new array, use quicksort algorithm to sort the elements


        for (int i = 0; i <= 15; i++){
        for (int j = i; j > 0; j--){
          if (absorptionIR[j-1]>absorptionIR[j]){
                //swap! use the temp variable
                double placeholder = absorptionIR[j];
                absorptionIR[j] = absorptionIR[j-1];
                absorptionIR[j-1] = placeholder;

            }
        }
    }

       //now pull out the biggest element (last element) and then pull our your smallest element
       //subtract both biggest and smallest, and this new value is the absorbtion of deoxygenated hemoglobin.

        double OxyHbAbsorption = absorptionIR[14] - absorptionIR[0];
        Serial.print(" OxyHbAbsorption : ");
        Serial.print(OxyHbAbsorption);
        Serial.print("\n");

       double totalAbsorption = OxyHbAbsorption + DeoxyHbAbsorption;
       //double finalAbsorption = totalAbsorption/10;
       char output[10];

       dtostrf(totalAbsorption, 6, 4, output);
       Serial.print(" totalAbsorption : ");
       Serial.print(output);
       Serial.print("\n");

       double concentrationHb = totalAbsorption/0.03;
       double totalconcentrationHb = concentrationHb/10;
       Serial.print(" totalconcentrationHb : ");
       Serial.print(totalconcentrationHb);
        if (totalconcentrationHb < 13.5){
          //send String value to app, indicating that the patient has anemia
                 //Serial.write('0');
                 Serial.print("Anemic");
        }else{
          //send String value to app, indicating that the patient does not have anemia
                 //Serial.write('1');
                 Serial.print("Non Anemic");
        }
       Serial.flush();
     }
    }
  }





   


 
