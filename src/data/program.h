#ifndef SRC_DATA_PROGRAM
#define SRC_DATA_PROGRAM

#include <cstdint>
#include <util/valve.hpp>

/** valve number of the system  */
static constexpr uint8_t NUM_PROGRAMS = 5U;
static constexpr uint8_t NUM_SEQUENCE = 10U;
static constexpr uint8_t NUM_VALVES = 2U;
static constexpr uint16_t NUM_EVENTS =
    ((uint16_t)NUM_SEQUENCE * (uint16_t)NUM_VALVES * (uint16_t)NUM_PROGRAMS);

namespace riego::data {

/**
 * @brief Valve time.
 *
 */
struct ValveTime {

    /** hour */
    uint8_t hour;
    /** minute */
    uint8_t minute;
};

struct Valve {

    /* initial time */
    ValveTime start;

    /* end time */
    ValveTime end;

    /** valve type */
    ValveType type;

    /**
     * @brief Construct a new Valve object
     *
     */
    Valve();
};

/**
 * @brief Program Date.
 *
 */
struct ProgramDate {

    /** year */
    uint16_t year;
    /** month */
    uint8_t month;
    /** day */
    uint8_t day;
};

struct Sequence {

    /* valve identifier */
    uint8_t valveId;

    /* initial time */
    ValveTime start;

    /* sequence duration */
    uint8_t duration;

    /* duration magnitude from 0 to 60
     * 0 = sequence deactivate
     * 1 <-> 60 minutes
     * 1 <-> 11 hours
     */
    uint16_t durationMagnitude;

    /** valve type */
    ValveType type;

    /** sequence status, see ValveCommand */
    uint8_t status; 
};

struct Program {

    /** start date */
    ProgramDate start;

    /** end date */
    ProgramDate end;

    /** days of the week (bitmask)*/
    uint8_t daysOfTheWeek;

    /* number of valves */
    //Valve valves[NUM_VALVES];

    /* sequences */
    Sequence sequences[NUM_SEQUENCE];

    /* period 12h / 1 <-> 5 days in minutes */
    uint16_t period;

    /* enable or disable*/
    bool status;

    /**
     * @brief read the system program.
     *
     * @param program system program to load
     * @return true
     * @return false
     */
    static bool readSystemProgram(Program *program);

    /**
     * @brief save the system program.
     *
     * @param program read system program.
     * @return true
     * @return false
     */
    static bool saveSystemProgram(Program *program);

    /**
     * @brief read the selected program.
     *
     * @param selected program
     * @return true
     * @return false
     */
    static bool readSelectedProgram(uint8_t *selectedProgram);

    /**
     * @brief set the selected program.
     *
     * @param selected program
     * @return true
     * @return false
     */
    static bool setSelectedProgram(uint8_t selectedProgram);

    /**
     * @brief print program
     *
     * @param program program to print
     * @return true
     * @return false
     */
    static void printProgram(Program *program);
};

} // namespace riego::data

#endif /* SRC_DATA_PROGRAM */
