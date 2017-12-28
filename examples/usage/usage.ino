// Example usage for logSystem library by wvdv2002.

#include "logSystem.h"

// Initialize objects from the lib
LogSystem logSystem;

void setup() {
    // Call functions on initialized library objects that require hardware
    logSystem.begin();
}

void loop() {
    // Use the library's initialized objects and functions
    logSystem.process();
}
