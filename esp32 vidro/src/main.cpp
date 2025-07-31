#include <WiFi.h>

void acionarEstado1();
void acionarEstado2();
void acionarEstado3();
void acionarEstado4();
void resetSaidas();
String gerarPaginaHTML();
int contagemCiclos = 0;

// --- Rede Wi-Fi ---
const char *ssid = "ESP32-GARRA";
const char *password = "12345678";
WiFiServer server(80);
String header;

#define ENTRADA_VP 36  // ED0 -    SENSOR DA GARRA
#define ENTRADA_VN 39  // ED1 -    SENSOR LIMITE FRENTE
#define ENTRADA_D34 34 // ED2 -    SENSOR TRÁS
#define ENTRADA_D32 32 // ED3 -    SENSOR LIMITE GARRA DIREITA
#define ENTRADA_D33 33 // ED4 -    SENSOR PENÚLTIMO GARRA DIREITA
#define ENTRADA_D25 25 // ED5 -    SENSOR LIMITE GARRA ESQUERDA
#define ENTRADA_D26 26 // ED6 -
#define ENTRADA_D27 27 // ED7 -

#define SAIDA_D23 23 // SD0 -     TRAVA/PINO - TRAVA A PEÇA
#define SAIDA_D22 22 // SD1 -     ESTEIRA - ANDA ATÉ LIMITE COLETA PEÇA
#define SAIDA_D21 21 // SD2 -     SEPARADOR - ANDA ATÉ O LIMITE P DESCARTE
#define SAIDA_D19 19 // SD3 -     MAGAZINE - LIMITE DIREITA GARRA
#define SAIDA_D18 18 // SD4 -     MEDIDOR - QUASE MEIO UM POUCO PRA ESQUERDA
#define SAIDA_TX2 17 // SD5 -     LIMITE DESCARTE GARRA
#define SAIDA_RX2 16 // SD6 -     ABAIXA A PRENSA
#define SAIDA_D4 4   // SD7 -     NAO FEZ NADA

void setup()
{
  Serial.begin(9600);

  pinMode(ENTRADA_VP, INPUT);
  pinMode(ENTRADA_VN, INPUT);
  pinMode(ENTRADA_D34, INPUT);
  pinMode(ENTRADA_D32, INPUT);
  pinMode(ENTRADA_D33, INPUT);
  pinMode(ENTRADA_D25, INPUT);
  pinMode(ENTRADA_D26, INPUT);
  pinMode(ENTRADA_D27, INPUT);

  pinMode(SAIDA_D23, OUTPUT);
  pinMode(SAIDA_D22, OUTPUT);
  pinMode(SAIDA_D21, OUTPUT);
  pinMode(SAIDA_D19, OUTPUT);
  pinMode(SAIDA_D18, OUTPUT);
  pinMode(SAIDA_TX2, OUTPUT);
  pinMode(SAIDA_RX2, OUTPUT);
  pinMode(SAIDA_D4, OUTPUT);

  digitalWrite(SAIDA_D23, LOW);
  digitalWrite(SAIDA_D22, LOW);
  digitalWrite(SAIDA_D21, LOW);
  digitalWrite(SAIDA_D19, LOW);
  digitalWrite(SAIDA_D18, LOW);
  digitalWrite(SAIDA_TX2, LOW);
  digitalWrite(SAIDA_RX2, LOW);
  digitalWrite(SAIDA_D4, LOW);

  WiFi.softAP(ssid, password);
  Serial.println("Rede WiFi criada");
  Serial.println(WiFi.softAPIP());

  server.begin();
}

void loop()
{
  WiFiClient client = server.available();

  if (client)
  {
    String currentLine = "";
    header = "";

    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        header += c;

        if (c == '\n')
        {
          // Fim do cabeçalho HTTP
          if (currentLine.length() == 0)
          {
            if (header.indexOf("GET /statusCiclos") >= 0)
            {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/plain");
              client.println("Connection: close");
              client.println();
              client.println(contagemCiclos);
            }
            else if (header.indexOf("GET /statusSensor") >= 0)
            {
              int estadoSensor = digitalRead(ENTRADA_VP);
              Serial.print("Sensor VP: ");
              Serial.println(estadoSensor);

              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/plain");
              client.println("Connection: close");
              client.println();
              client.println(estadoSensor);
            }
            else
            {
              // Cabeçalho padrão e resposta da página
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();

              if (header.indexOf("GET /estado1") >= 0)
                acionarEstado1();
              if (header.indexOf("GET /estado2") >= 0)
                acionarEstado2();
              if (header.indexOf("GET /estado3") >= 0)
                acionarEstado3();
              if (header.indexOf("GET /estado4") >= 0)
                acionarEstado4();
              if (header.indexOf("GET /reset") >= 0)
                resetSaidas();

              client.println(gerarPaginaHTML());
            }

            break;
          }
          else
          {
            currentLine = "";
          }
        }
        else if (c != '\r')
        {
          currentLine += c;
        }
      }
    }
    delay(1);
    client.stop();
  }
}

void acionarEstado1()
{
  digitalWrite(SAIDA_D19, HIGH);
  delay(1000);
  digitalWrite(SAIDA_D22, HIGH);
  if (digitalRead(ENTRADA_VP) == HIGH)
  {
    digitalWrite(SAIDA_D23, HIGH);
  }
}

