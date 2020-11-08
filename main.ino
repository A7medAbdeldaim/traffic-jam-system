
#include <stdio.h>
#include <time.h>
#include "SevSeg.h"
SevSeg sevseg;
int LEDRa = 33;
int LEDGa = 32;
int LEDRb = 23;
int LEDGb = 22;
int LEDRc = 50;
int LEDGc = 51;
int LEDRd = 42;
int LEDGd = 43;

int IRa1 = 44;
int IRa2 = 52;
int IRb1 = 49;
int IRb2 = 48;
int IRc1 = 35;
int IRc2 = 37;
int IRd1 = 47;
int IRd2 = 46;

int RoadATraffic[] = {
  0,
  0
};
int RoadBTraffic[] = {
  0,
  0
};
int RoadCTraffic[] = {
  0,
  0
};
int RoadDTraffic[] = {
  0,
  0
};
void setup() {
  pinMode(LEDRa, OUTPUT);
  pinMode(LEDGa, OUTPUT);
  pinMode(LEDRb, OUTPUT);
  pinMode(LEDGb, OUTPUT);
  pinMode(LEDRc, OUTPUT);
  pinMode(LEDGc, OUTPUT);
  pinMode(LEDRd, OUTPUT);
  pinMode(LEDGd, OUTPUT);
  pinMode(IRa1, INPUT);
  pinMode(IRa2, INPUT);
  pinMode(IRb1, INPUT);
  pinMode(IRb2, INPUT);
  pinMode(IRc1, INPUT);
  pinMode(IRc2, INPUT);
  pinMode(IRd1, INPUT);
  pinMode(IRd2, INPUT);
  
  byte numDigits = 2;
  byte digitPins[] = {
    11,
    10
  };
  byte segmentPins[] = {
    9,
    8,
    6,
    3,
    4,
    7,
    2,
    5
  };
  bool resistorsOnSegments = true;
  byte hardwareConfig = COMMON_CATHODE;
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);
  Serial.begin(9600); // open the serial port at 9600 bps:
}
const int SIZE = 4;
int jam_val[] = {
  0,
  0,
  0,
  0
};
int jam_time[] = {
  0,
  0,
  0,
  0
};
char jam_char[] = {
  'n',
  'n',
  'n',
  'n'
};
int jam_a, jam_b, jam_c, jam_d;
char max_jam, current_route;
static int counter = 99;
int jam_ind = 0;
bool start = false;
void loop() {
  RoadATraffic[0] = (digitalRead(IRa1) != 1) ? (1) : (0);
  RoadATraffic[1] = (digitalRead(IRa2) != 1) ? (1) : (0);
  RoadBTraffic[0] = (digitalRead(IRb1) != 1) ? (1) : (0);
  RoadBTraffic[1] = (digitalRead(IRb2) != 1) ? (1) : (0);
  RoadCTraffic[0] = (digitalRead(IRc1) != 1) ? (1) : (0);
  RoadCTraffic[1] = (digitalRead(IRc2) != 1) ? (1) : (0);
  RoadDTraffic[0] = (digitalRead(IRd1) != 1) ? (1) : (0);
  RoadDTraffic[1] = (digitalRead(IRd2) != 1) ? (1) : (0);
  jam_a = RoadATraffic[0] + RoadATraffic[1];
  jam_b = RoadBTraffic[0] + RoadBTraffic[1];
  jam_c = RoadCTraffic[0] + RoadCTraffic[1];
  jam_d = RoadDTraffic[0] + RoadDTraffic[1];
  if (start == false) {
    current_route = 'n';
    change_route(current_route);
    start = true;
  }
  get_jam();
  sort();
  display();
}
void get_jam() {
  /*
  ** if route in jam_dict and value = current value then escape
  ** if route not in jam_dict the add it and add value and add time
  ** if route in jam_dict and value != current value then update current value and time
  ** get index of the route
  */
  
  /*
  ** Route A
  */
  int ind_a = find('a');
  if (ind_a == -1 && jam_a != 0) {        // Route Not Found in Jam_Char Array
    int ind_null = find('n');             // Get the first empty index in the array
    if (ind_null != -1) {                 // Add it
      jam_char[ind_null] = 'a';
      jam_val[ind_null] = jam_a;
      jam_time[ind_null] = (int) time(NULL);
    }
  } else if (jam_a == 0 && ind_a != -1) {   // Route Found but not Jammed
    jam_char[ind_a] = 'n';
    jam_val[ind_a] = 0;
    jam_time[ind_a] = 0;
  } else if (ind_a != -1 && jam_val[ind_a] != jam_a) {  // Route Found in Jam_char array but current value is different
    jam_val[ind_a] = jam_a;
    jam_time[ind_a] = (int) time(NULL);
  }
  
  /*
  ** Route B
  */
  
  int ind_b = find('b'); // get index of the route
  if (ind_b == -1 && jam_b != 0) { // Route Not Found in Jam_Char Array
    int ind_null = find('n'); // Get the first empty index in the array
    if (ind_null != -1) { // Add it
      jam_char[ind_null] = 'b';
      jam_val[ind_null] = jam_b;
      jam_time[ind_null] = (int) time(NULL);
    }
  } else if (jam_b == 0 && ind_b != -1) { // Route Found but not Jammed
    jam_char[ind_b] = 'n';
    jam_val[ind_b] = 0;
    jam_time[ind_b] = 0;
  } else if (ind_b != -1 && jam_val[ind_b] != jam_b) { // Route Found in Jam_char array but current value is different
    jam_val[ind_b] = jam_b;
    jam_time[ind_b] = (int) time(NULL);
  }
  
  /*
  ** Route C
  */
  
  // get index of the route
  int ind_c = find('c');
  if (ind_c == -1 && jam_c != 0) { // Route Not Found in Jam_Char Array
    int ind_null = find('n'); // Get the first empty index in the array
    if (ind_null != -1) { // Add it
      jam_char[ind_null] = 'c';
      jam_val[ind_null] = jam_c;
      jam_time[ind_null] = (int) time(NULL);
    }
  } else if (jam_c == 0 && ind_c != -1) { // Route Found but not Jammed
    jam_char[ind_c] = 'n';
    jam_val[ind_c] = 0;
    jam_time[ind_c] = 0;
  } else if (ind_c != -1 && jam_val[ind_c] != jam_c) { // Route Found in Jam_char array but current value is different
    jam_val[ind_c] = jam_c;
    jam_time[ind_c] = (int) time(NULL);
  }
  
  /*
  ** Route D
  */
  
  // get index of the route
  int ind_d = find('d');
  if (ind_d == -1 && jam_d != 0) { // Route Not Found in Jam_Char Array
    int ind_null = find('n'); // Get the first empty index in the array
    if (ind_null != -1) { // Add it
      jam_char[ind_null] = 'd';
      jam_val[ind_null] = jam_d;
      jam_time[ind_null] = (int) time(NULL);
    }
  } else if (jam_d == 0 && ind_d != -1) { // Route Found but not Jammed
    jam_char[ind_d] = 'n';
    jam_val[ind_d] = 0;
    jam_time[ind_d] = 0;
  } else if (ind_d != -1 && jam_val[ind_d] != jam_d) { // Route Found in Jam_char array but current value is different
    jam_val[ind_d] = jam_d;
    jam_time[ind_d] = (int) time(NULL);
  }
}
void sort() {
  /*
  ** sort by the maximum value
  ** then sort by time if only equal values
  ** then add current route to the begining
  */
  
  int i, j, tmp;
  char tmp_char;
  for (i = 0; i < SIZE; i++) {
    for (j = i + 1; j < SIZE; j++) {
      if (jam_val[i] < jam_val[j]) {
        tmp = jam_val[i];
        jam_val[i] = jam_val[j];
        jam_val[j] = tmp;
        tmp_char = jam_char[i];
        jam_char[i] = jam_char[j];
        jam_char[j] = tmp_char;
        tmp = jam_time[i];
        jam_time[i] = jam_time[j];
        jam_time[j] = tmp;
      }
    }
  }

  // Sort By Time
  for (i = 0; i < SIZE; i++) {
    for (j = i + 1; j < SIZE; j++) {
      if (jam_val[i] == jam_val[j] && jam_time[i] < jam_time[j]) {
        tmp = jam_val[i];
        jam_val[i] = jam_val[j];
        jam_val[j] = tmp;
        tmp_char = jam_char[i];
        jam_char[i] = jam_char[j];
        jam_char[j] = tmp_char;
        tmp = jam_time[i];
        jam_time[i] = jam_time[j];
        jam_time[j] = tmp;
      }
    }
  }
  
  max_jam = jam_char[1];
  
  // TODO: Check this Bug
  // Add the current route to the begining of the array
  //  int ind = find(current_route);
  //  if (ind != -1 && ind != 0) {
  //    int temp_val = jam_val[ind];
  //    int temp_time = jam_time[ind];
  //    char temp_char = jam_char[ind];
  //    for (int k = ind; k > 0; k--) {
  //      jam_val[k] = jam_val[k - 1];
  //      jam_char[k] = jam_char[k - 1];
  //      jam_time[k] = jam_time[k - 1];
  //    }
  //    jam_val[0] = temp_val;
  //    jam_char[0] = temp_char;
  //    jam_time[0] = temp_time;
  //  }
  // add the current route to the beigning of the array
  // then add the maximum value
  // if 2 elements or more are equal then sort by time
}


