bool initSDCard() {
    DEBUG.print(F("[SD] Initializing the SD Card... "));
    int attempts = 0;
    bool cardMounted = false;
    do {
        cardMounted = SD.begin();
        if(!cardMounted){
            // DEBUG.println(F("Card Mount Failed"));
            delay(SD_INIT_ATTEMPTS_INTERVAL);
        }
    } while(attempts++ < SD_INIT_ATTEMPTS);

    if (!cardMounted) {
        DEBUG.println(F(" FAIL!"));
        return false;
    }

    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        DEBUG.println(F("No SD card attached"));
        return false;
    }

    // DEBUG.print(F("SD Card Type: "));
    // if(cardType == CARD_MMC){
    //     DEBUG.println(F("MMC"));
    // } else if(cardType == CARD_SD){
    //     DEBUG.println(F("SDSC"));
    // } else if(cardType == CARD_SDHC){
    //     DEBUG.println(F("SDHC"));
    // } else {
    //     DEBUG.println(F("UNKNOWN"));
    // }

    // uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    // DEBUG.printf("SD Card Size: %lluMB\n", cardSize);
    
    DEBUG.println(F("OK!"));
    return true;
}

int listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    DEBUG.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        DEBUG.println(F("Failed to open directory"));
        return 1;
    }
    if(!root.isDirectory()){
        DEBUG.println(F("Not a directory"));
        return 2;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            DEBUG.print(F("  DIR : "));
            DEBUG.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            DEBUG.print(F("  FILE: "));
            DEBUG.print(file.name());
            DEBUG.print(F("  SIZE: "));
            DEBUG.println(file.size());
        }
        file = root.openNextFile();
    }
    return 0;
}

int createDir(fs::FS &fs, const char * path){
    DEBUG.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path)){
        DEBUG.println(F("Dir created"));
    } else {
        DEBUG.println(F("mkdir failed"));
        return 1;
    }
    return 0;
}

int removeDir(fs::FS &fs, const char * path){
    DEBUG.printf("Removing Dir: %s\n", path);
    if(fs.rmdir(path)){
        DEBUG.println(F("Dir removed"));
    } else {
        DEBUG.println(F("rmdir failed"));
        return 1;
    }
    return 0;
}

int readFile(fs::FS &fs, const char * path, String *buffer){
    // DEBUG.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        // DEBUG.println(F("Failed to open file for reading"));
        return 1;
    }

    // DEBUG.print(F("Read from file: "));
    sdBuffer = "";
    while(file.available()){
        sdBuffer += String((char)file.read());
    }
    // DEBUG.println(sdBuffer);
    file.close();
    return 0;
}

int writeFile(fs::FS &fs, const char * path, const char * message){
    DEBUG.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        DEBUG.println(F("Failed to open file for writing"));
        return 1;
    }
    if(file.print(message)){
        DEBUG.println(F("File written"));
    } else {
        DEBUG.println(F("Write failed"));
        return 2;
    }
    file.close();
    return 0;
}

int appendFile(fs::FS &fs, const char * path, const char * message){
    // DEBUG.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        // DEBUG.println(F("Failed to open file for appending"));
        return 1;
    }
    if(file.print(message)){
        // DEBUG.println(F("Message appended"));
    } else {
        // DEBUG.println(F("Append failed"));
        return 2;
    }
    file.close();
    return 0;
}

int renameFile(fs::FS &fs, const char * path1, const char * path2){
    DEBUG.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2)) {
        DEBUG.println(F("File renamed"));
    } else {
        DEBUG.println(F("Rename failed"));
        return 1;
    }
    return 0;
}

int deleteFile(fs::FS &fs, const char * path){
    DEBUG.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        DEBUG.println(F("File deleted"));
    } else {
        DEBUG.println(F("Delete failed"));
        return 1;
    }
    return 0;
}

int testFileIO(fs::FS &fs, const char * path){
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if(file){
        len = file.size();
        size_t flen = len;
        start = millis();
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        DEBUG.printf("%u bytes read for %u ms\n", flen, end);
        file.close();
    } else {
        DEBUG.println(F("Failed to open file for reading"));
        return 1;
    }


    file = fs.open(path, FILE_WRITE);
    if(!file){
        DEBUG.println(F("Failed to open file for writing"));
        return 2;
    }

    size_t i;
    start = millis();
    for(i=0; i<2048; i++){
        file.write(buf, 512);
    }
    end = millis() - start;
    DEBUG.printf("%u bytes written for %u ms\n", 2048 * 512, end);
    file.close();
    return 0;
}

int readWiFiCredentialsFromSD(char ssid[], char pswd[]) {
	DEBUG.print(F("[SD] Reading credentials from SD... "));
    // Read File
    sdBuffer = "";
    int attempts = 0;
    int error = -1;
    do {
        error = readFile(SD, "/wifi.txt", &sdBuffer); 
        delay(SD_READ_ATTEMPTS_INTERVAL);
    } while (error != 0 && attempts++ < SD_READ_ATTEMPTS);

    if (error != 0) {
        DEBUG.println(F("Error reading file."));
        return error;
    }
    // DEBUG.println(sdBuffer);

    // Parse file
    int ssidIndex = sdBuffer.indexOf("SSID[");
    int ssidIndexEnd = sdBuffer.indexOf("]", ssidIndex);
    int pswdIndex = sdBuffer.indexOf("PSWD[");
    int pswdIndexEnd = sdBuffer.indexOf("]", pswdIndex);
    if (ssidIndex == -1 || ssidIndexEnd == -1 || pswdIndex == -1 || pswdIndexEnd == -1) {
        DEBUG.println(F("Invalid credentials!"));
        // Set the standard values
        String(CREDENTIALS_SSID).toCharArray(ssid,50);   // your network SSID (name) 
        String(CREDENTIALS_PASS).toCharArray(pass,50);   // your network password
        return 3;
    }
    DEBUG.println(F("Credentials updated!"));
    // Update the global wifi credentials
    sdBuffer.substring(ssidIndex + 5, ssidIndexEnd).toCharArray(ssid, 50);
    sdBuffer.substring(pswdIndex + 5, pswdIndexEnd).toCharArray(pswd, 50);
}

void logDataToSD() {
	DEBUG.print(F("[SD] Logging data to SD... "));
    if (updateLocalTime()) {
        // Date \t Time \t Temp \t Luminosity \t Fan speed \n
        String data = "";
        data += (currentTimeFormatted);
        data += (LOG_SEPARATOR);
        data += (insideTemperature);
        data += (LOG_SEPARATOR);
        data += (insideLuminosity);
        data += (LOG_SEPARATOR);
        data += (fanSpeed);
        data += (LOG_END_OF_LINE);

        // Append File
        int attempts = 0;
        int error = -1;
        do {
            error = appendFile(SD, "/log.csv", data.c_str()); 
            delay(SD_WRITE_ATTEMPTS_INTERVAL);
        } while (error != 0 && attempts++ < SD_WRITE_ATTEMPTS);

        if (error != 0) {
            DEBUG.println(F("Error appending file."));
        } 
        else {
	        DEBUG.println(F("OK!"));    
        }
    }
    else {
        DEBUG.println(F("Error updating time."));
        getTimeFromNtpServer();
    }
}
