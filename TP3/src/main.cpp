#include <math.h>
#include "DHT.h"
#define DHTPIN 2      // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

const int N_FEATURES = 12;
const float MEAN[N_FEATURES] = {
  1.59710779e1, 4.85373042e1, 1.95916202e3, 6.70996847e2,
  1.29420960e4, 1.97528810e4, 9.38631227e2, 1.00696999e2,
  1.82686136e2, 4.96914945e2, 2.01438321e2, 7.82185453e1
};
const float STD[N_FEATURES] = {
  1.43738212e1, 8.84828339e0, 7.87625627e3, 1.89589360e3,
  2.73182902e2, 6.10781113e2, 1.32724015e0, 9.20281510e2,
  1.95299547e3, 4.30117965e3, 2.18737131e3, 1.06474507e3
};
const float WEIGHTS[N_FEATURES] = {
  -1.08780205f, 1.42235606f, -17.43100344f, 5.42896129f, 3.30972065f
   -7.72708402f, -2.43886775f, -0.31748721f, -0.19510689f, -0.4230216f
   -0.18567765f, -0.0771766f
};
const float BIAS = -.31355287f; /* b */

float X[N_FEATURES] = {.0f, .0f, .0f, .0f, .0f, .0f, .0f, .0f, .0f, .0f, .0f, .0f}; // Input features

float* standerdize() {
  for (int i = 0; i < N_FEATURES; i++)
    X[i] = (X[i] - MEAN[i]) / STD[i];
}

float sigmoid(float z) {
  return 1.f / (1.f + exp(-z));
}

void setup()
{
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  dht.begin();
}

void loop()
{
  delay(2000);
  memcpy(X, MEAN, N_FEATURES * sizeof(float));

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // add data to input array
  X[0] = t;
  X[1] = h;

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f))
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  standerdize();

  float z = .0f;
  for (int i = 0; i < N_FEATURES; i++)
    z += X[i] * WEIGHTS[i] + BIAS;

  z = sigmoid(z);

  // Compute heat index in Fahrenheit (the default)
  // float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  // float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%  Tempeature: ");
  Serial.print(t);
  Serial.print("°C ");
  Serial.println(f);

  Serial.print("Z: ");
  Serial.print(z);
  Serial.print("State: ");
  Serial.println(z > .7f ? "Fire" : "Not fire");
}
