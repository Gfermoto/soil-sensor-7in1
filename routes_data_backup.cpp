/**
 * @file routes_data.cpp
 * @brief ╨£╨░╤Ç╤ê╤Ç╤â╤é╤ï ╨┤╨╗╤Å ╤Ç╨░╨▒╨╛╤é╤ï ╤ü ╨┤╨░╨╜╨╜╤ï╨╝╨╕ ╨┤╨░╤é╤ç╨╕╨║╨░
 * @details ╨₧╨▒╤Ç╨░╨▒╨╛╤é╨║╨░ ╨╖╨░╨┐╤Ç╨╛╤ü╨╛╨▓ ╨┐╨╛╨║╨░╨╖╨░╨╜╨╕╨╣ ╨┤╨░╤é╤ç╨╕╨║╨░, JSON API ╨╕ ╨▓╨╡╨▒-╨╕╨╜╤é╨╡╤Ç╤ä╨╡╨╣╤ü╨░ ╨┤╨░╨╜╨╜╤ï╤à
 */

#include "../../include/web_routes.h"
#include "../../include/logger.h"
#include "../../include/jxct_config_vars.h"
#include "../../include/jxct_ui_system.h"
#include "../../include/jxct_format_utils.h"
#include "../wifi_manager.h"
#include "../modbus_sensor.h"
#include "calibration_manager.h"
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <LittleFS.h>
#include <time.h>
#include "../../include/jxct_strings.h"

extern NTPClient* timeClient;

// ╨₧╨▒╤è╤Å╨▓╨╗╨╡╨╜╨╕╤Å ╨▓╨╜╨╡╤ê╨╜╨╕╤à ╤ä╤â╨╜╨║╤å╨╕╨╣
extern String navHtml();
extern String formatValue(float value, const char* unit, int precision);
extern String getApSsid();

// ╨æ╤â╤ä╨╡╤Ç ╨┤╨╗╤Å ╨╖╨░╨│╤Ç╤â╨╖╨║╨╕ ╤ä╨░╨╣╨╗╨╛╨▓ (╨║╨░╨╗╨╕╨▒╤Ç╨╛╨▓╨║╨░ ╤ç╨╡╤Ç╨╡╨╖ /readings)
static File uploadFile;
static SoilProfile uploadProfile = SoilProfile::SAND;

struct RecValues { float t, hum, ec, ph, n, p, k; };

