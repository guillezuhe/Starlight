/*
#include "state_manager.hpp"

extern StateManager stateManager;
*/
AsyncWebServer server(80);

String processor(const String& var) {
  if (var == "TIMER_STATUS") {
    if (stateManager.getTimerState()) {
      return "<span class=\"timer-status\">⏱️ ON</span>";
    }
  }
  return String();
}

void setupServer() {

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.serveStatic("/", SPIFFS, "/");

  server.on("/setStarType", [](AsyncWebServerRequest *request) {
    if (request->hasArg("type")) {
      int type = request->arg("type").toInt();
      stateManager.setStarType(type);
      //request->send(200, "text/plain", "Star type set to " + String(type));
      request->redirect("/");
    } else {
      request->send(400, "text/plain", "Missing 'type' parameter");
    }
  });

  server.on("/setBrightness", [](AsyncWebServerRequest *request) {
    if (request->hasArg("value")) {
      int value = request->arg("value").toInt();
      // Transform from absolute magnitude to brightness percentage
      value = - 10 / 3 * value + 200 / 3; // Linear mapping from [-10, +20] to [100, 0]
      stateManager.setBrightness(value);
      FastLED.setBrightness(value);
      //request->send(200, "text/plain", "Brightness set to " + String(value));
      request->redirect("/");
    } else {
      request->send(400, "text/plain", "Missing 'value' parameter");
    }
  });

  server.on("/setNoiseStr", [](AsyncWebServerRequest *request) {
    if (request->hasArg("value")) {
      float value = request->arg("value").toFloat();
      // Transform from Rossby number to noise strength
      // First linearize from [0.1, 10] to [-1, 1]
      value = log10(value);
      // Then map to [0, 125] and convert to int
      value = -125 / 2 * value + 125 / 2;
      stateManager.setNoiseStr((int)value);
      //request->send(200, "text/plain", "Noise strength set to " + String(value));
      request->redirect("/");
    } else {
      request->send(400, "text/plain", "Missing 'value' parameter");
    }
  });

  server.on("/setTimer", [](AsyncWebServerRequest *request) {
    if (request->hasArg("value")) {
      int value = request->arg("value").toInt();
      // Set timer sleep duration in minutes
      if (value < 1) value = 1;
      if (value > 60) value = 60;
      stateManager.activateTimer((unsigned long)value * 60000);
      //request->send(200, "text/plain", "Sleep timer set to " + String(value) + " minutes");
      request->redirect("/");
    } else {
      request->send(400, "text/plain", "Missing 'value' parameter");
    }
  });

  server.begin();
}