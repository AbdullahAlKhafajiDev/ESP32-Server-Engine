#include <WiFi.h>
#include <WebServer.h>

/*Put your SSID & Password*/
const char* ssid = "NETWORK_SSID";  // Enter SSID here
const char* password = "NETWORK_PASSWORD";  // Enter Password here
////////////////////////////

/* Authentication Password */
const String validPassword = "0780";
/////////////////////////////

WebServer server(80);

uint8_t RELAY_PIN = 16;
bool powerStatus = LOW;

void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(RELAY_PIN, OUTPUT);

  // Connects to LAN.
  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  // Routes
  server.on("/", handle_OnConnect);
  server.on("/control", HTTP_POST, handle_control);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected, attempting to reconnect...");
    WiFi.begin(ssid, password);
    unsigned long startAttemptTime = millis();

    // Retry for up to 10 seconds
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
      delay(500);
      Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Reconnected!");
    } else {
      Serial.println("Reconnect failed.");
    }
  }

  server.handleClient();
  digitalWrite(RELAY_PIN, powerStatus ? LOW : HIGH);
}


// Handles the web form that gets sent to the user upon
// successful connection to the "/" route.
void handle_OnConnect() {
  powerStatus = LOW;
  Serial.println("GPIO4 Status: OFF");
  server.send(200, "text/html", SendHTML(powerStatus)); 
}

// Server API.
// Authenticates the command against the hard-coded password,
// and dispatches the appropriate actions.
void handle_control() {
  Serial.println("Got a request.");

  if (server.hasArg("password") && server.hasArg("mode")) {
    String inputPassword = server.arg("password");
    String mode = server.arg("mode");

    if (inputPassword == validPassword) {
      if (mode == "on") {
        Serial.println("Powering on.");
        handle_poweron();
      } else if (mode == "off") {
        Serial.println("Powering off.");
        handle_poweroff();
      } else {
        server.send(400, "text/plain", "Invalid mode");
        return;
      }
    } else {
      server.send(403, "text/plain", "Forbidden: Invalid password");
      return;
    }
  } else {
    server.send(400, "text/plain", "Bad Request");
    return;
  }
}

// Handles turning on the system, and sends a JSON response back to the client.
void handle_poweron() {
  powerStatus = HIGH;
  Serial.println("GPIO16 Status: ON");
  server.send(200, "text/html", SendHTML(powerStatus)); 
}

// Handles shutting off the system, and sends a JSON response back to the client.
void handle_poweroff() {
  powerStatus = LOW;
  Serial.println("GPIO16 Status: OFF");
  server.send(200, "text/html", SendHTML(powerStatus)); 
}

// Handles invalid routes.
void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

