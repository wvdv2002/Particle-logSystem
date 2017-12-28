# Particle-logSystem
This library can be used to log strings of data to an SD card using iostreams. The logs will automatically rotate every hour. 
There are ways to remove old logs, for example if the SD card slowly fills up with data.

## Getting started
Connect an SDcard to your Particle device. Setup the right pins and SD card configuration in the example and run it by compiling and flashing the files to a particle device.
Run the example for a few hours. Remove the sd card and check the files and log structure on the sd card.
Make sure that particle device time is synchronized.

See the [examples](examples) folder for more details.

## Documentation
When restarting the particle device, new data is automatically appended if a log file still exists for the time slot.
All iostream manipulations that the SDfat library supports can be used. For now pushing a string into a log file does not work (see Contributing).
The file structure on the sd card will be as follows:
/"logname"/"year"/"monthnumber"/"Version"-"hour".log, for example:
/datalog/2017/12/V1-12.log


## Contributing
* Pushing a particle String into a log did work when I just added the sdfat library as code to my project, but adding it as a library it does not work. Maybe someone could look into the way I solved String handling for an iostream (see LogSystem.h).
* When pushing a string into the log, it cannot have more than 128 characters for now, as a string is converted to a char array before pushing it in. If somebody has nicer code for this, please update that part.
* The second constructor in which you can set the rotation speed and version number of the files created is untested!
* The functions to remove old logs are quite nasty at the moment and logs can only be removed with month granularity. This part of the code could be improved.

## LICENSE
Copyright 2017 wvdv2002

Licensed under the ISC license
