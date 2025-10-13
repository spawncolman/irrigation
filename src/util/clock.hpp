#ifndef SRC_UTIL_CLOCK
#define SRC_UTIL_CLOCK

#include <RtcDS1302.h>
#include <data/program.h>

namespace util::clock {

class CustomClock {
  public:
    CustomClock();

    void setup();

    bool getTime(RtcDateTime &dt);

    bool setTime(RtcDateTime &dt);

    void printDateTime(const RtcDateTime &dt);

    static void setInternalDateTime(RtcDateTime &now);

    static void printInternalDateTime();

    /**
     * @brief Get the Time From Program Date object
     *
     * @param progDate
     * @return time_t
     *
     * @note convierte tiempo de programa a time_t
     */
    static time_t getTimeFromProgramDate(riego::data::ProgramDate &progDate);

    /**
     * @brief Get the Time Now To Valve Time object
     *
     * @param valveTime
     * @return time_t
     *
     * @note devuelve en formato time_t la hora de la valvula partiendo del dia
     * de hoy.
     */
    static time_t getTimeNowToValveTime(riego::data::ValveTime &valveTime);

  private:
};

} // namespace util::clock

#endif /* SRC_UTIL_CLOCK */
