// #include <Arduino.h>
// // Entradas (nomes e GPIOs)
// #define ENTRADA_VP 36  // ED0 -    SENSOR DA GARRA
// #define ENTRADA_VN 39  // ED1 -    SENSOR LIMITE FRENTE
// #define ENTRADA_D34 34 // ED2 -    SENSOR TRÁS
// #define ENTRADA_D32 32 // ED3 -    SENSOR LIMITE GARRA DIREITA
// #define ENTRADA_D33 33 // ED4 -    SENSOR PENÚLTIMO GARRA DIREITA
// #define ENTRADA_D25 25 // ED5 -    SENSOR LIMITE GARRA ESQUERDA
// #define ENTRADA_D26 26 // ED6 -
// #define ENTRADA_D27 27 // ED7 -

// // Saídas correspondentes
// #define SAIDA_D23 23 // SD0 -     TRAVA/PINO - TRAVA A PEÇA
// #define SAIDA_D22 22 // SD1 -     ESTEIRA - ANDA ATÉ LIMITE COLETA PEÇA
// #define SAIDA_D21 21 // SD2 -     SEPARADOR - ANDA ATÉ O LIMITE P DESCARTE
// #define SAIDA_D19 19 // SD3 -     MAGAZINE - LIMITE DIREITA GARRA
// #define SAIDA_D18 18 // SD4 -     MEDIDOR - QUASE MEIO UM POUCO PRA ESQUERDA
// #define SAIDA_TX2 17 // SD5 -     LIMITE DESCARTE GARRA
// #define SAIDA_RX2 16 // SD6 -     ABAIXA A PRENSA
// #define SAIDA_D4 4   // SD7 -     NAO FEZ NADA

// // Definição dos estados
// enum Estado
// {
//   ESTADO_1,
//   ESTADO_2,
//   ESTADO_3,
//   ESTADO_4,
//   ESTADO_5,
//   ESTADO_6,
//   ESTADO_7,
//   ESTADO_8,
//   ESTADO_9,
//   ESTADO_10,
// };

// Estado estadoAtual = ESTADO_1;

// void setup()
// {
//   Serial.begin(9600);
//   // Configurar entradas
//   pinMode(ENTRADA_VP, INPUT);
//   pinMode(ENTRADA_VN, INPUT);
//   pinMode(ENTRADA_D34, INPUT);
//   pinMode(ENTRADA_D32, INPUT);
//   pinMode(ENTRADA_D33, INPUT);
//   pinMode(ENTRADA_D25, INPUT);
//   pinMode(ENTRADA_D26, INPUT);
//   pinMode(ENTRADA_D27, INPUT);

//   // Configurar saídas
//   pinMode(SAIDA_D23, OUTPUT);
//   pinMode(SAIDA_D22, OUTPUT);
//   pinMode(SAIDA_D21, OUTPUT);
//   pinMode(SAIDA_D19, OUTPUT);
//   pinMode(SAIDA_D18, OUTPUT);
//   pinMode(SAIDA_TX2, OUTPUT);
//   pinMode(SAIDA_RX2, OUTPUT);
//   pinMode(SAIDA_D4, OUTPUT);

//   // Pull-Down das entradas digitais
//   digitalWrite(ENTRADA_VP, LOW);
//   digitalWrite(ENTRADA_VN, LOW);
//   digitalWrite(ENTRADA_D34, LOW);
//   digitalWrite(ENTRADA_D32, LOW);
//   digitalWrite(ENTRADA_D33, LOW);
//   digitalWrite(ENTRADA_D25, LOW);
//   digitalWrite(ENTRADA_D26, LOW);
//   digitalWrite(ENTRADA_D27, LOW);

//   // Reset das Saídas
//   digitalWrite(SAIDA_D23, LOW);
//   digitalWrite(SAIDA_D22, LOW);
//   digitalWrite(SAIDA_D21, LOW);
//   digitalWrite(SAIDA_D19, LOW);
//   digitalWrite(SAIDA_D18, LOW);
//   digitalWrite(SAIDA_TX2, LOW);
//   digitalWrite(SAIDA_RX2, LOW);
//   digitalWrite(SAIDA_D4, LOW);
// }

