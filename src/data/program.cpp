#include "program.h"
#include <data/program.h>
/* se usa preferences para almacenar la configuracion dado que solo se usan
 * variables al parecer el maximo por defecto son 20 Kb
 */
#include <Preferences.h>
#include <cstdint>

Preferences prefs;

#define PREFS_SYSTEM_CONFIG_NAME "systemProgram"
#define PREFS_SELECTED_PROGRAM_NAME "selectedProgram"
#define RW_MODE false
#define RO_MODE true

namespace riego::data {

Valve::Valve() : type(NOT_COFIGURED) {}


bool riego::data::Program::readSystemProgram(Program *program) {
    bool retValue = false;

    prefs.begin(PREFS_SYSTEM_CONFIG_NAME, RO_MODE);
    size_t programLength = prefs.getBytesLength(PREFS_SYSTEM_CONFIG_NAME);

    if (programLength > 0) {
        char buffer[programLength] = {0}; // prepare a buffer for the data

        prefs.getBytes(PREFS_SYSTEM_CONFIG_NAME, buffer, programLength);
        if (programLength % (sizeof(riego::data::Program) *
                             NUM_PROGRAMS)) { // simple check that data fits
            Serial.println("Program size doesnt fit ");
        } else {
            // cast the bytes into a struct ptr
            memcpy((void *)program, (void *)buffer, programLength);
            Serial.printf("Program loaded, %u bytes\n",
                          (sizeof(Program) * NUM_PROGRAMS));
            retValue = true;
        }
    } else {
        Serial.println("No Program found");
    }
    prefs.end();

    return retValue;
}

bool Program::saveSystemProgram(Program *program) {
    bool retValue = false;

    if (program != nullptr) {
        printProgram(program);
        prefs.begin(PREFS_SYSTEM_CONFIG_NAME, RW_MODE);
        prefs.putBytes(PREFS_SYSTEM_CONFIG_NAME, program,
                       (sizeof(Program) * NUM_PROGRAMS));
        Serial.printf("Program saved, %u bytes\n",
                      (sizeof(Program) * NUM_PROGRAMS));
        prefs.end();
        retValue = true;
    }
    return retValue;
}

bool Program::readSelectedProgram(uint8_t *selectedProgram) {

    bool retValue = false;

    prefs.begin(PREFS_SELECTED_PROGRAM_NAME, RO_MODE);
    size_t programLength = prefs.getBytesLength(PREFS_SELECTED_PROGRAM_NAME);

    if (programLength > 0) {
        // prepare a buffer for the data
        char buffer[programLength] = {0};

        prefs.getBytes(PREFS_SYSTEM_CONFIG_NAME, buffer, programLength);
        // simple check that data fits
        if (programLength % sizeof(uint8_t)) {
            Serial.println("Selected program size doesnt fit ");
        } else {
            // cast the bytes into a struct ptr
            memcpy((void *)selectedProgram, (void *)buffer, programLength);
            Serial.printf("Selected program %u \n", *selectedProgram);
            retValue = true;
        }
    } else {
        Serial.println("No selected program found");
    }
    prefs.end();

    return retValue;
}

bool Program::setSelectedProgram(uint8_t selectedProgram) {
    bool retValue = false;

    prefs.begin(PREFS_SELECTED_PROGRAM_NAME, RW_MODE);
    prefs.putBytes(PREFS_SELECTED_PROGRAM_NAME, &selectedProgram,
                   (sizeof(uint8_t)));
    Serial.printf("Selected program %u saved\n", selectedProgram);
    prefs.end();
    retValue = true;

    return retValue;
}

void Program::printProgram(Program *program) {

    for (uint8_t ind = 0; ind < NUM_PROGRAMS; ++ind) {
        Serial.printf("PROGRAM %u\n", ind);
        Serial.printf("\tstart %.2d:%.2d:%.4d\n", program[ind].start.day,
                      program[ind].start.month, program[ind].start.year);
        Serial.printf("\tend %.2d:%.2d:%.4d\n", program[ind].end.day,
                      program[ind].end.month, program[ind].end.year);
        Serial.printf("\tSTATUS %u\n", program[ind].status);
        for (uint8_t ind2 = 0; ind2 < NUM_SEQUENCE; ++ind2) {
            Serial.printf("\tSequence %u\n", ind2);

            Serial.printf("\t\tValve id %.2d type %u\n",
                          program[ind].sequences[ind2].valveId,
                          program[ind].sequences[ind2].type);
            Serial.printf("\t\tstart %.2d:%.2d\n",
                          program[ind].sequences[ind2].start.hour,
                          program[ind].sequences[ind2].start.minute);
            Serial.printf("\t\tDuration %.2d * %.2d\n",
                          program[ind].sequences[ind2].duration,
                          program[ind].sequences[ind2].durationMagnitude);
            Serial.printf("\t\tStatus %u\n",
                          program[ind].sequences[ind2].status);
        }
    }
}

} // namespace riego::data