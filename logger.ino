void logSensorsToSerial() {
    if (updateLocalTime()) {
        // Date \t Time \t Temp \t Luminosity \t Fan speed \n
        LOG.print("[LOGGER]");
        LOG.print(currentTimeFormatted);
        LOG.print(LOG_SEPARATOR);
        LOG.print(insideTemperature);
        LOG.print(LOG_SEPARATOR);
        LOG.print(insideLuminosity);
        LOG.print(LOG_SEPARATOR);
        LOG.print(fanSpeed);
        LOG.print(LOG_END_OF_LINE);
        // DEBUG.println(F("Sensors successfully logged in.."));  
    }
    else {
        DEBUG.println(F("Error updating time."));
        getTimeFromNtpServer();
    }
}

void getCommandsFromSerial() {
    if (Serial.available()) {
        String received = Serial.readStringUntil('\n');
        DEBUG.print(F("[LOG] Received data from Serial... "));
        // DEBUG.print(received);

        // Parse file
        int cmdIndexStart = received.indexOf("[") + 1;
        int cmdIndexEnd = received.indexOf("]", cmdIndexStart);

        int param1IndexStart = received.indexOf("[", cmdIndexEnd + 1) + 1;
        int param1IndexEnd = received.indexOf("]", param1IndexStart);

        int param2IndexStart = received.indexOf("[", param1IndexEnd + 1) + 1;
        int param2IndexEnd = received.indexOf("]", param2IndexStart);

        if (cmdIndexStart == -1 || cmdIndexEnd == -1 || 
            param1IndexStart == -1 || param1IndexEnd == -1 ||
            param2IndexStart == -1 || param2IndexEnd == -1) {
            DEBUG.println(F("Invalid command!"));
            return;
        }
        
        String cmd = received.substring(cmdIndexStart,cmdIndexEnd);
        String param1 = received.substring(param1IndexStart,param1IndexEnd);
        String param2 = received.substring(param2IndexStart,param2IndexEnd);

        DEBUG.print("CMD: "); DEBUG.print(cmd);
        DEBUG.print(" PARAM1: "); DEBUG.print(param1);
        DEBUG.print(" PARAM2: "); DEBUG.println(param2);

        // Sensors
        if (cmd.equals("SENSORS")) {
            if (param1.equals("ENABLE")) {
                unsigned long newInterval = param2.toInt();
                if (isnan(newInterval)) {
                    DEBUG.println("Invalid interval!");
                }
                else {
                    threadUpdateSensorsData.enabled = true;
                    threadUpdateSensorsData.setInterval(newInterval);
                    DEBUG.print(F("Sensors update thread enabled with "));
                    DEBUG.println(newInterval);     
                }
            }
            else if (param1.equals("DISABLE")) {
                threadUpdateSensorsData.enabled = false;
                DEBUG.println(F("Sensors update thread disabled!"));
            }
            else {
                DEBUG.println("Invalid command!");
            }
        }

        // ThingSpeak
        if (cmd.equals("THINGSPEAK")) {
            if (param1.equals("ENABLE")) {
                unsigned long newInterval = param2.toInt();
                if (isnan(newInterval)) {
                    DEBUG.println("Invalid interval!");
                }
                else {
                    threadUpdateThingSpeakFields.enabled = true;
                    threadUpdateThingSpeakFields.setInterval(newInterval);
                    DEBUG.print(F("ThingSpeak thread enabled with "));
                    DEBUG.println(newInterval);     
                }
            }
            else if (param1.equals("DISABLE")) {
                threadUpdateThingSpeakFields.enabled = false;
                DEBUG.println(F("ThingSpeak thread disabled!"));
            }
            else {
                DEBUG.println("Invalid command!");
            }
        }

        // TalkBack
        if (cmd.equals("TALKBACK")) {
            if (param1.equals("ENABLE")) {
                unsigned long newInterval = param2.toInt();
                if (isnan(newInterval)) {
                    DEBUG.println("Invalid interval!");
                }
                else {
                    threadCheckForCommand.enabled = true;
                    threadCheckForCommand.setInterval(newInterval);
                    DEBUG.print(F("TalkBack thread enabled with "));
                    DEBUG.println(newInterval);     
                }
            }
            else if (param1.equals("DISABLE")) {
                threadCheckForCommand.enabled = false;
                DEBUG.println(F("TalkBack thread disabled!"));
            }
            else {
                DEBUG.println("Invalid command!");
            }
        }

        // Serial Logger
        if (cmd.equals("LOGGER")) {
            if (param1.equals("ENABLE")) {
                unsigned long newInterval = param2.toInt();
                if (isnan(newInterval)) {
                    DEBUG.println("Invalid interval!");
                }
                else {
                    threadLogSensorsToSerial.enabled = true;
                    threadLogSensorsToSerial.setInterval(newInterval);
                    DEBUG.print(F("Serial Logger thread enabled with "));
                    DEBUG.println(newInterval);     
                }
            }
            else if (param1.equals("DISABLE")) {
                threadLogSensorsToSerial.enabled = false;
                DEBUG.println(F("Serial Logger thread disabled!"));
            }
            else {
                DEBUG.println("Invalid command!");
            }
        }

        // SD logger
        if (cmd.equals("SD")) {
            if (param1.equals("ENABLE")) {
                unsigned long newInterval = param2.toInt();
                if (isnan(newInterval)) {
                    DEBUG.println("Invalid interval!");
                }
                else {
                    threadLogDataToSD.enabled = true;
                    threadLogDataToSD.setInterval(newInterval);
                    DEBUG.print(F("SD logger thread enabled with "));
                    DEBUG.println(newInterval);     
                }
            }
            else if (param1.equals("DISABLE")) {
                threadLogDataToSD.enabled = false;
                DEBUG.println(F("SD logger thread disabled!"));
            }
            else {
                DEBUG.println("Invalid command!");
            }
        }

        // Read Serial
        if (cmd.equals("SERIAL")) {
            if (param1.equals("ENABLE")) {
                unsigned long newInterval = param2.toInt();
                if (isnan(newInterval)) {
                    DEBUG.println("Invalid interval!");
                }
                else {
                    threadGetCommandsFromSerial.enabled = true;
                    threadGetCommandsFromSerial.setInterval(newInterval);
                    DEBUG.print(F("Read Serial thread enabled with "));
                    DEBUG.println(newInterval);     
                }
            }
            else if (param1.equals("DISABLE")) {
                threadGetCommandsFromSerial.enabled = false;
                DEBUG.println(F("Read Serial thread disabled!"));
            }
            else {
                DEBUG.println("Invalid command!");
            }
        }

        // Heart Beat Led
        if (cmd.equals("LED")) {
            if (param1.equals("ENABLE")) {
                unsigned long newInterval = param2.toInt();
                if (isnan(newInterval)) {
                    DEBUG.println("Invalid interval!");
                }
                else {
                    threadHeartBeat.enabled = true;
                    threadHeartBeat.setInterval(newInterval);
                    DEBUG.print(F("Heart Beat Led thread enabled with "));
                    DEBUG.println(newInterval);     
                }
            }
            else if (param1.equals("DISABLE")) {
                threadHeartBeat.enabled = false;
                DEBUG.println(F("Heart Beat Led thread disabled!"));
            }
            else {
                DEBUG.println("Invalid command!");
            }
        }
    }
}
