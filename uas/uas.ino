#include <ESP8266WiFi.h>

#include <PubSubClient.h>

#include <DHT.h>

#include <Wire.h>

#include <LiquidCrystal_I2C.h>



// Konfigurasi Wi-Fi dan MQTT

const char* ssid = "ALVARO";

const char* password = "alvaro123";

const char* mqtt_server = "usm.revolusi-it.com";

const char* mqtt_user = "usm";

const char* mqtt_password = "usmjaya24";

const char* mqtt_topic = "test/test";

const char* client_name = "G.231.21.0188";



// Konfigurasi sensor DHT

#define DHTPIN D7 // Pin yang terhubung ke sensor DHT

#define DHTTYPE DHT11 // Tipe sensor DHT (DHT11, DHT22, dll)

DHT dht(DHTPIN, DHTTYPE);



// Konfigurasi LCD

LiquidCrystal_I2C lcd(0x27, 16, 2); // Alamat I2C LCD 16x2



// Pin untuk LED

const int led1 = D3;

const int led2 = D4;

const int led3 = D5;

const int led4 = D6;


WiFiClient espClient;

PubSubClient client(espClient);



long lastMsg = 0;

char msg[100];



void setup() {

// Mulai komunikasi serial

Serial.begin(115200);

// Set pin LED sebagai output

pinMode(led1, OUTPUT);

pinMode(led2, OUTPUT);

pinMode(led3, OUTPUT);

pinMode(led4, OUTPUT);



// Inisialisasi sensor DHT

dht.begin();

// Inisialisasi LCD

Wire.begin(D1,D2); // aktivasi D1, D2 sebagai i2c

lcd.begin(16,2);

lcd.backlight();

// Koneksi ke Wi-Fi

setup_wifi();

// Konfigurasi server MQTT dan callback

client.setServer(mqtt_server, 1883);

client.setCallback(callback);

}



void setup_wifi() {

delay(10);

// Memulai koneksi ke Wi-Fi

Serial.println();

Serial.print("Connecting to ");

Serial.println(ssid);

WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {

delay(500);

Serial.print(".");

}

Serial.println("");

Serial.println("WiFi connected");

Serial.println("IP address: ");

Serial.println(WiFi.localIP());

}



void callback(char* topic, byte* payload, unsigned int length) {

// Fungsi callback untuk menangani pesan MQTT yang diterima

Serial.print("Message arrived [");

Serial.print(topic);

Serial.print("] ");

String message;

for (int i = 0; i < length; i++) {

message += (char)payload[i];

}

Serial.println(message);

// Kontrol LED berdasarkan pesan yang diterima

if (message == "D3=1") {

digitalWrite(led1, HIGH);

} else if (message == "D3=0") {

digitalWrite(led1, LOW);

}

if (message == "D4=1") {

digitalWrite(led2, HIGH);

} else if (message == "D4=0") {

digitalWrite(led2, LOW);

}



if (message == "D5=1") {

digitalWrite(led3, HIGH);

} else if (message == "D5=0") {

digitalWrite(led3, LOW);

}

if (message == "D6=1") {

digitalWrite(led4, HIGH);

} else if (message == "D6=0") {

digitalWrite(led4, LOW);

}

if (message == "{\"nim\":\"G.231.21.0188\",\"D3\":1}") {

digitalWrite(led1, HIGH);

} else if (message == "{\"nim\":\"G.231.21.0188\",\"D3\":0}") {

digitalWrite(led1, LOW);

}

if (message == "{\"nim\":\"G.231.21.0188\",\"D4\":1}") {

digitalWrite(led2, HIGH);

} else if (message == "{\"nim\":\"G.231.21.0188\",\"D4\":0}") {

digitalWrite(led2, LOW);

}

if (message == "{\"nim\":\"G.231.21.0188\",\"D5\":1}") {

digitalWrite(led3, HIGH);

} else if (message == "{\"nim\":\"G.231.21.0188\",\"D5\":0}") {

digitalWrite(led3, LOW);

}

if (message == "{\"nim\":\"G.231.21.0188\",\"D6\":1}") {

digitalWrite(led4, HIGH);

} else if (message == "{\"nim\":\"G.231.21.0188\",\"D6\":0}") {

digitalWrite(led4, LOW);

}


}

void reconnect() {

// Loop sampai terkoneksi kembali ke server MQTT

while (!client.connected()) {

Serial.print("Attempting MQTT connection...");

// Coba koneksi ke server MQTT

if (client.connect(client_name, mqtt_user, mqtt_password)) {

Serial.println("connected");

// Berlangganan ke topik

client.subscribe(mqtt_topic);

} else {

Serial.print("failed, rc=");

Serial.print(client.state());

Serial.println(" try again in 5 seconds");

// Tunggu 5 detik sebelum mencoba kembali

delay(5000);

}

}

}



void loop() {

// Pastikan tetap terhubung ke server MQTT

if (!client.connected()) {

reconnect();

}

client.loop();

// Mengirim data suhu dan kelembaban setiap 1 detik

long now = millis();

if (now - lastMsg > 1000) {

lastMsg = now;

// Membaca suhu dan kelembaban dari sensor DHT

float h = dht.readHumidity();

float t = dht.readTemperature();

// Cek jika pembacaan gagal

if (isnan(h) || isnan(t)) {

Serial.println("Failed to read from DHT sensor!");

return;

}

// Membuat payload JSON

String payload = "{\"nim\":\"";

payload += client_name;

payload += "\",\"suhu\":";

payload += t;

payload += ",\"kelembaban\":";

payload += h;

payload += "}";

// Publish ke topik MQTT

payload.toCharArray(msg, 100);

client.publish(mqtt_topic, msg);

// Tampilkan di serial monitor

Serial.print("Published: ");

Serial.println(msg);

// Tampilkan di LCD

lcd.clear();

lcd.setCursor(0, 0);

lcd.print("Suhu: ");

lcd.print(t);

lcd.print(" C");

lcd.setCursor(0, 1);

lcd.print("Kelembaban: ");

lcd.print(h);

lcd.print(" %");

}

}

