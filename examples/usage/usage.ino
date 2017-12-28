// Example usage for logSystem library by wvdv2002.

#include "SdFat.h"
#include "logSystem.h"

// Pick an SPI configuration.
// See SPI configuration section below (comments are for photon).
#define SPI_CONFIGURATION 3
//------------------------------------------------------------------------------
// Setup SPI configuration.
#if SPI_CONFIGURATION == 0
// Primary SPI with DMA
// SCK => A3, MISO => A4, MOSI => A5, SS => A2 (default)
SdFat sd;
const uint8_t chipSelect = SS;
#elif SPI_CONFIGURATION == 1
// Secondary SPI with DMA
// SCK => D4, MISO => D3, MOSI => D2, SS => D1
SdFat sd(1);
const uint8_t chipSelect = D1;
#elif SPI_CONFIGURATION == 2
// Primary SPI with Arduino SPI library style byte I/O.
// SCK => A3, MISO => A4, MOSI => A5, SS => A2 (default)
SdFatLibSpi sd;
const uint8_t chipSelect = SS;
#elif SPI_CONFIGURATION == 3
// Software SPI.  Use any digital pins.
// MISO => D5, MOSI => D6, SCK => D7, SS => D0
SdFatSoftSpi<D5, D6, D7> sd;
const uint8_t chipSelect = D0;
#endif  // SPI_CONFIGURATION
//------------------------------------------------------------------------------



LogSystem dataLog(sd,String("dataLog"));
uint32_t lastLogTime;
  
void setup() {
	int oldestLogTime;
	bool removeOldestMonth = 0;
	float temp;
	Particle.function("removeLogs", funcRemoveOldLogs);
	if (!sd.begin(chipSelect, SPI_HALF_SPEED)) {
		Particle.publish("pSys","No SD card found");
	}else{
		temp = getSDFreePercentage();
		Particle.publish("pSys","SD card found, free space: " + String());
		SdFile::dateTimeCallback(dateTime);
		if(temp<15){
		removeOldestMonth = 1;
		}
	}
    dataLog.begin();
	lastLogTime = millis();
	oldestLogTime = dataLog.getOldestLogTime();
	
	Particle.publish("pSys","Started logging, oldest logTime: " + String(oldestLogTime));
	if(removeOldestMonth){
		dataLog.removeOldestMonth();
	}
}



void loop() {
  if (millis() >lastLogTime+2000){
	lastLogTime = millis();
	dataLog << "Testing " << lastLogTime << endl; //Push data into the logs.
  }
  if (millis()>1000*3600*5){
	dataLog.endLogging(); //Stop logging after a few hours. This needs to be called before the particle device is shut down, otherwise the file size of the unclosed last log file will be 0 and data from the last hour will be lost.
	}
  dataLog.task(); //This task is so new log files are created.
}


//Call this function to remove a log from before a certain time.
//Use dataLog,1400000000 to remove all logs before "Tuesday 13 May 2014 16:53:20", WARNING, it will only remove complete months which are older than the current month.
//So in the example, april 2014 and older logs will be removed.
//Use www.epochconverter.com to convert a date to unix time.

int funcRemoveOldLogs(String command){
  int aComma = command.indexOf(",");
  if (aComma < 1){return -1;}
  String aDateString = command.substring(aComma+1);
  String aLog = command.substring(0,aComma);
  //If searching for the oldest log time:
  if(aDateString == "oldestLogTime"){
    if(aLog == "datalog"){
      aComma = dataLog.getOldestLogTime();
    }else{aComma = -1;}
    Particle.publish("pSys","Oldest " + aLog + " " + String(Time.year(aComma)) +"-"+String(Time.month(aComma)) + ", " + String(aComma));
    return 1;
  }

  int aDate = aDateString.toInt();
  if (aDate < 1400000000){return -1;}//If a wrong time is given, fail.
  if ((Time.year() == Time.year(aDate)) && (Time.month() == Time.month(aDate))){return -1;}//Cannot delete this month, as would give serious problems as there are still files being written.
  if (Time.now()<aDate){return -1;} //If the time is after now, fail.
  Particle.publish("pSys", "Removing "+ aLog + " from before: " + String(Time.year(aDate)) + "-" + String(Time.month(aDate)) + " Free:" + String(getSDFreePercentage()));
  if(aLog == "datalog"){
    dataLog.removeOldLogs(aDate);
    Particle.publish("pSys", "Removing done, now free: " + String(getSDFreePercentage()));
    return 1;
  }
  else{
    Particle.publish("pSys", "Wrong input, should be 'rawlog,1493987779' or 'datalog,1493987779' for example");
  }
  return -1;
}




float getSDFreePercentage(void){
	return ((float) sd.vol()->freeClusterCount())/((float)sd.vol()->clusterCount())*100;
}


void dateTime(uint16_t* date, uint16_t* time) {
  // return date using FAT_DATE macro to format fields
  *date = FAT_DATE(Time.year(Time.now()), Time.month(Time.now()), Time.day(Time.now()));

  // return time using FAT_TIME macro to format fields
  *time = FAT_TIME(Time.hour(Time.now()), Time.minute(Time.now()), Time.second(Time.now()));
}