// HTML code for the web from.
String SendHTML(uint8_t powerStatus){
  String ptr = "<!DOCTYPE html> <html lang=\"en\">\n";
  ptr += "<head>\n";
  ptr += "    <meta charset=\"UTF-8\">\n";
  ptr += "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
  ptr += "    <link rel=\"icon\" type=\"image/x-icon\" href=\"https://gronkfrax.com/icon.ico\">\n";
  ptr += "    <title>PC Management System</title>\n";
  ptr += "    <style>\n";
  ptr += "        body {\n";
  ptr += "            margin: 0;\n";
  ptr += "            padding: 0;\n";
  ptr += "            box-sizing: border-box;\n";
  ptr += "            min-height: 100vh;\n";
  ptr += "            width: 100vw;\n";
  ptr += "            animation: bgShift 10s linear infinite;\n";
  ptr += "            display: flex;\n";
  ptr += "            align-items: center;\n";
  ptr += "            justify-content: center;\n";
  ptr += "            background: url(https://i.pinimg.com/originals/4e/de/5a/4ede5a33c5490195b2b17466ad26d124.gif) center;\n";
  ptr += "            background-size: cover;\n";
  ptr += "        }\n";
  ptr += "        .container {\n";
  ptr += "            width: 100%;\n";
  ptr += "        }\n";
  ptr += "        .window {\n";
  ptr += "            background-image: linear-gradient(to bottom right, #9c71c4, #7492d17c, #9c71c4e8, #7492d17c, #9c71c4);\n";
  ptr += "            background-blend-mode: color-burn;\n";
  ptr += "            background-size: 300% 300%;\n";
  ptr += "            backdrop-filter: blur(10px);\n";
  ptr += "            animation: color 12s ease-in-out infinite;\n";
  ptr += "            box-shadow: 0px 0px 10px #4a575980;\n";
  ptr += "            padding: 2rem 4rem;\n";
  ptr += "            border-radius: 2rem;\n";
  ptr += "            max-width: 24.4rem;\n";
  ptr += "            margin-left: auto;\n";
  ptr += "            margin-right: auto;\n";
  ptr += "        }\n";
  ptr += "        .title {\n";
  ptr += "            text-align: center;\n";
  ptr += "            background-color: black;\n";
  ptr += "            color: white;\n";
  ptr += "            font-size: 2rem;\n";
  ptr += "            padding: 1rem;\n";
  ptr += "            border-radius: 1.5rem;\n";
  ptr += "        }\n";
  ptr += "        .options {\n";
  ptr += "            color: white;\n";
  ptr += "            width: min(10vw, fit-content);\n";
  ptr += "            margin-left: max(1rem, 1vw);\n";
  ptr += "            margin-right: 0;\n";
  ptr += "        }\n";
  ptr += "        .inputSectionTitle {\n";
  ptr += "            margin-block-start: 1rem;\n";
  ptr += "            margin-block-end: 0.8rem;\n";
  ptr += "        }\n";
  ptr += "        .passwordField {\n";
  ptr += "            max-width: 100%;\n";
  ptr += "            margin-bottom: 0.5rem;\n";
  ptr += "            padding: 0.4rem 0.5rem;\n";
  ptr += "            border-radius: 0.5rem;\n";
  ptr += "            font-size: 1.2rem;\n";
  ptr += "            color: white;\n";
  ptr += "            background-color: rgba(0, 0, 0, 0.726);\n";
  ptr += "            background-blend-mode: luminosity;\n";
  ptr += "            backdrop-filter: blur(1px);\n";
  ptr += "            border: solid #9c71c4 1px;\n";
  ptr += "        }\n";
  ptr += "        .passwordField::placeholder {\n";
  ptr += "            color: white;\n";
  ptr += "            opacity: 0.2;\n";
  ptr += "        }\n";
  ptr += "        .passwordField:focus {\n";
  ptr += "            border: solid orange 1px;\n";
  ptr += "            opacity: 0.8;\n";
  ptr += "        }\n";
  ptr += "        .list-item {\n";
  ptr += "            font-size: 1.2rem;\n";
  ptr += "            list-style: none;\n";
  ptr += "            padding: 0 0 0.8rem;\n";
  ptr += "        }\n";
  ptr += "        .radioOptions {\n";
  ptr += "            appearance: none;\n";
  ptr += "            border-radius: 50%;\n";
  ptr += "            width: 16px;\n";
  ptr += "            height: 16px;\n";
  ptr += "            border: 2px solid black;\n";
  ptr += "            transition: 0.2s all linear;\n";
  ptr += "        }\n";
  ptr += "        .radioOptions:checked {\n";
  ptr += "            border: 6px solid black;\n";
  ptr += "        }\n";
  ptr += "        .radioOptions:hover, label:hover {\n";
  ptr += "            cursor: pointer;\n";
  ptr += "        }\n";
  ptr += "        .button {\n";
  ptr += "            --textColor: white;\n";
  ptr += "            --backgroundColor: black;\n";
  ptr += "            font-weight: 600;\n";
  ptr += "            width: 7rem;\n";
  ptr += "            height: 3rem;\n";
  ptr += "            font-size: 2rem;\n";
  ptr += "            margin-top: 1rem;\n";
  ptr += "            border-radius: 10px;\n";
  ptr += "            display: block;\n";
  ptr += "            margin-left: auto;\n";
  ptr += "            margin-right: auto;\n";
  ptr += "            transition-duration: 0.2s;\n";
  ptr += "            background-image: radial-gradient(yellow, orange);\n";
  ptr += "            box-shadow: 0px 0px 20px orange;\n";
  ptr += "            color: black;\n";
  ptr += "            border: none;\n";
  ptr += "        }\n";
  ptr += "        .button:hover {\n";
  ptr += "            color: var(--backgroundColor);\n";
  ptr += "            box-shadow: 0px 0px 80px orange;\n";
  ptr += "            cursor: pointer;\n";
  ptr += "        }\n";
  ptr += "        .console-window {\n";
  ptr += "            color: white;\n";
  ptr += "            font-family: Courier;\n";
  ptr += "            min-height: 20vh;\n";
  ptr += "            background-color: black;\n";
  ptr += "            border-radius: 1rem;\n";
  ptr += "            max-width: 32rem;\n";
  ptr += "            margin-left: auto;\n";
  ptr += "            margin-right: auto;\n";
  ptr += "            margin-top: 2rem;\n";
  ptr += "            overflow: hidden; \n";
  ptr += "        }\n";
  ptr += "        .console-window > span {\n";
  ptr += "            display: inline-block;\n";
  ptr += "            padding-top: 1rem;\n";
  ptr += "            padding-left: 1rem;\n";
  ptr += "        }\n";
  ptr += "        .console-window > ul {\n";
  ptr += "            width: 85%;\n";
  ptr += "            padding-bottom: 1rem;\n";
  ptr += "            overflow-x: hidden; \n";
  ptr += "            word-wrap: break-word;\n";
  ptr += "        }\n";
  ptr += "        .console-window li {\n";
  ptr += "            white-space: normal;  \n";
  ptr += "            word-wrap: break-word; \n";
  ptr += "            word-break: break-all; \n";
  ptr += "        }\n";
  ptr += "        @keyframes bgShift {\n";
  ptr += "            0% {\n";
  ptr += "                background-color: #fff1fa;\n";
  ptr += "            }\n";
  ptr += "            25% {\n";
  ptr += "                background-color: #e2daff;\n";
  ptr += "            }\n";
  ptr += "            50% {\n";
  ptr += "                background-color: #e7ffe9;\n";
  ptr += "            }\n";
  ptr += "            75% {\n";
  ptr += "                background-color: #e2daff;\n";
  ptr += "            }\n";
  ptr += "            100% {\n";
  ptr += "                background-color: #fff1fa;\n";
  ptr += "            }\n";
  ptr += "        }\n";
  ptr += "        @keyframes color {\n";
  ptr += "            0% {\n";
  ptr += "                background-position: 0 50%;\n";
  ptr += "            }\n";
  ptr += "            50% {\n";
  ptr += "                background-position: 100% 50%;\n";
  ptr += "            }\n";
  ptr += "            100% {\n";
  ptr += "                background-position: 0 50%;\n";
  ptr += "            }\n";
  ptr += "        }\n";
  ptr += "    </style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "    <div class=\"container\">\n";
  ptr += "        <div class=\"window\">\n";
  ptr += "            <h1 class=\"title\">Abdullah's Remote Power<br> Management Server</h1>\n";
  ptr += "            <form class=\"options\" method=\"post\" target=\"_self\" action=\"/control\">\n";
  ptr += "                <h2 class=\"inputSectionTitle\">Password:</h2>\n";
  ptr += "                <input type=\"password\" class=\"passwordField\" name=\"password\" placeholder=\"password\" required />\n";
  ptr += "                <h2 class=\"inputSectionTitle\">Options:</h2>\n";
  ptr += "                    <div class=\"list-item\">\n";
  ptr += "                        <input type=\"radio\" id=\"turnOnRadio\" class=\"radioOptions\" name=\"mode\" value=\"on\" checked=\"checked\" />\n";
  ptr += "                        <label for=\"turnOnRadio\">Turn On</label>\n";
  ptr += "                    </div>\n";
  ptr += "                    <div class=\"list-item\">\n";
  ptr += "                        <input type=\"radio\" id=\"turnOffRadio\" class=\"radioOptions\" name=\"mode\" value=\"off\" />\n";
  ptr += "                        <label for=\"turnOffRadio\">Turn Off</label>\n";
  ptr += "                    </div>\n";
  ptr += "                <input type=\"submit\" class=\"button\" value=\"⍬\" />\n";
  ptr += "            </form>\n";
  ptr += "        </div>\n";
  ptr += "        <div class=\"console-window\">\n";
  ptr += "            <span>console:</span>\n";
  ptr += "            <ul id=\"console-output\">\n";
  ptr += "            </ul>\n";
  ptr += "        </div>\n";
  ptr += "    </div>\n";
  ptr += "    <script defer>\n";
  ptr += "        function getCookie(key) {\n";
  ptr += "            const cookieValueArray = document.cookie.split(\"=\")\n";
  ptr += "            for (let i = 0; i < cookieValueArray.length; i++) {\n";
  ptr += "                if (cookieValueArray[i] == key) {\n";
  ptr += "                    return cookieValueArray[i + 1].split(\";\")[0];\n";
  ptr += "                }\n";
  ptr += "            }\n";
  ptr += "        }\n";
  ptr += "        function addConsoleMessage(message) {\n";
  ptr += "            const consoleOutputWindow = document.getElementById('console-output');\n";
  ptr += "            const newEvent = document.createElement('li');\n";
  ptr += "            const date = new Date();\n";
  ptr += "            newEvent.innerText = `${date.toLocaleTimeString('it-IT')} - ${message}`;\n";
  ptr += "            consoleOutputWindow.appendChild(newEvent);\n";
  ptr += "        }\n";
  ptr += "        function handleSubmission(event) {\n";
  ptr += "            event.preventDefault();\n";
  ptr += "            const formElement = document.forms[0];\n";
  ptr += "            const formData = new FormData(formElement);\n";
  ptr += "            messageObject = {};\n";
  ptr += "            for (const [key, value] of formData) {\n";
  ptr += "                if (key != \"password\") {\n";
  ptr += "                    messageObject[key] = value;\n";
  ptr += "                } else {\n";
  ptr += "                    messageObject[key] = \"*\".repeat(formData.get(\"password\").length);\n";
  ptr += "                }\n";
  ptr += "            }\n";
  ptr += "            submitForm(formData);\n";
  ptr += "            addConsoleMessage(`Sending ${JSON.stringify(messageObject)} to the server...`);\n";
  ptr += "        }\n";
  ptr += "        async function submitForm(data) {\n";
  ptr += "            try {\n";
  ptr += "                const response = await fetch(document.forms[0].action, {\n";
  ptr += "                    method: \"POST\",\n";
  ptr += "                    body: data\n";
  ptr += "                });\n";
  ptr += "                const jsonResponse = await response.json();\n";
  ptr += "                try {\n";
  ptr += "                    addConsoleMessage(`Server: ${jsonResponse.message}`);\n";
  ptr += "                } catch (error) {\n";
  ptr += "                    addConsoleMessage(`Connected to the server, but encountered a problem.`);\n";
  ptr += "                    console.log(error);\n";
  ptr += "                }\n";
  ptr += "            } catch (error) {\n";
  ptr += "                addConsoleMessage(`Could not contact the server.`);\n";
  ptr += "            }\n";
  ptr += "        }\n";
  ptr += "        document.forms[0].onsubmit = (event)=> {handleSubmission(event)};\n";
  ptr += "        if (document.cookie != \"\") {\n";
  ptr += "            document.querySelector(\".passwordField\").value = getCookie(\"password\");\n";
  ptr += "        }\n";
  ptr += "    </script>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";


  return ptr;
}
