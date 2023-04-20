#include <dht_nonblocking.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11

int ADCValue = 0; // initialize ADC value
int Threshold = 500; // set dark-light switchover (threshold) ADC value
static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

void setup()
{
  Serial.begin(9600);
  pinMode(2, OUTPUT);
}

/*
 * Poll for a measurement, keeping the state machine alive.  Returns
 * true if a measurement is available.
 */
static bool measure_environment(float *temperature, float *humidity)
{
  static unsigned long measurement_timestamp = millis();

  if (millis() - measurement_timestamp > 3000)
  {
    if (dht_sensor.measure(temperature, humidity) == true)
    {
      measurement_timestamp = millis();
      return (true);
    }
  }

  return (false);
}

/*
 * Main program loop.
 */
void loop()
{
  float temperature;
  float humidity;
  ADCValue = analogRead(A3);
  delay(300); // Wait for 300 millisecond(s)

  if (measure_environment(&temperature, &humidity) == true)
  {
    Serial.print("{\"Temperature\": ");
    Serial.print(temperature, 1);
    Serial.print(", \"Humidity\": ");
    Serial.print(humidity, 1);
    Serial.print(", \"Light\": ");
    Serial.print(ADCValue);
    Serial.println("}");
  }
}

