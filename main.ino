#include <PietteTech_DHT.h>
#include <HttpClient.h>

#define DHTTYPE       DHT22
#define DHTPIN        D6
#define DHTPOWERPIN   D7

void dht_wrapper();
char cmd[64];
double t;
double h;
unsigned long co2;

PietteTech_DHT DHT(DHTPIN, DHTTYPE, dht_wrapper);
int n;      // counter

void dht_wrapper() {
  DHT.isrCallback();
}

byte co2cmd[] = {
  0xFE,
  0X44,
  0X00,
  0X08,
  0X02,
  0X9F,
  0X25
};

HttpClient http;

http_header_t headers[] = {
    { "Content-Type" , "application/json"},
    { "Accept" , "*/*"},
    { NULL, NULL }
};

http_request_t request;
http_response_t response;

char *apiKey = "e21...f9f";
char *projectId = "56b...00c";
char urlBuf[512];



void setup() {
  sprintf(urlBuf, "/3.0/projects/%s/events/measurements?api_key=%s", projectId, apiKey);

  Serial.begin(9600);
  Serial1.begin(9600);

  Particle.variable("t", t);
  Particle.variable("h", h);
  Particle.variable("co2", co2);

  pinMode(DHTPOWERPIN, OUTPUT);
}

void loop() {

  digitalWrite(DHTPOWERPIN, HIGH);
  int result = DHT.acquireAndWait(0);
  t = DHT.getCelsius();
  h = DHT.getHumidity();
  digitalWrite(DHTPOWERPIN, LOW);

  co2 = co2val(co2run(co2cmd));

  sprintf(cmd, "{\"ppm\":%d,\"t\":%.2f,\"h\":%.2f}", co2, t, h);

  Particle.publish("data", cmd, 600);

  request.hostname = "api.keen.io";
  request.port = 80;
  request.path = urlBuf;
  request.body = cmd;

  http.post(request, response, headers);

  delay(30000);
  /*System.sleep(SLEEP_MODE_DEEP,60);*/
}

byte* co2run(byte packet[]) {
  byte co2res[] = { 0, 0, 0, 0, 0, 0, 0 };

  while( !Serial1.available() ) {
    Serial1.write(co2cmd, 7);
    delay(50);
  }

  int timeout=0;
  while( Serial1.available() < 7 ) {
    timeout++;

    if(timeout > 10) {
      while(Serial1.available()) Serial1.read();
      break;
    }

    delay(50);
  }

  for (int i = 0; i < 7; i++) co2res[i] = Serial1.read();

  return co2res;
}

unsigned long co2val(byte packet[]) {
  int high          = packet[3];
  int low           = packet[4];

  return high * 256 + low;
}