static RecValues computeRecommendations()
{
    // 1. ╨æ╨░╨╖╨░ ╨┐╨╛ ╨║╤â╨╗╤î╤é╤â╤Ç╨╡ ╨╕╨╗╨╕ generic
    RecValues rec{21,60,1200,6.3,30,10,25};
    const char* id=config.cropId;
    if(strlen(id)>0){
        if      (strcmp(id,"tomato")==0)      rec={22,60,1500,6.5,40,10,30};
        else if (strcmp(id,"cucumber")==0)    rec={24,70,1800,6.2,35,12,28};
        else if (strcmp(id,"pepper")==0)      rec={23,65,1600,6.3,38,11,29};
        else if (strcmp(id,"lettuce")==0)     rec={20,75,1000,6.0,30,8,25};
        else if (strcmp(id,"blueberry")==0)   rec={18,65,1200,5.0,30,10,20};
        else if (strcmp(id,"lawn")==0)        rec={20,50,800,6.3,25,8,20};
        else if (strcmp(id,"grape")==0)       rec={22,55,1400,6.5,35,12,25};
        else if (strcmp(id,"conifer")==0)     rec={18,55,1000,5.5,25,8,15};
        else if (strcmp(id,"strawberry")==0)  rec={20,70,1500,6.0,35,10,25};
        else if (strcmp(id,"apple")==0)       rec={18,60,1200,6.5,25,10,20};
        else if (strcmp(id,"pear")==0)        rec={18,60,1200,6.5,25,10,20};
        else if (strcmp(id,"cherry")==0)      rec={20,60,1300,6.5,30,10,25};
        else if (strcmp(id,"raspberry")==0)   rec={18,65,1100,6.2,30,10,22};
        else if (strcmp(id,"currant")==0)     rec={17,65,1000,6.2,25,9,20};
    }

    // 2. ╨Ü╨╛╤Ç╤Ç╨╡╨║╤å╨╕╤Å ╨┐╨╛ soilProfile (╨▓╨╗╨░╨╢╨╜╨╛╤ü╤é╤î ╨╕ pH)
    int soil=config.soilProfile; // 0 sand,1 loam,2 peat,3 clay
    if(soil==0){ rec.hum+=-5; /* ╨┐╨╡╤ü╨╛╨║ */ }
    else if(soil==2){ rec.hum+=10; rec.ph-=0.3f; }
    else if(soil==3){ rec.hum+=5; /* ╨│╨╗╨╕╨╜╨░ */ }
    else if(soil==1){ rec.hum+=5; }

    // 3. ╨Ü╨╛╤Ç╤Ç╨╡╨║╤å╨╕╤Å ╨┐╨╛ environmentType
    switch(config.environmentType){
        case 1: // greenhouse
            rec.hum+=10; rec.ec+=300; rec.n+=5; rec.k+=5; rec.t+=2; break;
        case 2: // indoor
            rec.hum+=-5; rec.ec-=200; rec.t+=1; break;
    }

    // 3a. ╨Ü╨╛╨╜╨▓╨╡╤Ç╤ü╨╕╤Å NPK ╨▓ ╨╝╨│/╨║╨│ (╨┤╨░╤é╤ç╨╕╨║ ╨▓╤ï╨┤╨░╤æ╤é ╨╝╨│/╨║╨│, ╤é╨░╨▒╨╗╨╕╤å╨░ ╤à╤Ç╨░╨╜╨╕╨╗╨░╤ü╤î ╨▓ ╨╝╨│/╨┤╨╝┬│ ~ ╤ì╨║╤ü╤é╤Ç╨░╨║╤é 1:5)
    constexpr float NPK_FACTOR = 6.5f; // ╨┐╨╡╤Ç╨╡╤ü╤ç╤æ╤é ╨╝╨│/╨┤╨╝┬│ ΓåÆ ╨╝╨│/╨║╨│ (╧ü=1.3 ╨│/╤ü╨╝┬│, ╨▓╨╗╨░╨╢╨╜╨╛╤ü╤é╤î Γëê30%)
    rec.n *= NPK_FACTOR;
    rec.p *= NPK_FACTOR;
    rec.k *= NPK_FACTOR;

    // 4. ╨í╨╡╨╖╨╛╨╜╨╜╨░╤Å ╨║╨╛╤Ç╤Ç╨╡╨║╤å╨╕╤Å (╤é╨╛╨╗╤î╨║╨╛ ╨╡╤ü╨╗╨╕ ╨▓╨║╨╗╤Ä╤ç╨╡╨╜╨░)
    if(config.flags.seasonalAdjustEnabled){
        time_t now=time(nullptr); struct tm* ti=localtime(&now);
        int m=ti?ti->tm_mon+1:1;
        bool rainy=(m==4||m==5||m==6||m==10);
        
        // ╨Ü╨╛╤Ç╤Ç╨╡╨║╤å╨╕╤Å ╨▓╨╗╨░╨╢╨╜╨╛╤ü╤é╨╕ ╨╕ EC
        if(rainy){ rec.hum+=5; rec.ec-=100; }
        else{ rec.hum+=-2; rec.ec+=100; }
        
        // ╨Ü╨╛╤Ç╤Ç╨╡╨║╤å╨╕╤Å NPK ╨┐╨╛ ╤ü╨╡╨╖╨╛╨╜╤â
        if(config.environmentType == 0) { // Outdoor
            if(m >= 3 && m <= 5) { // ╨Æ╨╡╤ü╨╜╨░
                rec.n *= 1.20f; // +20%
                rec.p *= 1.15f; // +15%
                rec.k *= 1.10f; // +10%
            }
            else if(m >= 6 && m <= 8) { // ╨¢╨╡╤é╨╛
                rec.n *= 0.90f; // -10%
                rec.p *= 1.05f; // +5%
                rec.k *= 1.25f; // +25%
            }
            else if(m >= 9 && m <= 11) { // ╨₧╤ü╨╡╨╜╤î
                rec.n *= 0.80f; // -20%
                rec.p *= 1.10f; // +10%
                rec.k *= 1.15f; // +15%
            }
            else { // ╨ù╨╕╨╝╨░
                rec.n *= 0.70f; // -30%
                rec.p *= 1.05f; // +5%
                rec.k *= 1.05f; // +5%
            }
        }
        else if(config.environmentType == 1) { // Greenhouse
            if(m >= 3 && m <= 5) { // ╨Æ╨╡╤ü╨╜╨░
                rec.n *= 1.25f; // +25%
                rec.p *= 1.20f; // +20%
                rec.k *= 1.15f; // +15%
            }
            else if(m >= 6 && m <= 8) { // ╨¢╨╡╤é╨╛
                rec.n *= 1.10f; // +10%
                rec.p *= 1.10f; // +10%
                rec.k *= 1.30f; // +30%
            }
            else if(m >= 9 && m <= 11) { // ╨₧╤ü╨╡╨╜╤î
                rec.n *= 1.15f; // +15%
                rec.p *= 1.15f; // +15%
                rec.k *= 1.20f; // +20%
            }
            else { // ╨ù╨╕╨╝╨░
                rec.n *= 1.05f; // +5%
                rec.p *= 1.10f; // +10%
                rec.k *= 1.15f; // +15%
            }
        }
    }

    return rec;
}

