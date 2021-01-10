#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <LiquidCrystal_I2C.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <WiFiClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h> 

#define SS_PIN D8
#define RST_PIN D0

#define PASSW_MEM 0
#define MYSQL_SERVER 20
#define MYSQL_USER 40
#define MYSQL_PASSWD 60
#define SSID_MEM 80
#define SSID_PASSWD_MEM 100

#define LENGTH_PASSW_MEM 120
#define LENGTH_MYSQL_SERVER 125
#define LENGTH_MYSQL_USER 130
#define LENGTH_MYSQL_PASSWD 135
#define LENGTH_SSID_MEM 140
#define LENGTH_SSIDPASSWD_MEM 145

AsyncWebServer server(80);

WiFiClient client;
MySQL_Connection conn((Client *)&client);


LiquidCrystal_I2C lcd(0x27,16,2);

MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;
byte nuidPICC[4];

String content= "";

char INSERT_SQL[] = "INSERT INTO prova.lectura_rfid(lectura,data) VALUES (\"%s\",CURTIME())";
char query[128];


String s = "";
int token_int;

const char* PARAM_INPUT_1 = "input1";
const char* PARAM_INPUT_2 = "input2";
const char* PARAM_INPUT_3 = "input3";



const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
  <head>
  <title>Config del lector</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <br>
  <div style="text-align: center"><img src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAWIAAABaCAMAAAC13Kw8AAADJGlUWHRYTUw6Y29tLmFkb2JlLnhtcAAAAAAAPD94cGFja2V0IGJlZ2luPSLvu78iIGlkPSJXNU0wTXBDZWhpSHpyZVN6TlRjemtjOWQiPz4gPHg6eG1wbWV0YSB4bWxuczp4PSJhZG9iZTpuczptZXRhLyIgeDp4bXB0az0iQWRvYmUgWE1QIENvcmUgNS4zLWMwMTEgNjYuMTQ1NjYxLCAyMDEyLzAyLzA2LTE0OjU2OjI3ICAgICAgICAiPiA8cmRmOlJERiB4bWxuczpyZGY9Imh0dHA6Ly93d3cudzMub3JnLzE5OTkvMDIvMjItcmRmLXN5bnRheC1ucyMiPiA8cmRmOkRlc2NyaXB0aW9uIHJkZjphYm91dD0iIiB4bWxuczp4bXA9Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC8iIHhtbG5zOnhtcE1NPSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvbW0vIiB4bWxuczpzdFJlZj0iaHR0cDovL25zLmFkb2JlLmNvbS94YXAvMS4wL3NUeXBlL1Jlc291cmNlUmVmIyIgeG1wOkNyZWF0b3JUb29sPSJBZG9iZSBQaG90b3Nob3AgQ1M2IChNYWNpbnRvc2gpIiB4bXBNTTpJbnN0YW5jZUlEPSJ4bXAuaWlkOkYwNENGNUI0Q0M4MTExRTdCQTZEOTlGQzRERDI1RTlFIiB4bXBNTTpEb2N1bWVudElEPSJ4bXAuZGlkOkYwNENGNUI1Q0M4MTExRTdCQTZEOTlGQzRERDI1RTlFIj4gPHhtcE1NOkRlcml2ZWRGcm9tIHN0UmVmOmluc3RhbmNlSUQ9InhtcC5paWQ6RjA0Q0Y1QjJDQzgxMTFFN0JBNkQ5OUZDNEREMjVFOUUiIHN0UmVmOmRvY3VtZW50SUQ9InhtcC5kaWQ6RjA0Q0Y1QjNDQzgxMTFFN0JBNkQ5OUZDNEREMjVFOUUiLz4gPC9yZGY6RGVzY3JpcHRpb24+IDwvcmRmOlJERj4gPC94OnhtcG1ldGE+IDw/eHBhY2tldCBlbmQ9InIiPz5zUH1VAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAAuhQTFRFR3BM+DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE++DE+V3aBHQAAAPd0Uk5TAJDAMPBQ0OCwEGAC/QH+QAwDBPH6mOgGCfv0DuzRCPxr9aggKOkbaNhU9pMP96MK5x7kzB9GZy+AEd8UwUVNJJ3mGdS4SRIWZI0LlovETkygvm7bB6cuyLQXoV7D0z6aN7smW5xi7UrXDc0saqKu1dx9b1xIfHFhBc978sopIzgcSyKrHYFpr/lYWdI2ZZ4q5euHMtZt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"></div><br>
  <br><br>
  <div style="text-align: center; border-style: inset; padding: 30px; margin: 20px;">
  <h2 style="text-align: center">Introduix password</h2><br> 
  <form action="/post" method="post">
    Password: <input type="password" name="input1">
    <input type="submit" value="Envia">
  </form><br>
  </div>
