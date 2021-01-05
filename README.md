# Informate
From <https://en.wikipedia.org/wiki/Informating>:

> Informating [...] is the process that translates descriptions and measurements of activities, events and objects into information. By doing so, these activities become visible to the organization. 

## What is this
This is a firmware for ESP8266 Processors and a connected display to display an image, provided as XBM-like data by a webserver.
Currently supported is a Waveshare 2.9" e-paper display, connected by 4-Wire SPI.

## Hardware

I used a NodeMCU board.

## Debugging

More like a note for me, but use this if necessary.

```java -jar /path/to/EspStackTraceDecoder.jar /path/to/addr2line .pio/build/YOURENVIRONMENT/firmware.elf STACKTRACEFILE```

Obtain EspStacktraceDecoder at https://github.com/littleyoda/EspStackTraceDecoder 

addr2line is part of the GNU binutils.