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
// #define led_string_length 432
#define led_string_length 100
// number of same coloured pixel groups

// number of members in a group
// #define group_length 15

#include <Adafruit_NeoPixel.h>

// arduino pin
#define PIN        6

// taken from neopixel example
Adafruit_NeoPixel pixels(led_string_length, PIN, NEO_GRB + NEO_KHZ800);

typedef struct Pixels {
	// address is done with chain[i] so not needed here
	byte red; // 0 to 255
	byte green; // 0 to 255
	byte blue; // 0 to 255
	byte curr_brightness; // in per cent
	byte max_brightness; // in per cent
} Pixel;

Pixel chain[led_string_length];

byte idx, max_brightness, red, green, blue = 0;

void group_setup(int num_groups, int group_length) {
	
	for(int i = 0; i < led_string_length; i++) {
		chain[i].red = 0;
		chain[i].green = 0;
		chain[i].blue = 0;
		chain[i].curr_brightness = 0;
		chain[i].max_brightness = 0;
	}
	
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
				chain[idx].red = red;
				chain[idx].green = green;
				chain[idx].blue = blue;
				chain[idx].curr_brightness = (100 * group) / (num_groups);
				chain[idx].max_brightness = max_brightness;
			} else {
				member--;
			}
		}
	}
	
	// // now stagger the brightnesses of all the groups
	// for(int i = 0; i < led_string_length; i++) {
		// if(chain[i].max_brightness != 0) {
			// chain[i].curr_brightness = (100 * (chain[i].group)) / (num_groups);
		// }
	// }
}

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
		chain[i].red = 0;
		chain[i].green = 0;
		chain[i].blue = 0;
		chain[i].curr_brightness = 0;
		chain[i].max_brightness = 0;
	}

	// first time group setup
	group_setup(3, 10);
	
	Serial.println(F("debug print loop start"));
	// check the setup by printing the array vals to serial
	for(int i = 0; i < led_string_length-7; i=i+7) {
		for(int j = 0; j < 7; j++) {
			Serial.print(F("("));
			Serial.print(i+j);
			Serial.print(F("),"));
			Serial.print(chain[i + j].curr_brightness, DEC);
			Serial.print(F(","));
			Serial.print(chain[i + j].red, DEC);
			Serial.print(F("\t"));
		}
		Serial.println();
	}
	Serial.println(F("setup loop complete"));
}

void single_flash() {
	
	int num_groups = 3;
	int group_length = 10;
	
    // turn off all pixels
    pixels.clear();
	
	Serial.println(F("group_setup"));
	
	group_setup(num_groups, group_length);
	
	Serial.println(F("out of group_setup"));
	
	int b = 0;
	int i = 0;
	for (b = 0; b < 40; b++) {
		for(i = 0; i < led_string_length; i++) {
			pixels.setPixelColor(i, pixels.Color(
				(chain[i].red * b)/400, 
				(chain[i].green * b)/400, 
				(chain[i].blue * b)/400));
		}
		pixels.show();
		delay(5);
	}
	
	for (b = 40; b > 0; b--) {
		for(i = 0; i < led_string_length; i++) {
			pixels.setPixelColor(i, pixels.Color(
				(chain[i].red * b)/400, 
				(chain[i].green * b)/400, 
				(chain[i].blue * b)/400));
		}
		pixels.show();
		delay(5);
	}
}

void weird_effect_fast() {
		
	group_setup(10, 10);
		
	byte new_r = random(0, 255);
	byte new_g = random(0, 255);
	byte new_b = random(0, 255);
	byte max_brightness = random(1, 10);
	// check if any groups have flashed once and get new rgb and max_b vals for those pixels
	for(int i = 0; i < led_string_length; i++) {
		if(chain[i].curr_brightness >= 50) {
			chain[i].red = new_r;
			chain[i].green = new_g;
			chain[i].blue = new_b;
			chain[i].max_brightness = max_brightness;
			chain[i].curr_brightness = 0;
		} else {
			chain[i].curr_brightness++;
		}
	}
	
	for(int i = 0; i < led_string_length; i++) {
		pixels.setPixelColor(i, 
			pixels.Color(
				(chain[i].red * chain[i].curr_brightness)/200, 
				(chain[i].green * chain[i].curr_brightness)/200, 
				(chain[i].blue * chain[i].curr_brightness)/200
			)
		);
	}
	
	pixels.show();
	delay(5);
}

void weird_effect() {
	
	group_setup(10, 10);
	
	byte new_r = random(0, 255);
	byte new_g = random(0, 255);
	byte new_b = random(0, 255);
	byte max_brightness = random(1, 10);
	// check if any groups have flashed once and get new rgb and max_b vals for those pixels
	for(int i = 0; i < led_string_length; i++) {
		if(chain[i].curr_brightness >= 30 && chain[i].max_brightness != 0) {
			chain[i].red = new_r;
			chain[i].green = new_g;
			chain[i].blue = new_b;
			chain[i].max_brightness = max_brightness;
			chain[i].curr_brightness = 0;
		} else if(chain[i].max_brightness != 0) {
			chain[i].curr_brightness++;
		}
	}
	
	for(int i = 0; i < led_string_length; i++) {
		if(chain[i].max_brightness != 0) {
			pixels.setPixelColor(i, 
				pixels.Color(
					(chain[i].red * chain[i].curr_brightness)/200, 
					(chain[i].green * chain[i].curr_brightness)/200, 
					(chain[i].blue * chain[i].curr_brightness)/200
				)
			);			
		} else {
			pixels.setPixelColor(i, 
				pixels.Color(0,0,0)
			);
		}
	}
	
	pixels.show();
	delay(20);
}

void loop() {
	
	for(int i = 0; i < 100; i++) {
		weird_effect();
	}
	for(int i = 0; i < led_string_length; i++) {
		pixels.setPixelColor(i,pixels.Color(0,0,0));
	}
	pixels.show();
	delay(1000);
	for(int i = 0; i < 20; i++) {
		single_flash();
	}
	for(int i = 0; i < led_string_length; i++) {
		pixels.setPixelColor(i,pixels.Color(0,0,0));
	}
	pixels.show();
	delay(1000);
}


