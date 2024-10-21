#include <WiFi.h>
#include <WebServer.h>

/*Put your SSID & Password*/
const char *ssid = "NETWORK_SSID";         // Enter SSID here
const char *password = "NETWORK_PASSWORD"; // Enter Password here
////////////////////////////

/* Authentication Password */
const String validPassword = "0780";
/////////////////////////////

WebServer server(80);

uint8_t RELAY_PIN = 16;
bool powerStatus = LOW;

void setup()
{
  Serial.begin(115200);
  delay(100);
  pinMode(RELAY_PIN, OUTPUT);

  // Connects to LAN.
  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());

  // Routes
  server.on("/", handle_OnConnect);
  server.on("/control", HTTP_POST, handle_control);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();
  digitalWrite(RELAY_PIN, powerStatus ? LOW : HIGH);
}

// Handles the web form that gets sent to the user upon
// successful connection to the "/" route.
void handle_OnConnect()
{
  powerStatus = LOW;
  Serial.println("GPIO4 Status: OFF");
  server.send(200, "text/html", SendHTML(powerStatus));
}

// Server API.
// Authenticates the command against the hard-coded password,
// and dispatches the appropriate actions.
void handle_control()
{
  Serial.println("Got a request.");

  if (ARGS_ASSURANCE_IF_STATEMENT_CONFIG)
  {
    VARS_INITIALIZER_LIST_CONFIG
    BACKEND_VALUES_HANDLERS_CONFIG
  }
  else
  {
    server.send(400, "text/plain", "Bad Request");
    return;
  }
}

BACKEND_HANDLER_FUNCTIONS_CONFIG
// Handles invalid routes.
void handle_NotFound()
{
  server.send(404, "text/plain", "Not found");
}

// HTML code for the web from.
String SendHTML(uint8_t powerStatus)
{
  String ptr = "";

  HTML_CONFIG

  return ptr;
}