// blue wire data to arduino pin 6 (has to be PWM enabled)
// red wire 5V to arduino 5V
// white wire ground to arduino gnd

/******************************************   _
 *    x                           x       *   |
 *                    o               x   *   |
 *             o                          *   |
 *                       +       +        *   |
 *         x        o                     *   |
 *                                     x  *   |
 *                          o             *   | Columns
 *                    +                   *   |
 *       o                          x     *   |
 *               x                        *   |
 *                          o             *   |
 *                                     +  *   |
 *     o          +             x         *   |
 *                                        *   |
 *                     x                  *   |
 *         o                      +       *   |
 *                                        *   -
 *****************************************/
 /* 
  *|________________ rows _______________|
  * 
  */
 
 /* 
  * x, o, + are groups 
  * individual x, o, + are members of a group
  * an available LED is a pixel
  * 
  * 
  * Steps
  * 
  * 1. array of length = rows x columns with properties:
  * Setup: 
  *     - for each group, make: 
  *         - random nums of LEDs in this group
  *         - addresses of the LEDs in this group (make sure no overlap)
  *         - R, G, B max vals
  *         - max brightness (in percent)
  * Produces: 
  *     - rgb vals (max)
  *     - current r, g and b vals
  *     - brightness in percent ( = current rgb val / max rgb val)
  *     - 
  */

// number of LEDs total
#define led_string_length 100
// number of same coloured pixel groups
#define num_groups 3
// number of members in a group
#define group_length 30

#include <Adafruit_NeoPixel.h>

// arduino pin
#define PIN        6

// taken from neopixel example
Adafruit_NeoPixel pixels(led_string_length, PIN, NEO_GRB + NEO_KHZ800);

typedef struct Pixels {
	byte group;
	// address is done with chain[i] so not needed here
	byte red; // 0 to 255
	byte green; // 0 to 255
	byte blue; // 0 to 255
	byte curr_brightness; // in per cent
	byte max_brightness; // in per cent
} Pixel;

Pixel chain[led_string_length];

void setup() {
    Serial.begin(115200);

	for(int i = 0; i < 5; i++) {
		Serial.println(i);
		delay(100);
	}
	
	Serial.println(F("sizeof chain = "));
	Serial.println(sizeof(chain));	
    // init ws2813 strip
    pixels.begin();
	
	// default chain values
	for(int i = 0; i < led_string_length; i++) {
		chain[i].group = 255;
		chain[i].red = 0;
		chain[i].green = 0;
		chain[i].blue = 0;
		chain[i].curr_brightness = 0;
		chain[i].max_brightness = 0;
	}

	// first time group setup
	byte idx, max_brightness, red, green, blue = 0;
	for(int group = 0; group < num_groups; group++) {
		// use brightnesses between 5 and 40%
		// 40% max is good from testing with adafruit_neopixel.ino
		max_brightness = random(5, 40);
		red = random(0, 255);
		green = random(0, 255);
		blue = random(0, 255);
		for(int member = 0; member < group_length; member++) {
			idx = random(0, led_string_length);
			// check if idx is already populated
			if(chain[idx].max_brightness == 0) {
				chain[idx].group = group;
				chain[idx].red = red;
				chain[idx].green = green;
				chain[idx].blue = blue;
				chain[idx].curr_brightness = (100 * (chain[idx].group)) / (num_groups);
				chain[idx].max_brightness = max_brightness;
			} else {
				member--;
			}
		}
	}
	
	// now stagger the brightnesses of all the groups
	for(int i = 0; i < led_string_length; i++) {
		if(chain[i].group != 255) {
			chain[i].curr_brightness = (100 * (chain[i].group)) / (num_groups);
		}
	}
	
	Serial.println(F("debug print loop start"));
	// check the setup by printing the array vals to serial
	for(int i = 0; i < led_string_length-7; i=i+7) {
		for(int j = 0; j < 7; j++) {
			Serial.print(F("("));
			Serial.print(i+j);
			Serial.print(F("),"));
			Serial.print(chain[i + j].group, DEC);
			Serial.print(F(","));
			Serial.print(chain[i + j].curr_brightness, DEC);
			Serial.print(F(","));
			Serial.print(chain[i + j].red, DEC);
			Serial.print(F("\t"));
		}
		Serial.println();
	}
	Serial.println(F("setup loop complete"));
}

void loop() {
    // turn off all pixels
    pixels.clear();
	// reset the chain values
	// for(int i = 0; i < led_string_length; i++) {
		// chain[i].red = 0;
		// chain[i].green = 0;
		// chain[i].blue = 0;
		// chain[i].curr_brightness = 0;
		// chain[i].max_brightness = 0;
	// }
	
	
	// // new groups setup
	// int idx, max_brightness = 0;
	// for(int group = 0; group < num_groups; group++) {
		// // use brightnesses between 5 and 40%
		// // 40% max is good from testing with adafruit_neopixel.ino
		// max_brightness = random(5, 40);
		// for(int member = 0; member < group_length; member++) {
			// idx = random(0, led_string_length);
			// // check if idx is already populated
			// if(chain[idx].max_brightness != 0) {
				// continue;
			// }
		// }
	// }
}


