#ifndef __LOGSYSTEM_H__
#define __LOGSYSTEM_H__

#include "application.h"
#include <fstream.h>
#include <SdFat.h>

class LogSystem : public ofstream{
private:
  SdFatBase& sd;
  String workDir;
  String version;
  uint32_t openTime;
  uint32_t rotationRate;
  bool openNewLogFile(void);

protected:
public:
  LogSystem(SdFatBase& anSD,String workDir);
  LogSystem(SdFatBase& anSD, String workDir, String Version, int rotationRate);
  void task(void);
  void begin(void);
  void endLogging(void);
  void removeOldLogs(int beforeThisTime);
  int getOldestLogTime(void);
  void removeOldestMonth(void);
  friend ostream& operator<< (ostream &os, const String& arg) {
    char charBuf[128];
    arg.toCharArray(charBuf,sizeof(charBuf));
    os << charBuf;
    return os;
  }
};

#endif
