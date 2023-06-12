#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <string>
#include <iostream>
#include <FS.h>
#include <string.h>

// Configurando a Rede WiFi
#define WIFI_SSID "UPX V - Interfone"
#define WIFI_PASSWORD "492C9m4+"

// Definindo o TelegramBot
#define BOT_TOKEN "6289994300:AAFa6jrMKxsZK0WEuz2WG4t-32LfzhWI9RQ"
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

// Configurando pinagem
const int ledPin = 4;
const int travaPin = 13;

void setup() {
  Serial.begin(115200);
  Serial.println();

  pinMode(ledPin, OUTPUT);
  pinMode(travaPin, OUTPUT);
 
  digitalWrite(ledPin, HIGH);
  delay(2000);
  digitalWrite(ledPin, LOW);

  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org"); 
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);

}

void loop() {
  
  int numUpdates = bot.getUpdates(bot.last_message_received + 1);

  while (numUpdates) {
    String chatId = String(bot.messages[0].chat_id);
    String text = bot.messages[0].text;

    if (text == "/start") {
      bot.sendMessage(chatId, "Há alguém em frente a sua porta! Você deseja abri-la?");
      bot.sendMessage(chatId, "Escolha uma das opções:");
      bot.sendMessage(chatId, "2 - Abrir por 5 segundos e fechar em seguida");
      bot.sendMessage(chatId, "1 - Abrir");
      bot.sendMessage(chatId, "0 - Fechar");
    } else if (text == "1") {
      digitalWrite(travaPin, HIGH);
      bot.sendMessage(chatId, "A porta foi aberta");
    } else if (text == "0") {
      digitalWrite(travaPin, LOW);
      bot.sendMessage(chatId, "A porta esta fechada");
    } else if (text == "2") {
      digitalWrite(travaPin, HIGH);
      bot.sendMessage(chatId, "A porta foi aberta e fechara em breve!");
      delay(5000);
      digitalWrite(travaPin, LOW);
      bot.sendMessage(chatId, "A porta foi fechada!");
    } else if (text == "/help") {
      bot.sendMessage(chatId, "Os comandos disponíveis são:");
      bot.sendMessage(chatId, "COMANDOS DE CADASTRAMENTO E ADMINISTRAÇÃO");
      bot.sendMessage(chatId, "/help - Abrir o menu de ajuda");
      bot.sendMessage(chatId, "/cadastrar - Cadastra o ChatId da conta telegram para recebimento de alertas de visitas - Somente modo configuração");
      bot.sendMessage(chatId, "/limpar - Limpa a lista de ChatId's cadastrados no interfone - Somente modo configuração");
      bot.sendMessage(chatId, "/fotografar - Tira uma foto da sua câmera - Somente modo configuração");
      bot.sendMessage(chatId, "COMANDOS DE CONTROLE DE ENTRADAS E SAÍDAS");
      bot.sendMessage(chatId, "0 - Fechar o portão");
      bot.sendMessage(chatId, "1 - Abrir o portão");
      bot.sendMessage(chatId, "2 - Abrir por 5 segundos e fechar em seguida");
    } else if(text == "/cadastrar" || text == "/limpar" || text == "/fotografar") {
      bot.sendMessage(chatId, "Esse comando só é permitido no modo configuração");
      // Comandos de interfonia, mandando para o interfone
    } else {
      bot.sendMessage(chatId, "Comando desconhecido. Digite /help para ver os comandos disponíveis!");
    }

    numUpdates--;
  }
  

  
  delay(1000);
}