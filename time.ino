void getTimeFromNtpServer() {
  if(!getLocalTime(&currentTime)){
    DEBUG.println(F("Synchronizing the date and time via ntp server.."));
    configTime(GMT_OFFSET_SEC, DAY_LIGHT_OFFSET_SEC, NTP_SERVER);
  } else {
    DEBUG.println(F("Time and date data already synced!"));
  }
}

void updateLocalTime() {
  if(!getLocalTime(&currentTime)){
    DEBUG.println("Failed to obtain time");
    return;
  }
  strftime(currentTimeFormatted, 64, "%d/%m/%Y,%H:%M:%S", &currentTime); // "%A, %B %d %Y %H:%M:%S"
  DEBUG.println(currentTimeFormatted);
}

