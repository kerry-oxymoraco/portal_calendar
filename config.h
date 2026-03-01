#ifndef PORTALCALENDAR_CONFIG_H
#define PORTALCALENDAR_CONFIG_H

enum class WeatherDisplayType : uint8_t
{
    FORECAST_5_DAY = 0,
    FORECAST_12_HOUR = 1,
};

enum class WeatherUnits : uint8_t
{
    IMPERIAL = 0,
    METRIC = 1,
};

enum class WeatherSecondaryInfo : uint8_t
{
    POP = 0,
    HUMIDITY = 1,
};

/**
 * Default values for settings
 */
#define DEFAULT_LOCALE "en"

#define DEFAULT_SHOW_DAY true
#define DEFAULT_SHOW_MONTH true
#define DEFAULT_SHOW_YEAR false

#define DEFAULT_HOSTNAME "portalcalendar"

#define DEFAULT_PRIMARY_NTP_SERVER "pool.ntp.org"
#define DEFAULT_SECONDARY_NTP_SERVER "time.google.com"

#define DEFAULT_PRIMARY_TIMEZONED_SERVER "timezoned.rop.nl"
#define DEFAULT_SECONDARY_TIMEZONED_SERVER "timezoned.jacobjordan.tech"

#define DEFAULT_WEATHER_DISPLAY_TYPE WeatherDisplayType::FORECAST_5_DAY
#define DEFAULT_WEATHER_UNITS WeatherUnits::IMPERIAL
#define DEFAULT_WEATHER_SECONDARY_INFO WeatherSecondaryInfo::POP
#define DEFAULT_WEATHER_START_HOUR 9
#define DEFAULT_USE_24H_TIME false

#define DEFAULT_2_NTP_SYNCS_PER_DAY true
#define DEFAULT_MAX_RTC_CORRECTION_FACTOR 0.025

#define NTP_TIMEOUT_SECONDS 5
#define TZ_LOOKUP_TIMEOUT_SECONDS 5
#define WIFI_CONNECTION_TIMEOUT_SECONDS 10

#define MINUTES_BEFORE_MIDNIGHT_TO_SYNC_1 72
#define MINUTES_BEFORE_MIDNIGHT_TO_SYNC_2 8

#define ERROR_AFTER_HOURS_WITHOUT_INTERNET 24

#define AP_SSID "PortalCalendar"
// #define AP_PASS "12345678"

#define NTP_LOCAL_PORT_START 4242
#define TIMEZONED_LOCAL_PORT_START 2342

/**
 * Pin assignments for Waveshare Universal e-Paper Driver Board (ESP32)
 */
#define SPI_BUS     HSPI
#define DIN_PIN     14  // MOSI
#define CLK_PIN     13  // SCK
#define CS_PIN      15  // CS
#define DC_PIN      27  // Data/Command
#define RESET_PIN   26  // Reset
#define BUSY_PIN    25  // Busy

#define PD_PIN          19
#define PD_PIN_STATE    HIGH

// #define ENABLE_GPIO0_PULLUP
// #define HEADLESS
// #define DISABLE_MANUAL_CONFIG_SERVER_ACTIVATION
// #define DEV_WEBSERVER
// #define DEV_WEBSERVER_WIFI_SSID "ssid"
// #define DEV_WEBSERVER_WIFI_PASS "password"

#endif // PORTALCALENDAR_CONFIG_H
