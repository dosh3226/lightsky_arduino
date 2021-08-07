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
#define led_string_length 387
// number of same coloured pixel groups

// number of members in a group
// #define group_length 15

#include <Adafruit_NeoPixel.h>

// arduino pin
#define PIN        6

// taken from neopixel example
Adafruit_NeoPixel pixels(led_string_length, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
    Serial.begin(115200);

	for(int i = 0; i < 5; i++) {
		Serial.println(i);
		delay(100);
	}
	
    // init ws2813 strip
    pixels.begin();
	
	// default pixel values
	for(int i = 0; i < led_string_length; i++) {
		pixels.setPixelColor(i,pixels.Color(0,0,0));
	}

	Serial.println(F("setup loop complete"));
}

void slow_flash_no_struct(int num_groups, int group_length) {
	
	for(int i = 0; i < led_string_length; i++) {
		pixels.setPixelColor(i,pixels.Color(0,0,0));
	}
	
	// group_setup(10, 10);

	// brightness, r, g, b one byte each
	uint16_t pixels_addr[num_groups][group_length];
	uint32_t pixels_color[num_groups];
	int existing = 0;
	int new_addr = 0;
	byte r, g, b = 0;
	
	Serial.println(F("slow_flash_no_struct setup"));
	
	for(int i = 0; i < num_groups; i++) {
		r = random(0, 255);
		g = random(0, 255);
		b = random(0, 255);
		pixels_color[i] = r << 16;
		pixels_color[i] |= g << 8;
		pixels_color[i] |= b;
		Serial.println("pixels colour [i] = ");
		Serial.println(pixels_color[i], (DEC));
		Serial.println(i, (DEC));
		for(int j = 0; j < group_length; j++) {
			existing = 0;
			// check if address is already in use, try again if so
			while(1) {
				existing = 0;
				new_addr = random(0, led_string_length);
				for(int k = 0; k < i; k++) {
					for(int l = 0; l < j; l++) {
						if(pixels_addr[k][l] == new_addr) {
							existing = 1;
							break;
						}
					}
					if(existing) { break; }
				}
				if(!existing) { break; }
			}
			pixels_addr[i][j] = new_addr;
		}
	}
	
	Serial.println(F("slow_flash_no_struct checking"));
	
	uint16_t div = 200;
	
	for (byte brightness = 0; brightness < 40; brightness++) {
		int i = 0;
		for(i = 0; i < num_groups; i++) {
			r = (uint32_t)((uint8_t)(pixels_color[i] >> 16) * (uint32_t)brightness)/div;
			g = (uint32_t)((uint8_t)(pixels_color[i] >> 8) * (uint32_t)brightness)/div;
			b = (uint32_t)((uint8_t)(pixels_color[i]) * (uint32_t)brightness)/div;
			for(int j = 0; j < group_length; j++) {
				pixels.setPixelColor(pixels_addr[i][j],
					pixels.Color(
						r,
						g,
						b
					)
				);
			}
		}
		//Serial.println(r, (DEC));
		//Serial.println(g, (DEC));
		//Serial.println(b, (DEC));
		//Serial.println("");
		pixels.show();
		delay(5);
	}
	
	for (byte brightness = 40; brightness > 0; brightness--) {
		int i = 0;
		for(i = 0; i < num_groups; i++) {
			r = (uint32_t)((uint8_t)(pixels_color[i] >> 16) * (uint32_t)brightness)/div;
			g = (uint32_t)((uint8_t)(pixels_color[i] >> 8) * (uint32_t)brightness)/div;
			b = (uint32_t)((uint8_t)(pixels_color[i]) * (uint32_t)brightness)/div;
			for(int j = 0; j < group_length; j++) {
				pixels.setPixelColor(pixels_addr[i][j],
					pixels.Color(
						r,
						g,
						b
					)
				);
			}
		}
		//Serial.println(r, (DEC));
		//Serial.println(g, (DEC));
		//Serial.println(b, (DEC));
		//Serial.println("");
		pixels.show();
		delay(5);
	}
	
	Serial.println(F("slow_flash_no_struct complete"));
}

