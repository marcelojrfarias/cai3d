void initSDCard() {
    int attemps = 0;
    do {
        if(!SD.begin()){
            DEBUG.println("Card Mount Failed");
            
        }
    } while(attemps++ < 3);

    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        DEBUG.println("No SD card attached");
        return;
    }

    DEBUG.print("SD Card Type: ");
    if(cardType == CARD_MMC){
        DEBUG.println("MMC");
    } else if(cardType == CARD_SD){
        DEBUG.println("SDSC");
    } else if(cardType == CARD_SDHC){
        DEBUG.println("SDHC");
    } else {
        DEBUG.println("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    DEBUG.printf("SD Card Size: %lluMB\n", cardSize);
}

int listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    DEBUG.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        DEBUG.println("Failed to open directory");
        return 1;
    }
    if(!root.isDirectory()){
        DEBUG.println("Not a directory");
        return 2;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            DEBUG.print("  DIR : ");
            DEBUG.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            DEBUG.print("  FILE: ");
            DEBUG.print(file.name());
            DEBUG.print("  SIZE: ");
            DEBUG.println(file.size());
        }
        file = root.openNextFile();
    }
    return 0;
}

int createDir(fs::FS &fs, const char * path){
    DEBUG.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path)){
        DEBUG.println("Dir created");
    } else {
        DEBUG.println("mkdir failed");
        return 1;
    }
    return 0;
}

int removeDir(fs::FS &fs, const char * path){
    DEBUG.printf("Removing Dir: %s\n", path);
    if(fs.rmdir(path)){
        DEBUG.println("Dir removed");
    } else {
        DEBUG.println("rmdir failed");
        return 1;
    }
    return 0;
}

int readFile(fs::FS &fs, const char * path){
    DEBUG.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        DEBUG.println("Failed to open file for reading");
        return 1;
    }

    DEBUG.print("Read from file: ");
    while(file.available()){
        DEBUG.write(file.read());
    }
    file.close();
    return 0;
}

int writeFile(fs::FS &fs, const char * path, const char * message){
    DEBUG.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        DEBUG.println("Failed to open file for writing");
        return 1;
    }
    if(file.print(message)){
        DEBUG.println("File written");
    } else {
        DEBUG.println("Write failed");
        return 2;
    }
    file.close();
    return 0;
}

int appendFile(fs::FS &fs, const char * path, const char * message){
    DEBUG.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        DEBUG.println("Failed to open file for appending");
        return 1;
    }
    if(file.print(message)){
        DEBUG.println("Message appended");
    } else {
        DEBUG.println("Append failed");
        return 2;
    }
    file.close();
    return 0;
}

int renameFile(fs::FS &fs, const char * path1, const char * path2){
    DEBUG.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2)) {
        DEBUG.println("File renamed");
    } else {
        DEBUG.println("Rename failed");
        return 1;
    }
    return 0;
}

int deleteFile(fs::FS &fs, const char * path){
    DEBUG.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        DEBUG.println("File deleted");
    } else {
        DEBUG.println("Delete failed");
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
        DEBUG.println("Failed to open file for reading");
        return 1;
    }


    file = fs.open(path, FILE_WRITE);
    if(!file){
        DEBUG.println("Failed to open file for writing");
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