// void loop()
// {
//   switch (estadoAtual)
//   {
//   case ESTADO_1:
//     digitalWrite(SAIDA_D19, HIGH);
//     digitalWrite(SAIDA_D22, HIGH);
//     if (digitalRead(ENTRADA_VP) == HIGH)
//     {
//       digitalWrite(SAIDA_D23, HIGH);
//       estadoAtual = ESTADO_2;
//     }
//     break;
//     delay(1000);

//   case ESTADO_2:
//     if (digitalRead(SAIDA_D23) == HIGH)
//     { // Se têm peça faz algo
//       delay(1000);
//       digitalWrite(SAIDA_D19, LOW);
//       digitalWrite(SAIDA_D22, LOW);
//       delay(2000);
//       digitalWrite(SAIDA_D18, HIGH);
//       delay(2000);
//       estadoAtual = ESTADO_3;
//     }
//     break;
//     delay(2000);

//   case ESTADO_3:
//     if (digitalRead(SAIDA_D18) == HIGH)
//     {
//       digitalWrite(SAIDA_RX2, HIGH);
//       delay(2000);
//       digitalWrite(SAIDA_RX2, LOW);
//       delay(1000);
//       digitalWrite(SAIDA_D18, LOW);
//       estadoAtual = ESTADO_4;
//     }
//     break;
//     delay(2000);

//   case ESTADO_4:
//     if (digitalRead(SAIDA_D18) == LOW)
//     {
//       digitalWrite(SAIDA_TX2, HIGH);
//       // delay(1000);
//       digitalWrite(SAIDA_D21, HIGH);
//       delay(1000);
//       digitalWrite(SAIDA_D23, LOW);
//       delay(4000);
//       digitalWrite(SAIDA_TX2, LOW);
//       digitalWrite(SAIDA_D21, LOW);
//       digitalWrite(SAIDA_D22, HIGH);
//       delay(2000);
//       digitalWrite(SAIDA_D19, HIGH);
//       delay(1000);
//       digitalWrite(ENTRADA_VP, LOW);
//       estadoAtual = ESTADO_1;
//     }
//     break;
//   }
// }
























// Load Wi-Fi library
#include <WiFi.h>

// Network credentials Here
const char* ssid     = "ESP32-Network";
const char* password = "Esp32-Password";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

//variables to store the current LED states
String statePin16 = "off";
String statePin17 = "off";
//Output variable to GPIO pins
const int ledPin16 = 16;
const int ledPin17 = 17;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds
const long timeoutTime = 2000;

void setup() {
  Serial.begin(9600);
  
  pinMode(ledPin16, OUTPUT);      // set the LED pin mode
  digitalWrite(ledPin16, 0);      // turn LED off by default
  pinMode(ledPin17, OUTPUT);      // set the LED pin mode
  digitalWrite(ledPin17, 0);      // turn LED off by default

  WiFi.softAP(ssid,password);
  
  // Print IP address and start web server
  Serial.println("");
  Serial.println("IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client

    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            if (header.indexOf("GET /16/on") >= 0) {
              statePin16 = "on";
              digitalWrite(ledPin16, HIGH);               // turns the LED on
            } else if (header.indexOf("GET /16/off") >= 0) {
              statePin16 = "off";
              digitalWrite(ledPin16, LOW);                //turns the LED off
            }
            
            if (header.indexOf("GET /17/on") >= 0) {
              statePin17 = "on";
              digitalWrite(ledPin17, HIGH);               // turns the LED on
            } else if (header.indexOf("GET /17/off") >= 0) {
              statePin17 = "off";
              digitalWrite(ledPin17, LOW);                //turns the LED off
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            client.println("<style>html { font-family: monospace; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: yellowgreen; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 32px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: gray;}</style></head>");

            client.println("<body><h1>ESP32 Web Server</h1>");
            client.println("<p>Control LED State</p>");

            if (statePin16 == "off") {
              client.println("<p><a href=\"/16/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/16/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            if (statePin17 == "off") {
              client.println("<p><a href=\"/17/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/17/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
