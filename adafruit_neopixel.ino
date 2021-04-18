#include <Adafruit_NeoPixel.h>

// arduino pin
#define PIN        6

#define led_string_length 432

// taken from neopixel example
Adafruit_NeoPixel pixels(led_string_length, PIN, NEO_GRB + NEO_KHZ800);

// global ms delay
#define delayval 100

// min sustainable constant led brightness out of 255
#define min_brightness 5
#define max_brightness 80

void setup() {
	Serial.begin(115200);
	// init ws2813 strip
	pixels.begin();
}

// test function that turns on all pixels
void turn_all_on() {
	for(int i = 0; i < led_string_length; i++) {
		pixels.setPixelColor(i, pixels.Color(random(0, 8), random(0, 8), random(0, 8)));
		pixels.show();
		Serial.println(i, DEC);
		delay(delayval);
	}
}

int scale_brightness(int fade_level, int value_to_scale) {
	double percentage = (double)fade_level/(double)max_brightness;
	// Serial.println("percent");
	// Serial.println(percentage, DEC);
	// Serial.println("(int)((double)percentage * value_to_scale)");
	// Serial.println((int)((double)percentage * value_to_scale), DEC);
	return((int)((double)percentage * value_to_scale));
}

// speed range: 5 to 50
void single_colour_fade(uint32_t num_pixels, int speed) {
	
	uint32_t to_set[num_pixels];
	
	// time to delay for is inversely proportional to speed of flashing
	int max_speed = 50;
	int speed_delay = max_speed - speed;
	
	// generate random leds to flash
	for(int i = 0; i < num_pixels; i++) {
		to_set[i] = random(0, led_string_length);
	}
	
	// TODO shift one/threads number of total pixels by one third of max brightness
	// mod with max brightness so that something that starts bright goes back to dark again
	
	int red = random(0, max_brightness);
	int green = random(0, max_brightness);
	int blue = random(0, max_brightness);
	// special conditions for slow fade to reduce jitter at low brightnesses
	if(speed <= 10) {
		// fade in
			Serial.println("fade in");
			Serial.println(speed, DEC);
		for(int fade = min_brightness; fade < max_brightness; fade++) {
			// set pixels
			// Serial.println(fade, DEC);
			int fade_red = scale_brightness(fade, red);
			int fade_green = scale_brightness(fade, green);
			int fade_blue = scale_brightness(fade, blue);
			for(int i = 0; i < num_pixels; i++) {
				pixels.setPixelColor(to_set[i], pixels.Color(fade_red, fade_green, fade_blue));
			}
			pixels.show();
			delay(speed_delay/5);
		}
		
		// fade out
			Serial.println("fade out");
			Serial.println(speed, DEC);
		for(int fade = max_brightness; fade > min_brightness; fade--) {
			// set pixels
			// Serial.println(fade, DEC);
			int fade_red = scale_brightness(fade, red);
			int fade_green = scale_brightness(fade, green);
			int fade_blue = scale_brightness(fade, blue);
			for(int i = 0; i < num_pixels; i++) {
				pixels.setPixelColor(to_set[i], pixels.Color(fade_red, fade_green, fade_blue));
			}
			pixels.show();
			delay(speed_delay/5);
		}		
	} else {
		// fade in
			Serial.println("fade in");
			Serial.println(speed, DEC);
		for(int fade = min_brightness; fade < max_brightness; fade=fade+5) {
			// set pixels
			// Serial.println(fade, DEC);
			int fade_red = scale_brightness(fade, red);
			int fade_green = scale_brightness(fade, green);
			int fade_blue = scale_brightness(fade, blue);
			for(int i = 0; i < num_pixels; i++) {
				pixels.setPixelColor(to_set[i], pixels.Color(fade_red, fade_green, fade_blue));
			}
			pixels.show();
			delay(speed_delay);
		}
		
		// fade out
			Serial.println("fade out");
			Serial.println(speed, DEC);
		for(int fade = max_brightness; fade > min_brightness; fade=fade-5) {
			// set pixels
			// Serial.println(fade, DEC);
			int fade_red = scale_brightness(fade, red);
			int fade_green = scale_brightness(fade, green);
			int fade_blue = scale_brightness(fade, blue);
			for(int i = 0; i < num_pixels; i++) {
				pixels.setPixelColor(to_set[i], pixels.Color(fade_red, fade_green, fade_blue));
			}
			pixels.show();
			delay(speed_delay);
		}	
	}
	
}

// speed range: 5 to 50
void threaded_fade(uint32_t num_pixels, int speed, int threads) {

	uint32_t pixel_address[num_pixels];
	// max brightness of pixel
	uint32_t pixel_max_brightness[num_pixels];
	// start brightness of pixel
	uint32_t pixel_start_brightness[num_pixels];
	uint32_t pixel_colour[num_pixels];
	
	// set the led number in the string
	for(int i = 0; i < num_pixels; i++) {
		to_set[i] = random(0, led_string_length);
	}

	// set random start values of pixels in groups of num_pixels/threads
	// i.e. first third of pixels start at zero, second third start at max_brightness/3, 
	// last third start at 2/3 of max_brightness
	
	
}

void loop() {
	// turn off all pixels
	pixels.clear();
	for(int i = 0; i < 5; i++) {
		single_colour_fade(50, 50);
		pixels.clear();
	}
	for(int i = 0; i < 5; i++) {
		single_colour_fade(50, 20);
		pixels.clear();
	}	
	for(int i = 0; i < 5; i++) {
		single_colour_fade(50, 10);
		pixels.clear();
	}	
	for(int i = 0; i < 5; i++) {
		single_colour_fade(50, 5);
		pixels.clear();
	}
}