void display() {
  /*
  ** display the counter
  ** if maximum value in jam array not equal current route
  ** then set counter = 5 if the counter > 5
  ** if maximum value in jam array == current route and counter < 3
  ** then add 5 to counter
  ** if counter == 0 then
  ** set current_route == next_route [always = 1]
  ** change_route to the current_route
  ** set counter == 99
  */
  
  for (int h = 1; h < 10000; h++) {
    sevseg.setNumber(counter);
    sevseg.refreshDisplay();
  }
  
  /*
  ** if maximum value in jam array not equal current route
  ** then set counter = 5 if the counter > 5
  */
  
  
  if (current_route != max_jam && jam_val[0] == 0 && counter > 1) {
    counter = 1;
    
  } else if (current_route == max_jam && counter <= 1) {
    /*
    ** if maximum value in jam array == current route and counter < 3
    ** then add 5 to counter
    */

    if (jam_val[0] == 1) {
      counter = 16;  
    } else if (jam_val[0] == 2) {
      counter = 21;
    }
  } else {
    /*
    ** if counter == 0 then
    ** set current_route == next_route [always = 1]
    ** change_route to the current_route
    ** set counter == 99
    */
    
    if (counter <= 0 || (current_route != max_jam && jam_val[0] == 0)) {
      
      current_route = jam_char[1];
      if (jam_val[1] == 1) {
        counter = 16;  
      } else if (jam_val[1] == 2) {
        counter = 21;
      } else {
        counter = 100;
      }
      
      Serial.print(current_route);
      Serial.print('\n');
      change_route(current_route);
    }
    counter--;
  }
}