static void handleReadingsUpload()
{
    HTTPUpload& upload = webServer.upload();
    if (upload.status == UPLOAD_FILE_START)
    {
        CalibrationManager::init();
        const char* path = CalibrationManager::profileToFilename(SoilProfile::SAND); // custom.csv
        uploadFile = LittleFS.open(path, "w");
        if (!uploadFile)
        {
            logError("╨¥╨╡ ╤â╨┤╨░╨╗╨╛╤ü╤î ╤ü╨╛╨╖╨┤╨░╤é╤î ╤ä╨░╨╣╨╗ %s", path);
        }
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    {
        if (uploadFile)
        {
            uploadFile.write(upload.buf, upload.currentSize);
        }
    }
    else if (upload.status == UPLOAD_FILE_END)
    {
        if (uploadFile)
        {
            uploadFile.close();
            logSuccess("╨ñ╨░╨╣╨╗ ╨║╨░╨╗╨╕╨▒╤Ç╨╛╨▓╨║╨╕ ╨╖╨░╨│╤Ç╤â╨╢╨╡╨╜ (%u ╨▒╨░╨╣╤é)", upload.totalSize);
        }
        webServer.sendHeader("Location", "/readings?toast=╨Ü╨░╨╗╨╕╨▒╤Ç╨╛╨▓╨║╨░+╨╖╨░╨│╤Ç╤â╨╢╨╡╨╜╨░", true);
        webServer.send(302, "text/plain", "Redirect");
    }
}

static void handleProfileSave()
{
    if (webServer.hasArg("soil_profile"))
    {
        String profileStr = webServer.arg("soil_profile");
        if (profileStr == "sand") config.soilProfile = 0;
        else if (profileStr == "loam") config.soilProfile = 1;
        else if (profileStr == "peat") config.soilProfile = 2;
        else if (profileStr == "clay") config.soilProfile = 3;

        saveConfig();
        logSuccess("╨ƒ╤Ç╨╛╤ä╨╕╨╗╤î ╨┐╨╛╤ç╨▓╤ï ╨╕╨╖╨╝╨╡╨╜╤æ╨╜ ╨╜╨░ %s", profileStr.c_str());
    }
    webServer.sendHeader("Location", "/readings?toast=╨ƒ╤Ç╨╛╤ä╨╕╨╗╤î+╤ü╨╛╤à╤Ç╨░╨╜╨╡╨╜", true);
    webServer.send(302, "text/plain", "Redirect");
}

static void sendSensorJson()
{
    // unified JSON response for sensor data
    logWebRequest("GET", webServer.uri(), webServer.client().remoteIP().toString());
    if (currentWiFiMode != WiFiMode::STA)
    {
        webServer.send(403, "application/json", "{\"error\":\"AP mode\"}");
        return;
    }

    StaticJsonDocument<512> doc;
    doc["temperature"] = format_temperature(sensorData.temperature);
    doc["humidity"] = format_moisture(sensorData.humidity);
    doc["ec"] = format_ec(sensorData.ec);
    doc["ph"] = format_ph(sensorData.ph);
    doc["nitrogen"] = format_npk(sensorData.nitrogen);
    doc["phosphorus"] = format_npk(sensorData.phosphorus);
    doc["potassium"] = format_npk(sensorData.potassium);
    doc["raw_temperature"] = format_temperature(sensorData.raw_temperature);
    doc["raw_humidity"] = format_moisture(sensorData.raw_humidity);
    doc["raw_ec"] = format_ec(sensorData.raw_ec);
    doc["raw_ph"] = format_ph(sensorData.raw_ph);
    doc["raw_nitrogen"] = format_npk(sensorData.raw_nitrogen);
    doc["raw_phosphorus"] = format_npk(sensorData.raw_phosphorus);
    doc["raw_potassium"] = format_npk(sensorData.raw_potassium);
    doc["irrigation"] = sensorData.recentIrrigation;

    RecValues rec = computeRecommendations();
    doc["rec_temperature"] = format_temperature(rec.t);
    doc["rec_humidity"] = format_moisture(rec.hum);
    doc["rec_ec"] = format_ec(rec.ec);
    doc["rec_ph"] = format_ph(rec.ph);
    doc["rec_nitrogen"] = format_npk(rec.n);
    doc["rec_phosphorus"] = format_npk(rec.p);
    doc["rec_potassium"] = format_npk(rec.k);

    // ---- ╨ö╨╛╨┐╨╛╨╗╨╜╨╕╤é╨╡╨╗╤î╨╜╨░╤Å ╨╕╨╜╤ä╨╛╤Ç╨╝╨░╤å╨╕╤Å ----
    // ╨í╨╡╨╖╨╛╨╜ ╨┐╨╛ ╤é╨╡╨║╤â╤ë╨╡╨╝╤â ╨╝╨╡╤ü╤Å╤å╤â
    const char* seasonName = [](){
        // ╨ƒ╤Ç╨╛╨▓╨╡╤Ç╤Å╨╡╨╝ ╨╕╨╜╨╕╤å╨╕╨░╨╗╨╕╨╖╨░╤å╨╕╤Ä NTP
        if (timeClient == nullptr) {
            extern WiFiUDP ntpUDP;
            timeClient = new NTPClient(ntpUDP, "pool.ntp.org", 0, 3600000);
            timeClient->begin();
        }
        
        time_t now = timeClient ? (time_t)timeClient->getEpochTime() : time(nullptr);
        // ╨╡╤ü╨╗╨╕ ╨▓╤Ç╨╡╨╝╤Å < 2000-01-01 ╤ü╤ç╨╕╤é╨░╨╡╨╝, ╤ç╤é╨╛ NTP ╨╡╤ë╤æ ╨╜╨╡ ╤ü╨╕╨╜╤à╤Ç╨╛╨╜╨╕╨╖╨╕╤Ç╨╛╨▓╨░╨╜
        if (now < 946684800) {
            // ╨ƒ╤Ç╨╛╨▒╤â╨╡╨╝ ╨╛╨▒╨╜╨╛╨▓╨╕╤é╤î NTP
            if (timeClient) {
                timeClient->forceUpdate();
                now = (time_t)timeClient->getEpochTime();
                if (now < 946684800) return "╨¥/╨ö";
            } else {
                return "╨¥/╨ö";
            }
        }
        struct tm* ti = localtime(&now);
        if (!ti) return "╨¥/╨ö";
        uint8_t m = ti->tm_mon + 1;
        if (m==12 || m==1 || m==2) return "╨ù╨╕╨╝╨░";
        if (m>=3 && m<=5)           return "╨Æ╨╡╤ü╨╜╨░";
        if (m>=6 && m<=8)           return "╨¢╨╡╤é╨╛";
        return "╨₧╤ü╨╡╨╜╤î";
    }();
    doc["season"] = seasonName;

    // ╨ƒ╤Ç╨╛╨▓╨╡╤Ç╤Å╨╡╨╝ ╨╛╤é╨║╨╗╨╛╨╜╨╡╨╜╨╕╤Å
    String alerts="";
    auto append=[&](const char* n){ if(alerts.length()) alerts += ", "; alerts += n; };
    // ╨ñ╨╕╨╖╨╕╤ç╨╡╤ü╨║╨╕╨╡ ╨┐╤Ç╨╡╨┤╨╡╨╗╤ï ╨┤╨░╤é╤ç╨╕╨║╨░
    if (sensorData.temperature < -45 || sensorData.temperature > 115) append("T");
    if (sensorData.humidity    <   0 || sensorData.humidity    > 100) append("╬╕");
    if (sensorData.ec          <   0 || sensorData.ec          > 10000) append("EC");
    if (sensorData.ph          <   3 || sensorData.ph          > 9) append("pH");
    if (sensorData.nitrogen    <   0 || sensorData.nitrogen    > 1999) append("N");
    if (sensorData.phosphorus  <   0 || sensorData.phosphorus  > 1999) append("P");
    if (sensorData.potassium   <   0 || sensorData.potassium   > 1999) append("K");
    doc["alerts"] = alerts;

    doc["timestamp"] = (long)(timeClient ? timeClient->getEpochTime() : 0);

    String json;
    serializeJson(doc, json);
    webServer.send(200, "application/json", json);
}

void setupDataRoutes()
{
    // ╨Ü╤Ç╨░╤ü╨╕╨▓╨░╤Å ╤ü╤é╤Ç╨░╨╜╨╕╤å╨░ ╨┐╨╛╨║╨░╨╖╨░╨╜╨╕╨╣ ╤ü ╨╕╨║╨╛╨╜╨║╨░╨╝╨╕ (╨╛╤Ç╨╕╨│╨╕╨╜╨░╨╗╤î╨╜╤ï╨╣ ╨┤╨╕╨╖╨░╨╣╨╜)
    webServer.on("/readings", HTTP_GET,
                 []()
                 {
                     logWebRequest("GET", "/readings", webServer.client().remoteIP().toString());

                     if (currentWiFiMode == WiFiMode::AP)
                     {
                         webServer.send(200, "text/html; charset=utf-8", generateApModeUnavailablePage("╨ƒ╨╛╨║╨░╨╖╨░╨╜╨╕╤Å", UI_ICON_DATA));
                         return;
                     }

                     String html = generatePageHeader("╨ƒ╨╛╨║╨░╨╖╨░╨╜╨╕╤Å ╨┤╨░╤é╤ç╨╕╨║╨░", UI_ICON_DATA);
                     html += navHtml();
                     html += "<h1>" UI_ICON_DATA " ╨ƒ╨╛╨║╨░╨╖╨░╨╜╨╕╤Å ╨┤╨░╤é╤ç╨╕╨║╨░</h1>";
                     // ╨ÿ╨╜╤ä╨╛╤Ç╨╝╨░╤å╨╕╨╛╨╜╨╜╨░╤Å ╤ü╤é╤Ç╨╛╨║╨░ ╤ü╨╛╤ü╤é╨╛╤Å╨╜╨╕╤Å
                     html += "<div id='statusInfo' style='margin:10px 0;font-size:16px;color:#333'></div>";
                     // ╨ù╨░╨│╨╛╨╗╨╛╨▓╨╛╨║ 4-╨│╨╛ ╤ü╤é╨╛╨╗╨▒╤å╨░: ╨▓╤ï╨▒╤Ç╨░╨╜╨╜╨░╤Å ╨║╤â╨╗╤î╤é╤â╤Ç╨░ ╨╕╨╗╨╕ ┬½╨á╨╡╨║╨╛╨╝.┬╗
                     String recHeader = "╨á╨╡╨║╨╛╨╝.";
                     if (strlen(config.cropId) > 0)
                     {
                         const char* id = config.cropId;
                         if      (strcmp(id,"tomato")==0)      recHeader = "╨ó╨╛╨╝╨░╤é╤ï";
                         else if (strcmp(id,"cucumber")==0)    recHeader = "╨₧╨│╤â╤Ç╤å╤ï";
                         else if (strcmp(id,"pepper")==0)      recHeader = "╨ƒ╨╡╤Ç╨╡╤å";
                         else if (strcmp(id,"lettuce")==0)     recHeader = "╨í╨░╨╗╨░╤é";
                         else if (strcmp(id,"blueberry")==0)   recHeader = "╨ô╨╛╨╗╤â╨▒╨╕╨║╨░";
                         else if (strcmp(id,"lawn")==0)        recHeader = "╨ô╨░╨╖╨╛╨╜";
                         else if (strcmp(id,"grape")==0)       recHeader = "╨Æ╨╕╨╜╨╛╨│╤Ç╨░╨┤";
                         else if (strcmp(id,"conifer")==0)     recHeader = "╨Ñ╨▓╨╛╨╣╨╜╤ï╨╡";
                         else if (strcmp(id,"strawberry")==0)  recHeader = "╨Ü╨╗╤â╨▒╨╜╨╕╨║╨░";
                         else if (strcmp(id,"apple")==0)       recHeader = "╨»╨▒╨╗╨╛╨╜╤Å";
                         else if (strcmp(id,"pear")==0)        recHeader = "╨ô╤Ç╤â╤ê╨░";
                         else if (strcmp(id,"cherry")==0)      recHeader = "╨Æ╨╕╤ê╨╜╤Å";
                         else if (strcmp(id,"raspberry")==0)   recHeader = "╨£╨░╨╗╨╕╨╜╨░";
                         else if (strcmp(id,"currant")==0)     recHeader = "╨í╨╝╨╛╤Ç╨╛╨┤╨╕╨╜╨░";
                     }

                     html += "<div class='section'><table class='data'><thead><tr><th></th><th>RAW</th><th>╨Ü╨╛╨╝╨┐╨╡╨╜╤ü.</th><th>" + recHeader + "</th></tr></thead><tbody>";
                     html += "<tr><td>≡ƒîí∩╕Å ╨ó╨╡╨╝╨┐╨╡╤Ç╨░╤é╤â╤Ç╨░, ┬░C</td><td><span id='temp_raw'></span></td><td><span id='temp'></span></td><td><span id='temp_rec'></span></td></tr>";
                     html += "<tr><td>≡ƒÆº ╨Æ╨╗╨░╨╢╨╜╨╛╤ü╤é╤î, %</td><td><span id='hum_raw'></span></td><td><span id='hum'></span></td><td><span id='hum_rec'></span></td></tr>";
                     html += "<tr><td>ΓÜí EC, ┬╡S/cm</td><td><span id='ec_raw'></span></td><td><span id='ec'></span></td><td><span id='ec_rec'></span></td></tr>";
                     html += "<tr><td>ΓÜù∩╕Å pH</td><td><span id='ph_raw'></span></td><td><span id='ph'></span></td><td><span id='ph_rec'></span></td></tr>";
                     html += "<tr><td>≡ƒö┤ ╨É╨╖╨╛╤é (N), ╨╝╨│/╨║╨│</td><td><span id='n_raw'></span></td><td><span id='n'></span></td><td><span id='n_rec'></span><span id='n_season' class='season-adj'></span></td></tr>";
                     html += "<tr><td>≡ƒƒí ╨ñ╨╛╤ü╤ä╨╛╤Ç (P), ╨╝╨│/╨║╨│</td><td><span id='p_raw'></span></td><td><span id='p'></span></td><td><span id='p_rec'></span><span id='p_season' class='season-adj'></span></td></tr>";
                     html += "<tr><td>≡ƒö╡ ╨Ü╨░╨╗╨╕╨╣ (K), ╨╝╨│/╨║╨│</td><td><span id='k_raw'></span></td><td><span id='k'></span></td><td><span id='k_rec'></span><span id='k_season' class='season-adj'></span></td></tr>";
                     html += "</tbody></table></div>";
                     html += "<style>";
                     html += ".season-adj { font-size: 0.8em; margin-left: 5px; }";
                     html += ".season-adj.up { color: #2ecc71; }";
                     html += ".season-adj.down { color: #e74c3c; }";
                     html += "</style>";
                     html += "<script>";
                     html += "function set(id,v){if(v!==undefined&&v!==null){document.getElementById(id).textContent=v;}}";
                     html += "function colorDelta(a,b){var diff=Math.abs(a-b)/b*100;if(diff>30)return 'red';if(diff>20)return 'orange';if(diff>10)return 'yellow';return '';}";
                     html += "function colorRange(v,min,max){var span=(max-min);if(span<=0)return '';if(v<min||v>max)return 'red';if(v<min+0.05*span||v>max-0.05*span)return 'orange';if(v<min+0.10*span||v>max-0.10*span)return 'yellow';return '';}";
                     html += "function applyColor(spanId,cls){var el=document.getElementById(spanId);if(!el)return;el.classList.remove('red','orange','yellow','green');if(cls){el.classList.add(cls);}else{el.classList.add('green');}}";
                     html += "var limits={temp:{min:-45,max:115},hum:{min:0,max:100},ec:{min:0,max:10000},ph:{min:3,max:9},n:{min:0,max:1999},p:{min:0,max:1999},k:{min:0,max:1999}};";
                     html += "function updateSensor(){";
                     html += "fetch('/sensor_json').then(r=>r.json()).then(d=>{";
                     html += "set('temp_raw',d.raw_temperature);";
                     html += "set('hum_raw',d.raw_humidity);";
                     html += "set('ec_raw',d.raw_ec);";
                     html += "set('ph_raw',d.raw_ph);";
                     html += "set('n_raw',d.raw_nitrogen);";
                     html += "set('p_raw',d.raw_phosphorus);";
                     html += "set('k_raw',d.raw_potassium);";
                     html += "set('temp_rec',d.rec_temperature);set('hum_rec',d.rec_humidity);set('ec_rec',d.rec_ec);set('ph_rec',d.rec_ph);set('n_rec',d.rec_nitrogen);set('p_rec',d.rec_phosphorus);set('k_rec',d.rec_potassium);";
                     // === Arrow indicators block ===
                     html += "const tol={temp:0.2,hum:0.5,ec:20,ph:0.05,n:5,p:3,k:3};";
                     html += "function arrowSign(base,val,thr){base=parseFloat(base);val=parseFloat(val);if(isNaN(base)||isNaN(val))return '';if(val>base+thr)return 'Γåæ ';if(val<base-thr)return 'Γåô ';return '';};";
                     html += "function showWithArrow(id,sign,value){document.getElementById(id).textContent=sign+value;}";

                     // Compensated vs RAW arrows
                     html += "showWithArrow('temp', arrowSign(d.raw_temperature ,d.temperature ,tol.temp), d.temperature);";
                     html += "showWithArrow('hum',  arrowSign(d.raw_humidity    ,d.humidity    ,tol.hum ), d.humidity);";
                     html += "showWithArrow('ec',   arrowSign(d.raw_ec          ,d.ec          ,tol.ec  ), d.ec);";
                     html += "showWithArrow('ph',   arrowSign(d.raw_ph          ,d.ph          ,tol.ph  ), d.ph);";
                     html += "showWithArrow('n',    arrowSign(d.raw_nitrogen    ,d.nitrogen    ,tol.n   ), d.nitrogen);";
                     html += "showWithArrow('p',    arrowSign(d.raw_phosphorus  ,d.phosphorus  ,tol.p   ), d.phosphorus);";
                     html += "showWithArrow('k',    arrowSign(d.raw_potassium   ,d.potassium   ,tol.k   ), d.potassium);";

                     // Recommendation arrows (target vs current)
                     html += "showWithArrow('temp_rec', arrowSign(d.temperature ,d.rec_temperature ,tol.temp), d.rec_temperature);";
                     html += "showWithArrow('hum_rec',  arrowSign(d.humidity    ,d.rec_humidity    ,tol.hum ), d.rec_humidity);";
                     html += "showWithArrow('ec_rec',   arrowSign(d.ec          ,d.rec_ec          ,tol.ec  ), d.rec_ec);";
                     html += "showWithArrow('ph_rec',   arrowSign(d.ph          ,d.rec_ph          ,tol.ph  ), d.rec_ph);";
                     html += "showWithArrow('n_rec',    arrowSign(d.nitrogen    ,d.rec_nitrogen    ,tol.n   ), d.rec_nitrogen);";
                     html += "showWithArrow('p_rec',    arrowSign(d.phosphorus  ,d.rec_phosphorus  ,tol.p   ), d.rec_phosphorus);";
                     html += "showWithArrow('k_rec',    arrowSign(d.potassium   ,d.rec_potassium   ,tol.k   ), d.rec_potassium);";
                     // === End arrow indicators ===
                     
                     // ╨ö╨╛╨▒╨░╨▓╨╗╤Å╨╡╨╝ ╨╕╨╜╨┤╨╕╨║╨░╤å╨╕╤Ä ╤ü╨╡╨╖╨╛╨╜╨╜╤ï╤à ╨║╨╛╤Ç╤Ç╨╡╨║╤é╨╕╤Ç╨╛╨▓╨╛╨║
                     html += "function updateSeasonalAdjustments(season) {";
                     html += "  const adjustments = {";
                     html += "    '╨Æ╨╡╤ü╨╜╨░': { n: '+20%', p: '+15%', k: '+10%' },";
                     html += "    '╨¢╨╡╤é╨╛': { n: '-10%', p: '+5%', k: '+25%' },";
                     html += "    '╨₧╤ü╨╡╨╜╤î': { n: '-20%', p: '+10%', k: '+15%' },";
                     html += "    '╨ù╨╕╨╝╨░': { n: '-30%', p: '+5%', k: '+5%' }";
                     html += "  };";
                     html += "  const envType = " + String(config.environmentType) + ";";
                     html += "  if(envType === 1) {"; // ╨ó╨╡╨┐╨╗╨╕╤å╨░
                     html += "    adjustments['╨Æ╨╡╤ü╨╜╨░'] = { n: '+25%', p: '+20%', k: '+15%' };";
                     html += "    adjustments['╨¢╨╡╤é╨╛'] = { n: '+10%', p: '+10%', k: '+30%' };";
                     html += "    adjustments['╨₧╤ü╨╡╨╜╤î'] = { n: '+15%', p: '+15%', k: '+20%' };";
                     html += "    adjustments['╨ù╨╕╨╝╨░'] = { n: '+5%', p: '+10%', k: '+15%' };";
                     html += "  }";
                     html += "  const adj = adjustments[season] || { n: '', p: '', k: '' };";
                     html += "  ['n', 'p', 'k'].forEach(elem => {";
                     html += "    const span = document.getElementById(elem + '_season');";
                     html += "    if(span) {";
                     html += "      span.textContent = adj[elem] ? ` (${adj[elem]})` : '';";
                     html += "      span.className = 'season-adj ' + (adj[elem].startsWith('+') ? 'up' : 'down');";
                     html += "    }";
                     html += "  });";
                     html += "}";
                     
                     html += "var invalid = d.irrigation || d.alerts.length>0 || d.humidity<25 || d.temperature<5 || d.temperature>40;";
                     html += "var statusHtml = invalid ? '<span class=\\\"red\\\">╨ö╨░╨╜╨╜╤ï╨╡&nbsp;╨╜╨╡&nbsp;╨▓╨░╨╗╨╕╨┤╨╜╤ï</span>' : '<span class=\\\"green\\\">╨ö╨░╨╜╨╜╤ï╨╡&nbsp;╨▓╨░╨╗╨╕╨┤╨╜╤ï</span>';";
                     html += "var seasonColor={'╨¢╨╡╤é╨╛':'green','╨Æ╨╡╤ü╨╜╨░':'yellow','╨₧╤ü╨╡╨╜╤î':'yellow','╨ù╨╕╨╝╨░':'red','╨¥/╨ö':''}[d.season]||'';";
                     html += "var seasonHtml=seasonColor?(`<span class=\\\"${seasonColor}\\\">${d.season}</span>`):d.season;";
                     html += "document.getElementById('statusInfo').innerHTML=statusHtml+' | ╨í╨╡╨╖╨╛╨╜: '+seasonHtml;";
                     html += "updateSeasonalAdjustments(d.season);";
                     html += "var tvr=parseFloat(d.raw_temperature);applyColor('temp_raw',colorRange(tvr,limits.temp.min,limits.temp.max));";
                     html += "var hvr=parseFloat(d.raw_humidity);applyColor('hum_raw',colorRange(hvr,limits.hum.min,limits.hum.max));";
                     html += "var evr=parseFloat(d.raw_ec);applyColor('ec_raw',colorRange(evr,limits.ec.min,limits.ec.max));";
                     html += "var pvr=parseFloat(d.raw_ph);applyColor('ph_raw',colorRange(pvr,limits.ph.min,limits.ph.max));";
                     html += "var nvr=parseFloat(d.raw_nitrogen);applyColor('n_raw',colorRange(nvr,limits.n.min,limits.n.max));";
                     html += "var p2r=parseFloat(d.raw_phosphorus);applyColor('p_raw',colorRange(p2r,limits.p.min,limits.p.max));";
                     html += "var kvr=parseFloat(d.raw_potassium);applyColor('k_raw',colorRange(kvr,limits.k.min,limits.k.max));";
                     html += "['temp','hum','ec','ph','n','p','k'].forEach(function(id){var el=document.getElementById(id);if(el){el.classList.remove('red','orange','yellow','green');}});";
                     html += "var ct=parseFloat(d.temperature);";
                     html += "var ch=parseFloat(d.humidity);";
                     html += "var ce=parseFloat(d.ec);";
                     html += "var cph=parseFloat(d.ph);";
                     html += "var cn=parseFloat(d.nitrogen);";
                     html += "var cp=parseFloat(d.phosphorus);";
                     html += "var ck=parseFloat(d.potassium);";
                     html += "applyColor('temp_rec', colorDelta(ct, parseFloat(d.rec_temperature)));";
                     html += "applyColor('hum_rec',  colorDelta(ch, parseFloat(d.rec_humidity)));";
                     html += "applyColor('ec_rec',   colorDelta(ce, parseFloat(d.rec_ec)));";
                     html += "applyColor('ph_rec',   colorDelta(cph,parseFloat(d.rec_ph)));";
                     html += "applyColor('n_rec',    colorDelta(cn, parseFloat(d.rec_nitrogen)));";
                     html += "applyColor('p_rec',    colorDelta(cp, parseFloat(d.rec_phosphorus)));";
                     html += "applyColor('k_rec',    colorDelta(ck, parseFloat(d.rec_potassium)));";
                     html += "});";
                     html += "}";
                     html += "setInterval(updateSensor,3000);";
                     html += "updateSensor();";
                     html += "</script>";

                     // ======= ╨Ü╨░╨╗╨╕╨▒╤Ç╨╛╨▓╨║╨░ =======
                     bool csvPresent = CalibrationManager::hasTable(SoilProfile::SAND); // custom.csv

                     html += "<div class='section'><h2>ΓÜÖ∩╕Å ╨Ü╨░╨╗╨╕╨▒╤Ç╨╛╨▓╨║╨░";
                     if (!config.flags.calibrationEnabled) {
                         html += " <span style='font-size:14px;color:#9E9E9E'>(╨▓╤ï╨║╨╗╤Ä╤ç╨╡╨╜╨░)</span>";
                     } else if (csvPresent) {
                         html += " <span style='font-size:14px;color:#4CAF50'>(CSV ╨╖╨░╨│╤Ç╤â╨╢╨╡╨╜)</span>";
                     } else {
                         html += " <span style='font-size:14px;color:#2196F3'>(CSV ╨╜╨╡ ╨╖╨░╨│╤Ç╤â╨╢╨╡╨╜)</span>";
                     }
                     html += "</h2>";
                     // ----- ╨ñ╨╛╤Ç╨╝╨░ ╨╖╨░╨│╤Ç╤â╨╖╨║╨╕ CSV -----
                     html += "<form action='/readings/upload' method='post' enctype='multipart/form-data' style='margin-top:10px'>";
                     html += "<div class='section'><h3>╨ù╨░╨│╤Ç╤â╨╖╨╕╤é╤î CSV</h3><input type='file' name='calibration_csv' accept='.csv' required></div>";
                     html += generateButton(ButtonType::PRIMARY, UI_ICON_UPLOAD, "╨ù╨░╨│╤Ç╤â╨╖╨╕╤é╤î CSV", "");
                     html += "</form>";
                     // ╨Ü╨╜╨╛╨┐╨║╨░ ╤ü╨▒╤Ç╨╛╤ü╨░ CSV, ╨╡╤ü╨╗╨╕ ╤ä╨░╨╣╨╗ ╤ü╤â╤ë╨╡╤ü╤é╨▓╤â╨╡╤é
                     if(csvPresent){
                         html += "<form action='/readings/csv_reset' method='post' style='margin-top:10px'>";
                         html += generateButton(ButtonType::SECONDARY, "≡ƒùæ∩╕Å", "╨ú╨┤╨░╨╗╨╕╤é╤î CSV", "");
                         html += "</form>";
                     }

                     // CSS ╨┤╨╗╤Å ╤é╨░╨▒╨╗╨╕╤å╤ï ╨┤╨░╨╜╨╜╤ï╤à
                     html += "<style>.data{width:100%;border-collapse:collapse}.data th,.data td{border:1px solid #ccc;padding:6px;text-align:center}.data th{background:#f5f5f5}.green{color:#4CAF50}.yellow{color:#FFC107}.orange{color:#FF9800}.red{color:#F44336}</style>";

                     // API-╤ü╤ü╤ï╨╗╨║╨░ ╨▓╨╜╨╕╨╖╤â ╤ü╤é╤Ç╨░╨╜╨╕╤å╤ï
                     html += "<div style='margin-top:15px;font-size:14px;color:#555'><b>API:</b> <a href='" + String(API_SENSOR) + "' target='_blank'>" + String(API_SENSOR) + "</a> (JSON, +timestamp)</div>";
                     html += generatePageFooter();
                     webServer.send(200, "text/html; charset=utf-8", html);
                 });

    // AJAX ╤ì╨╜╨┤╨┐╨╛╨╕╨╜╤é ╨┤╨╗╤Å ╨╛╨▒╨╜╨╛╨▓╨╗╨╡╨╜╨╕╤Å ╨┐╨╛╨║╨░╨╖╨░╨╜╨╕╨╣
    webServer.on("/sensor_json", HTTP_GET, sendSensorJson);

    // Primary API v1 endpoint
    webServer.on(API_SENSOR, HTTP_GET, sendSensorJson);

    // ╨ù╨░╨│╤Ç╤â╨╖╨║╨░ ╨║╨░╨╗╨╕╨▒╤Ç╨╛╨▓╨╛╤ç╨╜╨╛╨│╨╛ CSV ╤ç╨╡╤Ç╨╡╨╖ ╨▓╨║╨╗╨░╨┤╨║╤â
    webServer.on("/readings/upload", HTTP_POST, [](){}, handleReadingsUpload);

    // ╨í╨▒╤Ç╨╛╤ü ╨┐╨╛╨╗╤î╨╖╨╛╨▓╨░╤é╨╡╨╗╤î╤ü╨║╨╕╤à CSV (╤â╨┤╨░╨╗╤Å╨╡╨╝ ╨▓╤ü╨╡ *.csv)
    webServer.on("/readings/csv_reset", HTTP_POST,
                 []() {
                     logWebRequest("POST","/readings/csv_reset", webServer.client().remoteIP().toString());
                     CalibrationManager::init();
                     bool removed = CalibrationManager::deleteTable(SoilProfile::SAND);
                     String toast = removed?"CSV+╤â╨┤╨░╨╗╨╡╨╜":"CSV+╨╜╨╡+╨╜╨░╨╣╨┤╨╡╨╜";
                     webServer.sendHeader("Location", String("/readings?toast=") + toast, true);
                     webServer.send(302,"text/plain","Redirect");
                 });

    // ╨ñ╨╛╤Ç╨╝╨░ ╨┤╨╗╤Å ╤ü╨╛╤à╤Ç╨░╨╜╨╡╨╜╨╕╤Å ╨┐╤Ç╨╛╤ä╨╕╨╗╤Å
    webServer.on("/readings/profile", HTTP_POST, [](){}, handleProfileSave);

    // Deprecated alias ╤â╨┤╨░╨╗╤æ╨╜ ╨▓ v2.7.0

    logDebug("╨£╨░╤Ç╤ê╤Ç╤â╤é╤ï ╨┤╨░╨╜╨╜╤ï╤à ╨╜╨░╤ü╤é╤Ç╨╛╨╡╨╜╤ï: /readings, /api/v1/sensor (json), /sensor_json [legacy]");
}

// ╨Æ╤ü╨┐╨╛╨╝╨╛╨│╨░╤é╨╡╨╗╤î╨╜╨░╤Å ╤ä╤â╨╜╨║╤å╨╕╤Å ╨┤╨╗╤Å ╨┐╨╛╨╗╤â╤ç╨╡╨╜╨╕╤Å SSID ╤é╨╛╤ç╨║╨╕ ╨┤╨╛╤ü╤é╤â╨┐╨░
extern String getApSsid();