void fast_flash_no_struct(int num_groups, int group_length) {
	
	for(int i = 0; i < led_string_length; i++) {
		pixels.setPixelColor(i,pixels.Color(0,0,0));
	}
	
	// group_setup(10, 10);

	// brightness, r, g, b one byte each
	uint16_t pixels_addr[num_groups][group_length];
	uint32_t pixels_color[num_groups];
	int existing = 0;
	int new_addr = 0;
	byte r, g, b = 0;
	
	Serial.println(F("slow_flash_no_struct setup"));
	
	for(int i = 0; i < num_groups; i++) {
		r = random(0, 255);
		g = random(0, 255);
		b = random(0, 255);
		pixels_color[i] = r << 16;
		pixels_color[i] |= g << 8;
		pixels_color[i] |= b;
		//Serial.println("pixels colour [i] = ");
		//Serial.println(pixels_color[i], (DEC));
		//Serial.println(i, (DEC));
		for(int j = 0; j < group_length; j++) {
			existing = 0;
			// check if address is already in use, try again if so
			while(1) {
				existing = 0;
				new_addr = random(0, led_string_length);
				for(int k = 0; k < i; k++) {
					for(int l = 0; l < j; l++) {
						if(pixels_addr[k][l] == new_addr) {
							existing = 1;
							break;
						}
					}
					if(existing) { break; }
				}
				if(!existing) { break; }
			}
			pixels_addr[i][j] = new_addr;
		}
	}
	
	Serial.println(F("slow_flash_no_struct checking"));
	
	uint16_t div = 150;
	
	for (byte brightness = 0; brightness < 15; brightness++) {
		int i = 0;
		for(i = 0; i < num_groups; i++) {
			r = (uint32_t)((uint8_t)(pixels_color[i] >> 16) * (uint32_t)brightness)/div;
			g = (uint32_t)((uint8_t)(pixels_color[i] >> 8) * (uint32_t)brightness)/div;
			b = (uint32_t)((uint8_t)(pixels_color[i]) * (uint32_t)brightness)/div;
			for(int j = 0; j < group_length; j++) {
				pixels.setPixelColor(pixels_addr[i][j],
					pixels.Color(
						r,
						g,
						b
					)
				);
			}
		}
		//Serial.println(r, (DEC));
		//Serial.println(g, (DEC));
		//Serial.println(b, (DEC));
		//Serial.println("");
		pixels.show();
		delay(5);
	}
	
	for (byte brightness = 15; brightness > 0; brightness--) {
		int i = 0;
		for(i = 0; i < num_groups; i++) {
			r = (uint32_t)((uint8_t)(pixels_color[i] >> 16) * (uint32_t)brightness)/div;
			g = (uint32_t)((uint8_t)(pixels_color[i] >> 8) * (uint32_t)brightness)/div;
			b = (uint32_t)((uint8_t)(pixels_color[i]) * (uint32_t)brightness)/div;
			for(int j = 0; j < group_length; j++) {
				pixels.setPixelColor(pixels_addr[i][j],
					pixels.Color(
						r,
						g,
						b
					)
				);
			}
		}
		//Serial.println(r, (DEC));
		//Serial.println(g, (DEC));
		//Serial.println(b, (DEC));
		//Serial.println("");
		pixels.show();
		delay(5);
	}
	
	Serial.println(F("slow_flash_no_struct complete"));
}
// 20 for max speed, higher for slower speeds
void weird_effect_no_struct(int num_groups, int group_length, int timer) {
	
	for(int i = 0; i < led_string_length; i++) {
		pixels.setPixelColor(i,pixels.Color(0,0,0));
	}
	
	// group_setup(10, 10);

	// brightness, r, g, b one byte each
	uint16_t pixels_addr[num_groups][group_length];
	uint32_t pixels_color[num_groups];
	int existing = 0;
	int new_addr = 0;
	byte r, g, b = 0;
	for(int i = 0; i < num_groups; i++) {
		r = random(0, 255);
		g = random(0, 255);
		b = random(0, 255);
		pixels_color[i] = r << 16;
		pixels_color[i] |= g << 8;
		pixels_color[i] |= b;
		for(int j = 0; j < group_length; j++) {
			existing = 0;
			// check if address is already in use, try again if so
			while(1) {
				existing = 0;
				new_addr = random(0, led_string_length);
				for(int k = 0; k < i; k++) {
					for(int l = 0; l < j; l++) {
						if(pixels_addr[k][l] == new_addr) {
							existing = 1;
							break;
						}
					}
					if(existing) { break; }
				}
				if(!existing) { break; }
			}
			pixels_addr[i][j] = new_addr;
		}
	}
	
	byte brightness = 150;
	uint16_t div = 150;
	
	for(int i = 0; i < num_groups; i++) {
		r = (uint32_t)((uint8_t)(pixels_color[i] >> 16) * (uint32_t)brightness)/div;
		g = (uint32_t)((uint8_t)(pixels_color[i] >> 8) * (uint32_t)brightness)/div;
		b = (uint32_t)((uint8_t)(pixels_color[i]) * (uint32_t)brightness)/div;
		for(int j = 0; j < group_length; j++) {
			pixels.setPixelColor(pixels_addr[i][j],
				pixels.Color(
					r,
					g,
					b
				)
			);
		}
	}
	pixels.show();
	delay(timer);
}

void loop() {
	for(int i = 0; i < led_string_length; i++) {
		pixels.setPixelColor(i,pixels.Color(15,0,0));
	}
	pixels.show();
	delay(500);
	
	for(int i = 0; i < 20; i++) {
		fast_flash_no_struct(3, 10);
	}

	for(int i = 0; i < 20; i++) {
		fast_flash_no_struct(3, 50);
	}
	for(int i = 0; i < 20; i++) {
		fast_flash_no_struct(3, 75);
	}
	for(int i = 0; i < 20; i++) {
		fast_flash_no_struct(10, 10);
	}
	for(int i = 0; i < 20; i++) {
		fast_flash_no_struct(3, 20);
	}
	
	pixels.setBrightness(30);
	for(int i = 0; i < 300; i++) {
		weird_effect_no_struct(100, 1, 20);
	}
	pixels.setBrightness(255);
	
	for(int i = 0; i < 5; i++) {
		slow_flash_no_struct(1, 1);
	}
	for(int i = 0; i < 5; i++) {
		slow_flash_no_struct(10, 1);
	}
	for(int i = 0; i < 5; i++) {
		slow_flash_no_struct(10, 10);
	}
	for(int i = 0; i < 5; i++) {
		slow_flash_no_struct(80, 1);
	}
}