void acionarEstado2()
{
  digitalWrite(SAIDA_D19, LOW);
  digitalWrite(SAIDA_D22, LOW);
  delay(1000);
  digitalWrite(SAIDA_D18, HIGH);
}

void acionarEstado3()
{
  digitalWrite(SAIDA_RX2, HIGH);
  delay(2000);
  digitalWrite(SAIDA_RX2, LOW);
  delay(1000);
  digitalWrite(SAIDA_D18, LOW);
}

void acionarEstado4()
{
  digitalWrite(SAIDA_TX2, HIGH);
  digitalWrite(SAIDA_D21, HIGH);
  delay(1000);
  digitalWrite(SAIDA_D23, LOW);
  delay(7000);
  digitalWrite(SAIDA_TX2, LOW);
  digitalWrite(SAIDA_D21, LOW);
  digitalWrite(SAIDA_D22, HIGH);
  delay(2000);
  digitalWrite(SAIDA_D19, HIGH);

  contagemCiclos++;
  Serial.print("Ciclo executado. Total: ");
  Serial.println(contagemCiclos);
}

void resetSaidas()
{
  digitalWrite(SAIDA_D23, LOW);
  digitalWrite(SAIDA_D22, LOW);
  digitalWrite(SAIDA_D21, LOW);
  digitalWrite(SAIDA_D19, LOW);
  digitalWrite(SAIDA_D18, LOW);
  digitalWrite(SAIDA_TX2, LOW);
  digitalWrite(SAIDA_RX2, LOW);
  digitalWrite(SAIDA_D4, LOW);
  contagemCiclos = 0;
}

String gerarPaginaHTML()
{
  String html = "<!DOCTYPE html><html lang='pt-BR'>";
  html += "<head><meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Controle da Garra</title>";
  html += "<style>";
  html += "body {";
  html += "  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;";
  html += "  background: linear-gradient(135deg, #e3f2fd, #ffffff);";
  html += "  display: flex;";
  html += "  flex-direction: column;";
  html += "  align-items: center;";
  html += "  justify-content: center;";
  html += "  padding: 0;";
  html += "  box-sizing: border-box;";
  html += "  border: 0;";
  html += "  margin: 0;";
  html += "  border: 0;";
  html += "  color: #333;";
  html += "  height: 100vh;";
  html += "}";

  html += "h1 {";
  html += "  font-size: 2.5em;";
  html += "  margin-bottom: 20px;";
  html += "  color: #0d47a1;";
  html += "}";

  html += "a { text-decoration: none; }";

  html += "button {";
  html += "  background-color: #2196f3;";
  html += "  border: none;";
  html += "  border-radius: 10px;";
  html += "  color: white;";
  html += "  padding: 16px 32px;";
  html += "  margin: 10px;";
  html += "  font-size: 18px;";
  html += "  transition: background-color 0.3s, transform 0.2s;";
  html += "  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);";
  html += "  cursor: pointer;";
  html += "}";

  html += "button:hover {";
  html += "  background-color: #1976d2;";
  html += "  transform: scale(1.05);";
  html += "}";

  html += "button.red {";
  html += "  background-color: #e53935;";
  html += "}";

  html += "button.red:hover {";
  html += "  background-color: #c62828;";
  html += "}";

  html += "#sensor {";
  html += "  margin-top: 20px;";
  html += "  font-size: 20px;";
  html += "  font-weight: bold;";
  html += "  color: #555;";
  html += "}";

  html += "#ciclos {";
  html += "  margin-top: 20px;";
  html += "  font-size: 20px;";
  html += "  font-weight: bold;";
  html += "  color: #555;";
  html += "}";
  html += "</style>";

  html += "</head><body>";
  html += "<h1>Controle da Garra</h1>";
  html += "<p><a href=\"/estado1\"><button>Estado 1 - Posição Inicial / Trava Peça</button></a></p>";
  html += "<p><a href=\"/estado2\"><button>Estado 2 - Posiciona a Peça na Prensa</button></a></p>";
  html += "<p><a href=\"/estado3\"><button>Estado 3 - Prensa a Peça</button></a></p>";
  html += "<p><a href=\"/estado4\"><button>Estado 4 - Descarta e Retorna</button></a></p>";
  html += "<p><a href=\"/reset\"><button class=\"red\">RESET</button></a></p>";
  html += "<p id=\"sensor\">Sensor da Garra: Desligado</p>";
  html += "<p id=\"ciclos\">Ciclos Executados: " + String(contagemCiclos) + "</p>";

  html += "<script>";
  html += "function atualizarSensor() {";
  html += "  fetch('/statusSensor').then(response => response.text()).then(data => {";
  html += "    document.getElementById('sensor').textContent = (data.trim() === '1') ? 'Sensor da Garra: Ligado' : 'Sensor da Garra: Desligado';";
  html += "  });";
  html += "}";

  html += "function atualizarCiclos() {";
  html += "  fetch('/statusCiclos').then(response => response.text()).then(data => {";
  html += "    document.getElementById('ciclos').textContent = 'Ciclos Executados: ' + data.trim();";
  html += "  });";
  html += "}";

  html += "setInterval(atualizarSensor, 1000);";
  html += "setInterval(atualizarCiclos, 1000);";
  html += "</script>";

  html += "</body></html>";
  return html;
}
