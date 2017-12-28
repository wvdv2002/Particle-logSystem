#pragma once

/* logSystem library by wvdv2002
 */

// This will load the definition for common Particle variable types
#include "Particle.h"

// This is your main class that users will import into their application
class LogSystem
{
public:
  /**
   * Constructor
   */
  LogSystem();

  /**
   * Example method
   */
  void begin();

  /**
   * Example method
   */
  void process();

private:
  /**
   * Example private method
   */
  void doit();
};
