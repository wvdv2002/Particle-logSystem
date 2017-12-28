#include "logSystem.h"
  LogSystem::LogSystem(SdFatBase& anSD,String aWorkDir):sd(anSD)
  {
    workDir = aWorkDir;
    rotationRate = 3600;
    version = "V1";
  }

  LogSystem::LogSystem(SdFatBase& anSD,String aWorkDir,String aVersion, int aRotationRate):sd(anSD){
    workDir = aWorkDir;
    rotationRate = aRotationRate;
    version = aVersion;
  }

  void LogSystem::task(void){
    if(Time.hour(Time.now())!=Time.hour(openTime)){
      openNewLogFile();
    }
  }

  void LogSystem::endLogging(void){
    if(is_open()){
      close();
    }
//    if(fileSize()==0){
//      remove();
//    }
  }

void LogSystem::removeOldestMonth(void){
  int aTime = getOldestLogTime();
  if(aTime>0){
    aTime += 2592000; //Add one month.
    if (aTime<Time.now()){
      Particle.publish("pSys", "Removing "+ workDir + " oldest month from before: " + String(Time.year(aTime)) + "-" + String(Time.month(aTime)));
      removeOldLogs(aTime);
    }else{
      Particle.publish("pSys","Cannot remove last month of data");
    }
  }else{Particle.publish("pSys","Cannot remove last month of data");}
}

int LogSystem::getOldestLogTime(void){
  char fileNameBuf[24];
  sd.chdir("/");
  workDir.toCharArray(fileNameBuf,sizeof(fileNameBuf));
  String aDirName;
  int aNumber;
  int aYear = 9999;
  int aMonth = 13;
  SdFile aDir;
  SdFile aMonthDir;
  if(sd.chdir(fileNameBuf)){
    sd.vwd()->rewind();
    //in main log dir, list of years
    while(aDir.openNext(sd.vwd(),O_READ)){
      if (aDir.isDir()){
        aDir.getName(fileNameBuf,sizeof(fileNameBuf));
        aDirName = String(fileNameBuf);
        aNumber = aDirName.toInt();
        if (aNumber < aYear){
            aYear=aNumber;
        }
      }
      aDir.close();
    }
    if(aYear<9999){//Found a year dir.
      String(aYear).toCharArray(fileNameBuf,sizeof(fileNameBuf));
      aDir.rewind();
      if(aDir.open(fileNameBuf,O_READ)){
        if(aDir.isDir()){
          aDir.rewind();
          while(aMonthDir.openNext(&aDir,O_READ)){
            if (aMonthDir.isDir()){
              aMonthDir.getName(fileNameBuf,sizeof(fileNameBuf));
              aDirName = String(fileNameBuf);
              aNumber = aDirName.toInt();
              if (aNumber < aMonth){
                aMonth=aNumber;
              }
            }
            aMonthDir.close();
          }
        }
        aDir.close();
      }
    }
  }
  sd.chdir("/");
  if((aMonth<13) && (aYear<9999)){
      aNumber = 1421283600; // The time at the 15th of january 2015.
      aNumber += (aYear-2015)*31557600; //The amount of seconds added for one year (365.25 days).
      aNumber += (aMonth-1)*2592000; //The amount of seconds in a month (30 days).
      if (aNumber<0){aNumber=0;}
      return aNumber;
  }
  return 0;
}

void LogSystem::removeOldLogs(int beforeThisTime){
  char fileNameBuf[24];
  sd.chdir("/");
  workDir.toCharArray(fileNameBuf,sizeof(fileNameBuf));
  String aDirName;
  int aNumber;
  int aYear = Time.year(beforeThisTime);
  int aMonth = Time.month(beforeThisTime);
  SdFile aDir;
  SdFile aMonthDir;
  if(sd.chdir(fileNameBuf)){
      sd.vwd()->rewind();
      //in main log dir, list of years
      while(aDir.openNext(sd.vwd(),O_READ)){
          if (aDir.isDir()){
            aDir.getName(fileNameBuf,sizeof(fileNameBuf));
            aDirName = String(fileNameBuf);
            aNumber = aDirName.toInt();
            if (aNumber < aYear){
                aDir.rmRfStar();
            }
            if (aNumber == aYear){
              String(aYear).toCharArray(fileNameBuf,sizeof(fileNameBuf));
              aDir.rewind();
              while(aMonthDir.openNext(&aDir,O_READ)){
                if (aMonthDir.isDir()){
                  aMonthDir.getName(fileNameBuf,sizeof(fileNameBuf));
                  aDirName = String(fileNameBuf);
                  aNumber = aDirName.toInt();
                  if (aNumber < aMonth){
                      aMonthDir.rmRfStar();
                  }
                }
                aMonthDir.close();
            }
          }
        }
        aDir.close();
      }
  }
  sd.chdir("/");
}

void LogSystem::begin(void){
    openNewLogFile();
  }

  bool LogSystem::openNewLogFile(void){

      char fileNameBuf[12];
      bool error=1;
      uint32_t tempTime = Time.now();
      String temp;
      endLogging();
      sd.chdir("/");
      workDir.toCharArray(fileNameBuf,sizeof(fileNameBuf));
      if(!sd.chdir(fileNameBuf)){sd.mkdir(fileNameBuf);sd.chdir(fileNameBuf);};

      temp = String(Time.year(tempTime));
      temp.toCharArray(fileNameBuf,sizeof(fileNameBuf));
      if(!sd.chdir(fileNameBuf)){sd.mkdir(fileNameBuf);sd.chdir(fileNameBuf);};

      temp = String(Time.month(tempTime));
      temp.toCharArray(fileNameBuf,sizeof(fileNameBuf));
      if(!sd.chdir(fileNameBuf)){sd.mkdir(fileNameBuf);sd.chdir(fileNameBuf);};

      temp = String(Time.day(tempTime));
      temp.toCharArray(fileNameBuf,sizeof(fileNameBuf));
      if(!sd.chdir(fileNameBuf)){sd.mkdir(fileNameBuf);sd.chdir(fileNameBuf);};

      String fileString = version + "-" + String(Time.hour(Time.now())) + ".log";
      fileString.toCharArray(fileNameBuf,sizeof(fileNameBuf));
      openTime = Time.now();
      open(fileNameBuf,ios::out | ios::app);
      if(is_open()){
        error=0;
      }
      if(error){;}
      sd.chdir("/");
      return error;
    }