</body></html>)rawliteral";

void reset_disp() {

      escriu_rom_int(12,LENGTH_MYSQL_SERVER);
      escriu_rom_int(4,LENGTH_MYSQL_USER);
      escriu_rom_int(4,LENGTH_MYSQL_PASSWD);
      escriu_rom_int(5,LENGTH_PASSW_MEM);
      escriu_rom_int(12,LENGTH_SSID_MEM);
      escriu_rom_int(19,LENGTH_SSIDPASSWD_MEM);

  
      escriu_rom("192.168.0.32",MYSQL_SERVER, 12);
      escriu_rom("root",MYSQL_USER, 4);
      escriu_rom("root",MYSQL_PASSWD, 4);
      escriu_rom("admin",PASSW_MEM, 5);
      escriu_rom("CEFIRE_CONFIG",SSID_MEM, 12);
      escriu_rom("CEFIREVALENCIACONF",SSID_PASSWD_MEM, 19);
    
}

String config_page() {

  int my_server_len = llig_rom_int(LENGTH_MYSQL_SERVER);
  int my_user_len = llig_rom_int(LENGTH_MYSQL_USER);
  int my_passwd_len = llig_rom_int(LENGTH_MYSQL_PASSWD);
  int disp_passwd_len = llig_rom_int(LENGTH_PASSW_MEM);
  int disp_ssid_len = llig_rom_int(LENGTH_SSID_MEM );
  int disp_ssid_passwd_len = llig_rom_int(LENGTH_SSIDPASSWD_MEM);

  Serial.println(my_server_len);
  Serial.println(my_user_len);
  Serial.println(my_passwd_len);

  String my_server = llig_rom(MYSQL_SERVER, my_server_len);
  String my_user = llig_rom(MYSQL_USER, my_user_len);
  String my_passwd = llig_rom(MYSQL_PASSWD, my_passwd_len);
  String disp_passwd = llig_rom(PASSW_MEM, disp_passwd_len);
  String disp_ssid = llig_rom(SSID_MEM, disp_ssid_len);
  String disp_ssid_passwd = llig_rom(SSID_PASSWD_MEM, disp_ssid_passwd_len);

  
           

  Serial.println(my_server+" "+my_user+" "+my_passwd+" "+disp_ssid);
  
  token_int = random(1, 1000000000);
  
  s = String(token_int);
  
  String config_html = "<!DOCTYPE HTML><html><head>  <title>Settings<\/title>  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
  "<\/head><body><h2 style=\"text-align: center\">Settings<\/h2><br>"  
  "<div style=\"text-align: center; border-style: inset; padding: 30px; margin: 20px;\">"
  "<div>La llargaria maxima de cadascun dels valors es de 16 caracters<\/div><br>"
  "<form action=\"\/canvi\" method=\"post\">"
  "<br>"
  "<div>Mysql Server:<br> <input type=\"text\" name=\"mysql\" value=\""+my_server+"\"><\/div>"    
  "<div>Usuari mysql:<br> <input type=\"text\" name=\"usuari_mysql\" value=\""+my_user+"\"><\/div>"
  "<div>Password mysql:<br> <input type=\"password\" name=\"password_mysql\" value=\""+my_passwd+"\"><\/div>"
  "<div>Canviar password:<br> <input type=\"text\" name=\"password_nou\" value=\""+disp_passwd+"\"><\/div>"
  "<div>SSID Wifi:<br> <input type=\"text\" name=\"ssid_wifi\" value=\""+disp_ssid+"\"><\/div>"
  "<div>Password wifi:<br> <input type=\"password\" name=\"password_wifi\" value=\""+disp_ssid_passwd+"\"><\/div>"
  "<div><input type=\"token\" name=\"token\" value=\"" + String(token_int) + "\" hidden><\/div>"
  "<br><div><input type=\"submit\" value=\"Submit\"><\/div><\/form>"  
  "<br>  <\/div><\/div><\/body><\/html>";
  Serial.println("token: "+s);
  return config_html;
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "No trobat");
}


