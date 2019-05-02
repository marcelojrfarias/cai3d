void getTimeFromNtpServer() {
  DEBUG.print(F("[TIME] Synchronizing the date and time via ntp server.. "));
  if(!getLocalTime(&currentTime)) {
    // Manage WiFi connection
    if (wifiConnect()) {
      configTime(GMT_OFFSET_SEC, DAY_LIGHT_OFFSET_SEC, NTP_SERVER);
      DEBUG.println(F("Synced!"));
    }
  } else {
    DEBUG.println(F("Time and date data already synced!"));
  }
}

bool updateLocalTime() {
  if(!getLocalTime(&currentTime)){
    // DEBUG.println("Failed to obtain time");
    return false;
  }
  strftime(currentTimeFormatted, 64, "%d/%m/%Y,%H:%M:%S", &currentTime); // "%A, %B %d %Y %H:%M:%S"
  // DEBUG.println(currentTimeFormatted);
  return true;
}

