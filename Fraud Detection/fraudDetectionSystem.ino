#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
const char* ssid = "******"; //replace with your SSID
const char* password = "********"; //replace with your password

#define BOTtoken "**************************************"  
#define CHAT_ID "************"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int bot_delay = 1000;
unsigned long lastTimeBotRan;


const int ledPin = 2;
bool ledState = LOW;


const int alertPin = 27;
bool fraud_detected = false;


// Indicates when fraud is detected 
void IRAM_ATTR detectFraud()
{
  //Serial.println("MOTION DETECTED!!!");
  fraud_detected = true;
}


// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) 
{
  Serial.println("Handling New Message");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++)
  {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID)
    {
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
     // Print the received message
    String user_text = bot.messages[i].text;
    Serial.println(user_text);
    String your_name = bot.messages[i].from_name;

    if (user_text == "/start") 
    {
      String welcome = "Welcome, " + your_name + ".\n";
      welcome += "Use the following commands to control your system.\n\n";
      welcome += "Send /alert_security_staff to indicate emergency situation \n";
      welcome += "Send /declare_normal_state to end an emergency situation \n";
      welcome += "Send /get_state to see current system state \n";
      bot.sendMessage(chat_id, welcome, "");
    }
    if (user_text == "/declare_normal_state") 
    {
      bot.sendMessage(chat_id, "Declare Normal Situation", "");
      ledState = LOW;
      digitalWrite(ledPin, ledState);
    }
    
    if (user_text == "/alert_security_staff")
    {
      bot.sendMessage(chat_id, "Handling an Emergency Situation", "");
      ledState = HIGH;
      digitalWrite(ledPin, ledState);
    }
    if (user_text == "/get_state")
    {
      if (digitalRead(ledPin))
      {
        bot.sendMessage(chat_id, "EMERGENCY", "");
      }
      else
      {
        bot.sendMessage(chat_id, "NORMAL", "");
      }
    }
  }
}


void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
  // alertPin mode INPUT_PULLUP
  pinMode(alertPin, INPUT_PULLUP);
  // Set alertPin as interrupt, assign interrupt function and set RISING mode
  attachInterrupt(digitalPinToInterrupt(alertPin), detectFraud, RISING);
  
 // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); 
  
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  bot.sendMessage(CHAT_ID, "Bot started up", "");
}

void loop()
{
  // put your main code here, to run repeatedly:
  if(fraud_detected)
  {
  bot.sendMessage(CHAT_ID, "Fraud detected, be careful someone has tried to access the facility three times in a row without success !!", "");
  Serial.println("Fraud Detected");
  fraud_detected = false;
  
  }
  if (millis() > lastTimeBotRan + bot_delay) 
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  
    while(numNewMessages)
    {
      Serial.println("Got Response!");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
