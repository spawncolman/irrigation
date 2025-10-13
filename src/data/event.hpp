#ifndef SRC_DATA_EVENT
#define SRC_DATA_EVENT

#include <cstdint>
#include <data/program.h>
#include <util/valve.hpp>

#include <Arduino.h>


struct Event {

    /* valve identifier */
    uint8_t valveId;

    /* program index */
    uint8_t progIndex;

    /* sequence index */
    uint8_t sequenceIndex;

    /* valve command */
    ValveCommand command;

    /* event time in seconds from 1970-01-01 00:00:00 +0000 */
    time_t time;
    
};

namespace riego::data {

struct Events {

    /**
     * @brief Construct a new Events object
     *
     */
    Events();

    /**
     * @brief event sort function.
     *
     */
    void sortEvents();

    /**
     * @brief print event list.
     *
     */
    void printEvents();

  private:
    /**
     * @brief merge sort algorithm for events
     *
     */
    void mergeSort(Event events[], int left, int right);

    /**
     * @brief merge support function.
     *
     * @param events
     * @param left
     * @param mid
     * @param right
     */
    void merge(Event events[], int left, int mid, int right);
};

} // namespace riego::data

#endif /* SRC_DATA_EVENT */
