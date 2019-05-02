#include "config.h"

void checkForCommand() {
	DEBUG.print(F("[TALKBACK] Checking for new commands... "));
    if (wifiConnect()) {
        // Create the TalkBack URI
        String tbURIPost = String("/talkbacks/");
        tbURIPost += String(myTalkBackID);
        tbURIPost += String("/commands/execute");
        
        // Create the message body for the POST out of the values
        String postMessage =  String("apikey=") + String(myTalkbackKey); 

        // Make a string for any commands that might be in the queue
        String newCommand = String();  

        // Make the POST to ThingSpeak
        int statusCode = httpRequest("POST ", tbURIPost, postMessage, newCommand);
        client.stop();

        // Check the result
        if(statusCode == 200){
        // DEBUG.print("checking queue..."); 
        // check for a command returned from Talkback
        if(newCommand.length() != 0){
            DEBUG.print("  Latest command from queue: ");
            DEBUG.println(newCommand);

            if (newCommand == "FAN_0")
                fanSpeed = 0;
            else if (newCommand == "FAN_25")
                fanSpeed = 25;
            else if (newCommand == "FAN_50")
                fanSpeed = 50;
            else if (newCommand == "FAN_75")
                fanSpeed = 75;
            else if (newCommand == "FAN_100")
                fanSpeed = 100;

            setFanSpeed(fanSpeed);
        }
        else{
            DEBUG.println("Nothing new.");  
        }

        }
        else{
            DEBUG.println("Problem checking queue. HTTP error code " + String(statusCode));
        }
    }
}

int deleteAllTalbackCommands() {
    
	DEBUG.print(F("[TALKBACK] Deleting all commands of fifo... "));
    if (wifiConnect()) {
        // Create the TalkBack URI
        String tbURIDelete = String("/talkbacks/");
        tbURIDelete += String(myTalkBackID);
        tbURIDelete += String("/commands");
        
        // Create the message body for the POST out of the values
        String postMessage =  String("apikey=") + String(myTalkbackKey);

        // Make a string for any commands that might be in the queue
        String newCommand = String();

        // Make the POST to ThingSpeak
        int statusCode = httpRequest("DELETE ", tbURIDelete, postMessage, newCommand);
        client.stop();

        // Check the result
        if(statusCode == 200){
            DEBUG.println("Commands deleted successfull"); 
        }
        else{
            DEBUG.println("Problem checking queue. HTTP error code " + String(statusCode));
        }  
    }
}
                             
// General function to send a request to ThingSpeak
int httpRequest(String method, String uri, String postMessage, String &response){

  bool connectSuccess = false;
  connectSuccess = client.connect("api.thingspeak.com",80);

  if(!connectSuccess){
      return -301;   
  }
  
  postMessage += "&headers=false";
  
  String Headers =  method + uri + String(" HTTP/1.1\r\n") +
                    String("Host: api.thingspeak.com\r\n") +
                    String("Content-Type: application/x-www-form-urlencoded\r\n") +
                    String("Connection: close\r\n") +
                    String("Content-Length: ") + String(postMessage.length()) +
                    String("\r\n\r\n");

  client.print(Headers);
  client.print(postMessage);

  long startWaitForResponseAt = millis();
  while(client.available() == 0 && millis() - startWaitForResponseAt < WAIT_RESPONSE_INTERVAL){
      delay(50);
  }

  if(client.available() == 0){       
    return -304; // Didn't get server response in time
  }

  if(!client.find(const_cast<char *>("HTTP/1.1"))){
      return -303; // Couldn't parse response (didn't find HTTP/1.1)
  }
  
  int status = client.parseInt();
  if(status != 200){
    return status;
  }

  if(!client.find(const_cast<char *>("\n\r\n"))){
    return -303;
  }

  String tempString = String(client.readString());
  response = tempString;
  
  return status;
}