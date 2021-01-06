// The URL to the script providing the XBM-Data
#define INFOURL "http://url.to.your.script/waveshare2.9.html.php"

// The SSID of your WiFi network
#define SSID "YOURSSID"

// The password of the wifi password
#define PASSWORD "YOURPASSWORD"


#ifndef U8G2_16BIT
    #error This display needs 16Bit support. Define U8G2_16BIT in your Environment. 
#endif

#define DISPLAY_WIDTH 296
#define DISPLAY_HEIGHT 128


 // DISPLAY_BYTES DISPLAY_WIDTH * DISPLAY_HEIGHT / 8
#define DISPLAY_BYTES 4736


//  5 Minutes
#define ERROR_WAIT 1000 * 60 * 5

//  1 Minute
#define SUCCESS_WAIT 1000 * 60

// 20 Seconds
#define TRY_AGAIN_WAIT 1000 * 20

#define UNITNAME "default"
