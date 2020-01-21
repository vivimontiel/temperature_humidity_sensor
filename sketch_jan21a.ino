#include "DHT.h"
#include <stdbool.h>/* Permet d'avoir accès au typedef bool (au lieu du type _Bool)
                     * et aux macros true et false.
                     */

#define DHTPIN D4     // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT12   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
float tenLastValues[10];
int idx = 0;

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);

  dht.begin();
}

void loop() {
  
  // Wait a few seconds between measurements.
  delay(2000);


  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Impossible de lire la sonde DHT!");
    return;
  }

  

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);


  if(idx != 10){
    Serial.print("Humidity: "); Serial.print(h); Serial.print(" %\t");
    Serial.print("Temperature: "); Serial.print(t); Serial.print(" *C "); Serial.print(f); Serial.print(" *F\t");
    Serial.print("Heat index: "); Serial.print(hic); Serial.print(" *C "); Serial.print(hif); Serial.println(" *F");
    
    tenLastValues[idx] = t;
    Serial.print(idx);
    Serial.print(":  ");
    Serial.println(tenLastValues[idx]); 
    
    
    float resultMoyenne = moyenne(tenLastValues, 10);
    float resultMediane = mediane(tenLastValues, 10);
    int resultTendance = tendance(tenLastValues, 10, idx);
    for (int i = 0; i < 10; ++i) {
      Serial.print(tenLastValues[i]);
      Serial.print(", ");
    }
    
    Serial.println(" ");
    Serial.print("MOYENNE: ");
    Serial.println(resultMoyenne);
    Serial.print("MEDIANE: ");
    Serial.println(resultMediane);
    Serial.print("TENDANCE: ");
    Serial.println(resultTendance);
    printTendance(resultTendance);

    idx = idx + 1;
  }
  if (idx >= 10) {
    idx = 0;
  }
}

float mediane(float t[], int const size) {
  float f[size];
  for (int i = 0; i < size; ++i) {
    f[i] = t[i];
  }
  tri_a_bulle(f, size);
  return f[4];
}


void tri_a_bulle(float t[], int const size) {
  bool en_desordre = true;
  for (int i = 0; i < size && en_desordre; ++i) {
    en_desordre = false;
    for (int j = 1; j < size - i; ++j) {
      if (t[j-1] > t[j]) {
        float temp = t[j-1];
        t[j-1] = t[j];
        t[j] = temp;
        en_desordre = true;
      }
    }
  }
  /*Serial.println(" MEDIANE TABLEAU TRIE ");
  for (int i = 0; i < 10; ++i) {
      Serial.print(t[i]);
      Serial.print(", ");
  }
  Serial.println(" "); */
}

float moyenne(float t[], int const size) {
  float acc = 0;
  for (int i = 0; i < size; i++) {
    acc = acc + t[i];
  }
  float result = acc / size;
  return result;
}

int tendance(float t[], int const size, int idx) {
  if (idx >= 2) {
    if(t[idx-2] < t[idx]){
      digitalWrite(D1, HIGH);
      digitalWrite(D2, LOW);
      return 1;
    }
    if(t[idx-2] > t[idx]){
      digitalWrite(D1, LOW);
      digitalWrite(D2, HIGH);
      return -1; 
    } else {
      digitalWrite(D1, LOW);
      digitalWrite(D2, LOW);
      return 0;
      
    }
  }
  if (idx == 1) {
    if(t[9] < t[idx]){
      digitalWrite(D1, HIGH);
      digitalWrite(D2, LOW);
      return 1;
    }
    if(t[9] > t[idx]){
      digitalWrite(D1, LOW);
      digitalWrite(D2, HIGH);
      return -1;
    } else {
      digitalWrite(D1, LOW);
      digitalWrite(D2, LOW);
      return 0;
    }
  }
  if (idx == 0) {
    if(t[8] < t[idx]){
      digitalWrite(D1, HIGH);
      digitalWrite(D2, LOW);
      return 1;
    }
    if(t[8] > t[idx]){
      digitalWrite(D1, LOW);
      digitalWrite(D2, HIGH);
      return -1;
    } else {
      digitalWrite(D1, LOW);
      digitalWrite(D2, LOW);
      return 0;
    }
  }
}

void printTendance(int tendance){
  if(tendance == 1) {
    Serial.println("+ + + +");
  }
  if(tendance == 0) {
    Serial.println("= = = =");
  }
  if(tendance == -1) {
    Serial.println("- - - -");
  }  
}
