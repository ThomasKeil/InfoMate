<?php

/**
 * Path to the needed binaries
 */
$WKHTMLTOIMAGE = "/usr/bin/wkhtmltoimage";
$CONVERT = "/usr/bin/convert";

/**
 * URL of your information source
 */
$URL = "http://localhost/example.php";

/**
 * Resolution of the display.
 * This is for a Waveshare 2.9" e-paper display
 */
$WIDTH = 296;
$HEIGHT = 128;
$BYTES_EXPECTED = $WIDTH * $HEIGHT / 8;


try {
    $id = uniqid();
    $png_file = "/tmp/".$id.".png";
    $xbm_file = "/tmp/".$id.".xbm";

    $output = [];

    $GET = http_build_query($_GET);
    if ($GET) $GET = "?".$GET;

    $command = $WKHTMLTOIMAGE . " --crop-h " . $HEIGHT . " --crop-w " . $WIDTH . " \"" . $URL . $GET . "\" "  . $png_file;
    $retval = 0;
    exec($command, $output, $retval);
    if ($retval != 0) {
        throw new Exception("PNG nicht erzeugt [".$retval."]");
    }

    if (!is_file($png_file)) {
        throw new Exception("PNG nicht gefunden");
    }

    $image_info = getimagesize($png_file);
    if ($image_info[0] != $WIDTH || $image_info[1] != $HEIGHT) {
        throw new Exception("Falsche Dimension: ".$image_info[0]."x".$image_info[1]);
    }

    $command = $CONVERT . " " . $png_file . " XBM:".$xbm_file;
    exec($command, $output, $retval);
    if ($retval != 0) {
        throw new Exception("XBM nicht erzeugt [".$retval."]");
    }

    $xbm_content = file_get_contents($xbm_file);

    $matches = [];
    preg_match("/\{(.*), \};/s", $xbm_content, $matches );

    $hex_bytes = $matches[1];
    if ($hex_bytes == "") {
        throw new Exception("Kein XBM Inhalt");
    }
    $hex_bytes = preg_replace("/\s/", "", $hex_bytes);
    $hex_bytes = explode(",", $hex_bytes);

    if (count($hex_bytes) != $BYTES_EXPECTED) {
        throw new Exception("Anzahl Bytes ".count($hex_bytes));
    }

    $result = "";
    foreach ($hex_bytes as $hex_byte) {
        $hex = substr($hex_byte, -2);
        $result .= chr(hexdec($hex));
    }
    unlink($png_file);
    unlink($xbm_file);
    echo "XBM: " . $result;
} catch (Exception $e) {
    $error = $e->getMessage();
    if (!$error) $error = "Unbekannter Fehler";
    echo "Error: " . $error;
}
