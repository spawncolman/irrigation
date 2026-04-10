#ifndef SRC_UTIL_CLOCK
#define SRC_UTIL_CLOCK

#include <RtcDS1302.h>
#include <data/program.h>

/**
 * @file clock.hpp
 * @brief RTC helper wrapping RtcDS1302 for the irrigation controller.
 *
 * The CustomClock class provides convenience operations for initializing the
 * RTC, reading and writing the RTC time, printing date/time values and
 * converting application types (ProgramDate/ValveTime) to time_t values.
 *
 * The implementation relies on the RtcDS1302 library and the POSIX time
 * APIs (mktime, localtime) for conversions.
 */

namespace util::clock {

/**
 * @class CustomClock
 * @brief High-level wrapper around the RtcDS1302 RTC device.
 *
 * Responsibilities:
 * - Initialize the RTC and ensure it runs and is writable.
 * - Provide safe get/set operations for the time.
 * - Convert between application types and time_t.
 */
class CustomClock {
  public:
    /**
     * @brief Construct a CustomClock instance.
     */
    CustomClock();

    /**
     * @brief Initialize the RTC hardware and sanity-check the stored time.
     *
     * This will set the RTC to the compilation time if the RTC is invalid
     * or older than the firmware build time.
     */
    void setup();

    /**
     * @brief Retrieve the current RTC time.
     * @param dt Output parameter to receive the current RTC time.
     * @return true if the RTC time is valid and `dt` was populated.
     * @return false if the RTC reports an invalid date/time.
     */
    bool getTime(RtcDateTime &dt);

    /**
     * @brief Set the RTC time.
     * @param dt The date/time to set on the RTC.
     * @return true if the RTC reports a valid date/time after setting.
     */
    bool setTime(RtcDateTime &dt);

    /**
     * @brief Print a RtcDateTime to Serial in a human-friendly format.
     * @param dt The date/time to print.
     */
    void printDateTime(const RtcDateTime &dt);

    /**
     * @brief Set the system internal (POSIX) clock to the provided RtcDateTime.
     * @param now RtcDateTime value to install into the OS-level clock.
     */
    static void setInternalDateTime(RtcDateTime &now);

    /**
     * @brief Print the system internal POSIX date/time to stdout.
     */
    static void printInternalDateTime();

    /**
     * @brief Convert a ProgramDate object to time_t (midnight of that date).
     * @param progDate ProgramDate to convert.
     * @return time_t representing midnight (00:00) of the given date in
     *         the local timezone.
     */
    static time_t getTimeFromProgramDate(riego::data::ProgramDate &progDate);

    /**
     * @brief Convert today's date combined with a ValveTime into time_t.
     * @param valveTime ValveTime containing hour/minute to apply to today's date.
     * @return time_t representing the next occurrence of that time today (local timezone).
     */
    static time_t getTimeNowToValveTime(riego::data::ValveTime &valveTime);

  private:
};

} // namespace util::clock

#endif /* SRC_UTIL_CLOCK */
