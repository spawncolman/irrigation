#ifndef SECURE_BOOT_BOOT_HPP
#define SECURE_BOOT_BOOT_HPP


#include <FastLED.h>
#include <cstdint>

enum BOOT_PHASE:uint8_t {
	RED_BOOT_PHASE,
	GREEN_BOOT_PHASE,
	BLUE_BOOT_PHASE
} ;

namespace util::boot {

class Boot {
 public:
	Boot();

	/**
	 * @brief class setup.
	 * 
	 */
	void setup();

	bool checkPhase(BOOT_PHASE phase);

	void ledOn(CRGB color);

	void ledOff();

 private:
};

} // namespace util::boot

#endif  // SECURE_BOOT_BOOT_HPP
