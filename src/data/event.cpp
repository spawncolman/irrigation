#include "event.hpp"
#include "context.h"

#include <vector>

namespace riego::data {

Events::Events() {}

void Events::sortEvents() {
    if (activeEvents > 0) {
        mergeSort(eventList, 0, activeEvents - 1);
    }
}

void Events::printEvents() {
    for (uint8_t ind = 0; ind < activeEvents; ++ind) {
        Serial.printf("Event %u\n", ind);
        Serial.printf("\tvalveId: %u\n", eventList[ind].valveId);
        Serial.printf("\tcommand: %u\n", eventList[ind].command);
        Serial.printf("\ttime: %llu\n", eventList[ind].time);
        // Convert now to tm struct for local timezone
        tm *localtm = localtime(&eventList[ind].time);
        printf("\thuman time: %s", asctime(localtm));
    }
}

void Events::mergeSort(Event events[], int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;

        // Sort first and second halves
        mergeSort(events, left, middle);
        mergeSort(events, middle + 1, right);

        merge(events, left, middle, right);
    }
}

void riego::data::Events::merge(Event events[], int left, int middle,
                                int right) {
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;

    // Create temp arrays
    Event L[n1], R[n2];

    // Copy data to temp arrays L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = events[left + i];
    for (j = 0; j < n2; j++)
        R[j] = events[middle + 1 + j];

    // Merge the temp arrays back into arr[l..r
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        if (L[i].time <= R[j].time) {
            events[k] = L[i];
            i++;
        } else {
            events[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[],
    // if there are any
    while (i < n1) {
        events[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[],
    // if there are any
    while (j < n2) {
        events[k] = R[j];
        j++;
        k++;
    }
}

} // namespace riego::data