void escriu_rom(String sample,int adr_mem, int llargaria)
{ 

  for(int i=0;i<sample.length();i++)
  {
    EEPROM.write(adr_mem+i, sample[i]); //Escriu a la ROM de la placa
  }
  EEPROM.commit();    //Guarda a la EEPROM
}

String llig_rom(int addr_mem, int llargaria)
{ 
  //Llegir cadenes de la ROM
  String strText = "";
  for(int i=0;i<llargaria;i++)
  {
    strText = strText + char(EEPROM.read(addr_mem+i)); //0x0F
  }
  return strText;
}

void escriu_rom_int(int sample,int adr_mem)
{ 

  EEPROM.write(adr_mem, sample >> 8);
  EEPROM.write(adr_mem + 1, sample & 0xFF);
  EEPROM.commit();    //Guarda a la EEPROM
}

int llig_rom_int(int addr_mem)
{ 
  byte byte1 = EEPROM.read(addr_mem);
  byte byte2 = EEPROM.read(addr_mem + 1);
  return (byte1 << 8) + byte2;
}

void intro_lcd () {
  lcd.clear();
  lcd.setCursor(0, 0);
  // Escribimos el Mensaje en el LCD.
  lcd.print("CEFIRE VALENCIA");
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

//Funció de reset

void(* resetFunc) (void) = 0;

/*
 * SETUP del projecte.
 * 
 */


void setup() {

  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  lcd.init();
  //Encender la luz de fondo.
  lcd.backlight(); //lcd.noBacklight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Proyecto CEEDCV");
  delay(1500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Inicialitzant...");
  WiFi.mode(WIFI_STA);

  int llarg_ssid=llig_rom_int(LENGTH_SSID_MEM);
  int llarg_wifi_passwd=llig_rom_int(LENGTH_SSIDPASSWD_MEM);

  char ssid[llarg_ssid];
  char password[llarg_wifi_passwd];
  
  llig_rom(SSID_MEM,llarg_ssid).toCharArray(ssid, llarg_ssid+1);
  llig_rom(SSID_PASSWD_MEM,llarg_wifi_passwd).toCharArray(password, llarg_wifi_passwd+1);
  
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Ha fallat WIFI!");
    lcd.print("Ha fallat WIFI");
    return;
  }
  
  EEPROM.begin(512);  //Initialize EEPROM

  //Estem resetejant el dispositiu cada vegada que reiniciem als valors indicats. Anem a posar un botó de reset amb un if per a resetejar manualment.
  //reset_disp(); //ja no

  int llarg_user_mysql=llig_rom_int(LENGTH_MYSQL_USER);
  int llarg_passwd_mysql=llig_rom_int(LENGTH_MYSQL_PASSWD);
  int llarg_mysql_server=llig_rom_int(LENGTH_MYSQL_SERVER);

  char user_mysql[llarg_user_mysql];
  char pass_mysql[llarg_passwd_mysql];

  llig_rom(MYSQL_USER,llarg_user_mysql).toCharArray(user_mysql, llarg_user_mysql+1);
  llig_rom(MYSQL_PASSWD,llarg_passwd_mysql).toCharArray(pass_mysql, llarg_passwd_mysql+1);

  String server_mysql_aux = llig_rom(MYSQL_SERVER,llarg_mysql_server);

  int server_addr_arr[4];

  for (byte i = 0; i < 4; i++) {
    server_addr_arr[i]=getValue(server_mysql_aux, '.', i).toInt();
  }  
  Serial.println();
  Serial.println(server_addr_arr[0]);
  Serial.println(server_addr_arr[1]);
  Serial.println(server_addr_arr[2]);
  Serial.println(server_addr_arr[3]);
  Serial.println(user_mysql);
  Serial.println(pass_mysql);

  IPAddress server_addr(server_addr_arr[0], server_addr_arr[1] , server_addr_arr[2], server_addr_arr[3]);

  int intentos = 0;
  while (conn.connect(server_addr, 3306, user_mysql, pass_mysql) != true && intentos < 2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Error database");
    lcd.setCursor(0, 1);
    lcd.print("Espera...");
    Serial.println ( "Error en conexió a la base de dades");
    delay(2000);
    intentos++;
  }


  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  delay(100);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Adreca IP:");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CEFIRE VALENCIA");
  
  lcd.noBacklight();
  
  // Envia pàgina al client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });


  
 //SERVIDOR DE PARÀMETRES  
  //Este es canviarà per un més segur, un post o un get o amb un token en el GET
  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      resetFunc();
    });
  
  server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request)
    {
      
      String inputMessage2;

      inputMessage2 = request->getParam(0)->value();
      int passwd_len=llig_rom_int(LENGTH_PASSW_MEM);
      String pass = llig_rom(PASSW_MEM,passwd_len);
      Serial.println(pass+" "+inputMessage2);

      if (inputMessage2 == pass) {
        request->send_P(200, "text/html", config_page().c_str());
      } else {
        request->send(200, "text/html", "Password incorrecte");
      }
    });

    
  server.on("/canvi", HTTP_POST, [](AsyncWebServerRequest *request)
    {

      String input_mysql = request->getParam(0)->value();
      String input_usr_msql = request->getParam(1)->value();
      String input_pass_mysql = request->getParam(2)->value();
      String input_paswd_nou = request->getParam(3)->value();
      String input_ssid = request->getParam(4)->value();
      String input_paswd_wifi = request->getParam(5)->value();
      String input_token = request->getParam(6)->value();
      Serial.println(input_mysql);
      Serial.println(input_usr_msql);
      Serial.println(input_pass_mysql);
      Serial.println(input_paswd_nou);
      Serial.println(input_ssid);
      Serial.println(input_paswd_wifi);
      Serial.println(input_token);

      
      if (input_mysql.length()>16 || input_usr_msql.length()>16 || input_pass_mysql.length()>16 || input_paswd_nou.length()>16 || input_ssid.length()>16 || input_paswd_wifi.length()>32 ) {
          request->send(200, "text/html", "Alguna de les cadenes que has introduit es massa llarga");
      } else {
          if (input_token == s) {
              Serial.println(input_mysql.length());
              Serial.println(input_usr_msql.length());
              Serial.println(input_pass_mysql.length());
              Serial.println(input_paswd_nou.length());
              Serial.println(input_ssid.length());
              Serial.println(input_paswd_wifi.length());
              Serial.println(input_token.length());
            
            escriu_rom_int(input_paswd_nou.length(),LENGTH_PASSW_MEM);
            escriu_rom_int(input_mysql.length(),LENGTH_MYSQL_SERVER);
            escriu_rom_int(input_usr_msql.length(),LENGTH_MYSQL_USER);
            escriu_rom_int(input_pass_mysql.length(),LENGTH_MYSQL_PASSWD);
            escriu_rom_int(input_ssid.length(),LENGTH_SSID_MEM);
            escriu_rom_int(input_paswd_wifi.length(),LENGTH_SSIDPASSWD_MEM);

        
            escriu_rom(input_mysql,MYSQL_SERVER, input_mysql.length());
            escriu_rom(input_usr_msql,MYSQL_USER, input_usr_msql.length());
            escriu_rom(input_pass_mysql,MYSQL_PASSWD, input_pass_mysql.length());
            if(input_paswd_nou.length() !=0){
              escriu_rom(input_paswd_nou,PASSW_MEM, input_paswd_nou.length());
            }
            escriu_rom(input_ssid,SSID_MEM, input_ssid.length());
            escriu_rom(input_paswd_wifi,SSID_PASSWD_MEM, input_paswd_wifi.length());

            request->send(200, "text/html", "<div style=\"text-align: center\"><h1>Has actualitzat les dades</h1><br><a href=\"/reset\">Reset per a aplicar les noves dades</a></div>");
          } else {
            request->send(200, "text/html", "El token no es correcte");
          }
      }
    });
  server.onNotFound(notFound);
  server.begin();
}



//
//
void loop() {
  // Noves targetes
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verifica que s'ha llegit el NUID
  if ( ! rfid.PICC_ReadCardSerial())
    return;

    // Guarda NUID
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }

    
    content= "";
    lcd.setCursor(0, 1);

    for (byte i = 0; i < rfid.uid.size; i++) 
    {
       content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
       content.concat(String(rfid.uid.uidByte[i], HEX));
    }
    Serial.print(content);
    lcd.print("Llegit");
    sprintf(query, INSERT_SQL, content.c_str());
    Serial.println("Escrivint en base de dades");
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    cur_mem->execute(query);
    delete cur_mem;
    lcd.backlight(); //lcd.noBacklight();
    delay(750);
    lcd.noBacklight();

    rfid.PICC_HaltA();
  

    rfid.PCD_StopCrypto1();
    intro_lcd();

  
}
