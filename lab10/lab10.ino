#include <WiFi.h>

const char* ssid     = "Info Orange";
const char* password = "qwerty1230";

WiFiServer server(80);

char line[256] = {};
int idx = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Connecting to WiFi...");
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void sendPage(WiFiClient &client, String text, String linkURL, String linkName) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  
  client.println("<html><body>");
  client.println("<h1>" + text + "</h1>");
  client.println("<a href=\"" + linkURL + "\">" + linkName + "</a>");
  client.println("</body></html>");
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  Serial.println("\nNew Client Connected");

  memset(line, 0, 256);
  idx = 0;

  while (client.connected()) {
    if (client.available()) {
      char c = client.read();

      if (c == '\n') {
        if (strlen(line) == 0) {
          break;  
        } else {
          Serial.print("Client requested: ");
          Serial.println(line); 

          if (strstr(line, "GET /H"))
            sendPage(client, "Ai accesat URL-ul /H", "/L", "Mergi la LOW");
          else if (strstr(line, "GET /L"))
            sendPage(client, "Ai accesat URL-ul /L", "/H", "Mergi la HIGH");
          else
            sendPage(client, "URL necunoscut", "/H", "Mergi la HIGH");

          break;
        }
      }
      else if (c != '\r') {
        line[idx++] = c;
      }
    }
  }

  client.stop();
  Serial.println("Client disconnected.");
}