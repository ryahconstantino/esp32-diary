#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <DNSServer.h>

const char* ssid = "Diary";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
DNSServer dnsServer;

const char* dbPath = "/diary.json";

void initFS() {
  if (!LittleFS.begin(true)) {
    return;
  }
  if (!LittleFS.exists(dbPath)) {
    File file = LittleFS.open(dbPath, "w");
    file.print("[]");
    file.close();
  }
}

void notifyClients() {
  ws.textAll("update");
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
}

void setup() {
  Serial.begin(115200);
  initFS();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, NULL);

  dnsServer.start(53, "*", WiFi.softAPIP());

  ws.onEvent(onEvent);
  server.addHandler(&ws);

  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

  server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request){
    request->redirect("/");
  });

  server.on("/api/entries", HTTP_GET, [](AsyncWebServerRequest *request){
    if(LittleFS.exists(dbPath)){
        request->send(LittleFS, dbPath, "application/json");
    } else {
        request->send(200, "application/json", "[]");
    }
  });

  server.on("/api/storage", HTTP_GET, [](AsyncWebServerRequest *request){
    size_t total = LittleFS.totalBytes();
    size_t used = LittleFS.usedBytes();
    String json = "{\"total\":" + String(total) + ", \"used\":" + String(used) + "}";
    request->send(200, "application/json", json);
  });

  server.on("/api/add", HTTP_POST, [](AsyncWebServerRequest *request){
    if(request->hasParam("text", true) && request->hasParam("date", true)) {
      String text = request->getParam("text", true)->value();
      String date = request->getParam("date", true)->value();

      File fileRead = LittleFS.open(dbPath, "r");
      DynamicJsonDocument doc(20000);
      DeserializationError error = deserializeJson(doc, fileRead);
      fileRead.close();

      if (error) {
        doc.clear();
        doc.to<JsonArray>();
      }

      JsonObject obj = doc.createNestedObject();
      obj["id"] = millis() + random(1000,9999);
      obj["date"] = date;
      obj["text"] = text;

      File fileWrite = LittleFS.open(dbPath, "w");
      if(!fileWrite) {
          request->send(500, "text/plain", "Error");
          return;
      }
      serializeJson(doc, fileWrite);
      fileWrite.close();

      notifyClients();
      request->send(200, "text/plain", "Saved");
    } else {
      request->send(400, "text/plain", "Missing Data");
    }
  });

  server.on("/api/delete", HTTP_POST, [](AsyncWebServerRequest *request){
    if(request->hasParam("id", true)) {
      String idStr = request->getParam("id", true)->value();
      unsigned long id = strtoul(idStr.c_str(), NULL, 10);

      File fileRead = LittleFS.open(dbPath, "r");
      DynamicJsonDocument doc(20000);
      deserializeJson(doc, fileRead);
      fileRead.close();

      JsonArray array = doc.as<JsonArray>();
      for (int i = 0; i < array.size(); i++) {
        if (array[i]["id"] == id) {
          array.remove(i);
          break;
        }
      }

      File fileWrite = LittleFS.open(dbPath, "w");
      serializeJson(doc, fileWrite);
      fileWrite.close();

      notifyClients();
      request->send(200, "text/plain", "Deleted");
    } else {
      request->send(400);
    }
  });

  server.onNotFound([](AsyncWebServerRequest *request){
    request->redirect("/");
  });

  server.begin();
}

void loop() {
  dnsServer.processNextRequest();
  ws.cleanupClients();
}