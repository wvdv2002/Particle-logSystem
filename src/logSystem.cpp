/* logSystem library by wvdv2002
 */

#include "logSystem.h"

/**
 * Constructor.
 */
LogSystem::LogSystem()
{
  // be sure not to call anything that requires hardware be initialized here, put those in begin()
}

/**
 * Example method.
 */
void LogSystem::begin()
{
    // initialize hardware
    Serial.println("called begin");
}

/**
 * Example method.
 */
void LogSystem::process()
{
    // do something useful
    Serial.println("called process");
    doit();
}

/**
* Example private method
*/
void LogSystem::doit()
{
    Serial.println("called doit");
}
