void logSensorsToSerial() {
    if (updateLocalTime()) {
        // Date \t Time \t Temp \t Luminosity \t Fan speed \n
        LOG.print("[LOG]");
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
