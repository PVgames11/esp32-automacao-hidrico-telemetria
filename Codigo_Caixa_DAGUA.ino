#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <UrlEncode.h> 

// --- Definições dos pinos ---
#define TRIG 5
#define ECHO 4
#define RELE 2 

// --- Medidas da caixa (cm) ---
const float COMPRIMENTO = 50.0;
const float LARGURA = 35.0;
const float ALTURA_CAIXA = 40.0;
const float VOLUME_TOTAL = (COMPRIMENTO * LARGURA * ALTURA_CAIXA) / 1000.0;

// --- Limites ---
const float NIVEL_MINIMO = 15.0; // %
const float NIVEL_MAXIMO = 40.0; // %

// --- Controle da bomba ---
bool bombaLigada = false;
bool modoManual = false; 

// --- WiFi ---
const char* ssid = "Seu wifi";
const char* pass = "senha";

// --- Backend ---
String API_BASE = "https://caixadagua.techfund.net.br/api";

// --- WhatsApp (📱 CALLMEBOT CONFIGURAÇÃO) ---
String numeroTelefone = "+55seunumero"; // seu número com código internacional
String apiKey = "sua chave"; // API key do CallMeBot

// --- Função para enviar mensagens pelo WhatsApp ---
void sendWhatsApp(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client;

    String url = "http://api.callmebot.com/whatsapp.php?phone=" + numeroTelefone +
                 "&apikey=" + apiKey +
                 "&text=" + urlEncode(message);

    http.begin(client, url);
    int httpResponseCode = http.GET();

    if (httpResponseCode == 200) {
      Serial.println("📱 WhatsApp enviado: " + message);
    } else {
      Serial.println("❌ Falha ao enviar WhatsApp. Código: " + String(httpResponseCode));
    }
    http.end();
  } else {
    Serial.println("⚠ WiFi desconectado. Não foi possível enviar WhatsApp.");
  }
}

// --- Variáveis de loop ---
float percentual = 0.0;
unsigned long ultimoLoop = 0;
const unsigned long intervalo = 10000; // 10s

// --- Função de medição ---
float medirCaixa() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duracao = pulseIn(ECHO, HIGH);
  float distancia = duracao * 0.0343 / 2;
  float alturaAgua = ALTURA_CAIXA - distancia;

  if (alturaAgua < 0) alturaAgua = 0;
  if (alturaAgua > ALTURA_CAIXA) alturaAgua = ALTURA_CAIXA;

  float volume = (COMPRIMENTO * LARGURA * alturaAgua) / 1000.0;
  percentual = (volume / VOLUME_TOTAL) * 100.0;
  return percentual;
}

// --- API GET comando ---
String buscarComando() {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;
  String url = API_BASE + "/comandos/get";
  
  http.begin(client, url);
  int code = http.GET();
  String payload = "";
  
  if (code > 0 && code == HTTP_CODE_OK) payload = http.getString();
  http.end();
  return payload;
}

// --- API POST confirmar comando ---
void confirmarComando(String status, String mensagem) {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;
  String url = API_BASE + "/comandos/confirm";
  
  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");
  String body = "{\"status\":\"" + status + "\",\"mensagem\":\"" + mensagem + "\"}";
  int code = http.POST(body);
  http.end();
}

// --- API POST nível ---
void enviarNivel(float nivel) {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;
  String url = API_BASE + "/nivel/nivel"; 
  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");
  String body = "{\"percentual\":" + String(nivel, 1) + "}";
  http.POST(body);
  http.end();
}

// --- Forçar modo automático ---
void forcarModoAutomatico(String mensagem) {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;
  String url = API_BASE + "/comandos/update"; 
  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");
  String body = "{\"tipo\":\"desligar\", \"modo\":\"automatico\"}"; 
  http.POST(body);
  http.end();
  sendWhatsApp("⚠ Segurança ativada! " + mensagem); // 📱 Mensagem de alerta
}

// --- Lógica principal ---
void executarLogica() {
  float nivel = medirCaixa();
  Serial.println("📊 Nível atual: " + String(nivel, 1) + "%");

  String tipoComando = ""; 
  String modoRemoto = ""; 

  String comandoJson = buscarComando();

  if (comandoJson.length() > 0) {
    StaticJsonDocument<512> doc; 
    DeserializationError error = deserializeJson(doc, comandoJson);
    if (!error && doc["success"]) {
      tipoComando = doc["comando"]["tipo"].as<String>();
      modoRemoto = doc["comando"]["modo_operacao"].as<String>(); 
      bool novoModoManual = (modoRemoto == "manual");

      // 📱 Envia mensagem se o modo for alterado
      if (modoManual != novoModoManual) {
        if (novoModoManual)
          sendWhatsApp("⚙ Sistema entrou em Modo MANUAL (manutenção).");
        else
          sendWhatsApp("🤖 Sistema voltou ao Modo AUTOMÁTICO.");
      }

      modoManual = novoModoManual;
    }
  }

  // --- Segurança (caixa cheia) ---
  if (bombaLigada && nivel >= NIVEL_MAXIMO) {
    bombaLigada = false;
    if (modoManual) {
      String msg = "💧 Caixa cheia! Bomba desligada automaticamente no modo manual.";
      forcarModoAutomatico(msg);
    } else {
      confirmarComando("executado", "Bomba desligada automaticamente: nível máximo atingido.");
      sendWhatsApp("💧 Caixa cheia! Bomba foi desligada automaticamente.");
    }
  }

  // --- Controle automático ---
  if (!modoManual) {
    if (nivel <= NIVEL_MINIMO && !bombaLigada) {
      bombaLigada = true;
      sendWhatsApp("🚰 Caixa vazia! Bomba ligada automaticamente.");
    }
  }

  // --- Comandos manuais ---
  if (modoManual) {
    if (tipoComando == "ligar" && !bombaLigada && nivel < NIVEL_MAXIMO) {
      bombaLigada = true;
      confirmarComando("executado", "Bomba ligada remotamente.");
      sendWhatsApp("🟢 Bomba LIGADA remotamente.");
    } 
    else if (tipoComando == "desligar" && bombaLigada) {
      bombaLigada = false;
      confirmarComando("executado", "Bomba desligada remotamente.");
      sendWhatsApp("🔴 Bomba DESLIGADA remotamente.");
    }
  }

  // --- Atualiza relé e envia nível ---
  digitalWrite(RELE, bombaLigada ? LOW : HIGH);
  enviarNivel(nivel);
}

// --- Setup ---
void setup() {
  Serial.begin(115200);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(RELE, OUTPUT);
  digitalWrite(RELE, HIGH);

  WiFi.begin(ssid, pass);
  Serial.print("🔌 Conectando WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi conectado!");
  Serial.println("IP: " + WiFi.localIP().toString());

  // 📱 Envia mensagem de inicialização
  sendWhatsApp("🚀 Sistema da caixa d'água iniciado com sucesso!");
}

// --- Loop ---
void loop() {
  unsigned long agora = millis();
  if (agora - ultimoLoop >= intervalo) {
    ultimoLoop = agora;
    executarLogica();
  }
}