int find(char ele) {
  for (int i = 0; i < SIZE; i++) {
    if (jam_char[i] == ele) {
      return i;
    }
  }
  return -1;
}


void change_route(char route) {
  switch (route) {
  case 'a':
    digitalWrite(LEDGa, HIGH);
    digitalWrite(LEDRa, LOW);
    digitalWrite(LEDGb, LOW);
    digitalWrite(LEDRb, HIGH);
    digitalWrite(LEDGc, LOW);
    digitalWrite(LEDRc, HIGH);
    digitalWrite(LEDGd, LOW);
    digitalWrite(LEDRd, HIGH);
    break;
  case 'b':
    digitalWrite(LEDGa, LOW);
    digitalWrite(LEDRa, HIGH);
    digitalWrite(LEDGb, HIGH);
    digitalWrite(LEDRb, LOW);
    digitalWrite(LEDGc, LOW);
    digitalWrite(LEDRc, HIGH);
    digitalWrite(LEDGd, LOW);
    digitalWrite(LEDRd, HIGH);
    break;
  case 'c':
    digitalWrite(LEDGa, LOW);
    digitalWrite(LEDRa, HIGH);
    digitalWrite(LEDGb, LOW);
    digitalWrite(LEDRb, HIGH);
    digitalWrite(LEDGc, HIGH);
    digitalWrite(LEDRc, LOW);
    digitalWrite(LEDGd, LOW);
    digitalWrite(LEDRd, HIGH);
    break;
  case 'd':
    digitalWrite(LEDGa, LOW);
    digitalWrite(LEDRa, HIGH);
    digitalWrite(LEDGb, LOW);
    digitalWrite(LEDRb, HIGH);
    digitalWrite(LEDGc, LOW);
    digitalWrite(LEDRc, HIGH);
    digitalWrite(LEDGd, HIGH);
    digitalWrite(LEDRd, LOW);
    break;


  case 'n':
    digitalWrite(LEDGa, HIGH);
    digitalWrite(LEDRa, LOW);
    digitalWrite(LEDGb, HIGH);
    digitalWrite(LEDRb, LOW);
    digitalWrite(LEDGc, HIGH);
    digitalWrite(LEDRc, LOW);
    digitalWrite(LEDGd, HIGH);
    digitalWrite(LEDRd, LOW);
    break;
  }
}
