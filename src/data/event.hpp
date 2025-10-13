#ifndef SRC_DATA_EVENT
#define SRC_DATA_EVENT

#include <cstdint>
#include <data/program.h>
#include <util/valve.hpp>
#include <ctime>

#include <Arduino.h>

/**
 * @file event.hpp
 * @brief Event scheduling types and utilities for the irrigation controller.
 */

/**
 * @struct Event
 * @brief Representation of a scheduled valve event.
 *
 * Contains minimal information required by the scheduler: which valve to act on,
 * which program/sequence generated the event, the command (open/close) and the
 * epoch timestamp for the event.
 */
struct Event {
  uint8_t valveId;       ///< Valve identifier (0..NUM_VALVES-1)
  uint8_t progIndex;     ///< Origin program index
  uint8_t sequenceIndex; ///< Origin sequence index
  ValveCommand command;  ///< Action to perform (Open/Close)
  time_t time;           ///< Event time (seconds since UNIX epoch UTC)
};

namespace riego::data {

/**
 * @class Events
 * @brief Small helper for sorting and printing an array of Event entries.
 *
 * The system keeps events in a global `eventList[]` array and tracks the
 * number of active entries in `activeEvents`. This class provides operations
 * that operate on that global array (sorting, debug printing).
 */
struct Events {
  Events();

  /**
   * @brief Sort the first `activeEvents` elements of the global `eventList`.
   */
  void sortEvents();

  /**
   * @brief Print the current events to Serial (debug helper).
   */
  void printEvents();

private:
  /**
   * @brief Internal merge sort implementation used by sortEvents.
   */
  void mergeSort(Event events[], int left, int right);

  /**
   * @brief Merge helper for mergeSort.
   */
  void merge(Event events[], int left, int mid, int right);
};

} // namespace riego::data

#endif /* SRC_DATA_EVENT */
