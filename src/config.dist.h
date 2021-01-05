#ifndef U8G2_16BIT
    #error This display needs 16Bit support. Define U8G2_16BIT in your Environment. 
#endif

#define DISPLAY_WIDTH 296
#define DISPLAY_HEIGHT 128

#define INFOURL "http://url.to.your.script/waveshare2.9.html.php"

 // DISPLAY_BYTES DISPLAY_WIDTH * DISPLAY_HEIGHT / 8
#define DISPLAY_BYTES 4736

#define SSID "YOURSSID"
#define PASSWORD "YOURPASSWORD"

//  5 Minutes
#define ERROR_WAIT 1000 * 60 * 5

//  1 Minute
#define SUCCESS_WAIT 1000 * 60

// 20 Seconds
#define TRY_AGAIN_WAIT 1000 * 20

#define UNITNAME "default"
