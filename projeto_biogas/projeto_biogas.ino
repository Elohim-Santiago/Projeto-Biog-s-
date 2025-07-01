#include <WiFi.h>
#include <HTTPClient.h>

// wifi do negão
const char* SECRET_SSID = "REDE 2G";
const char* SECRET_PASS = "ee061224";

// Pino do sensor MQ 5
int pino_sensor = 34; // pino do ESP32... pino analógico
float  sensorAnalógico;
float VREF = 5;

float parte_por_milhao;
float valor_porcento;
// variáveis de tempo
unsigned long tempo_anterior = 0;
int intervalo_envio = 15000; // 15 segundos
void setup() {

Serial.begin(115200);
  
analogRead(pino_sensor);
   
WiFi.begin(SECRET_SSID, SECRET_PASS);
while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao wifi...");
   }

}

void loop() {

//  a padronização do ESP32 tem 12 bits, o valor vai ser lido 
// de 0 a 4095(2^12 -1)

// para achar a tensão que equivale o valor do sensor
//  V = (tensão_lida / resolucção ADC) X Vref
// 12 bits(0-4095): tensão = (tensão_lida / 4095) X 5
int valor_sensor = analogRead(pino_sensor); // leitura analógica
float tensao_lida =  (valor_sensor / 4095.0) * VREF; // para adc de 12 bits
parte_por_milhao = tensao_lida * 1000;
valor_porcento = parte_por_milhao / 10000;
Serial.print("Concentração de Metato estimado: ");
Serial.print(valor_porcento);
Serial.println("parte por milhão");
delay(10000);

// vericação da conexão WiFi
if (WiFi.status() == WL_CONNECTED){
  WiFiClient client;
  HTTPClient http;

  // Incializa a conexão HTTP
  http.begin(client, "http://laica.ifrn.edu.br/access-ng/log/");
  http.addHeader("Content-Type", "application/json");

  // Crianção do body com o valor da porcentagem
String body = "{\"deviceMac\": \"A0:DD:6C:85:96:D0\",\"topic\": \"monitoramento_gas\", \"type\": \"INFO\",\"message\": \"parte_por_milhao = ";
body += String(parte_por_milhao, 2);
body += F(", valor_porcentagem = ");
body += String(valor_porcento,2);
body += F("\"}");


  // exibindo o conteúdo do body no monitor serial
  Serial.print("Body: ");
  Serial.println(body);

  int httpCode = http.POST(body);

  // checando a resposta da API
  if (httpCode > 0) {
    String payload = http.getString();
    Serial.print("Código de resposta: ");
    Serial.println(httpCode);
    Serial.print("Resposta da API: ");
    Serial.println(payload);
  } else {
    Serial.print("Erro na requisição: ");
    Serial.println(http.errorToString(httpCode).c_str());
  }
  // finalizando a conexão
  http.end();
}
}







