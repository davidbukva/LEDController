
#include <Adafruit_NeoPixel.h>


#define PIN      6

#define NUMPIXELS 38

#define NUMPARTICLES 6

struct Particle{
  uint8_t pos;
  uint8_t speed = 1;
  int8_t dir = 1;
  uint32_t color;
};
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_BRG + NEO_KHZ800);


Particle randomParticle(){
  Particle p;
  p.pos = random()%NUMPIXELS;
  p.dir = (random()%2)*2 - 1;
  p.speed = random()%3 + 2;
  p.color = maxColor(pixels.gamma32(random() % 0x00ffffff));

  

  return p;
}

Particle parts[NUMPARTICLES];

void setup() {
  pixels.begin(); 
  pixels.setBrightness(255);
  pixels.clear();

  for(int i = 0; i < NUMPARTICLES; i++){
    parts[i] = randomParticle();
  }
  
}

int time = 0;

void loop() {
  time++;

  for(int i = 0; i < NUMPIXELS; i++){
    pixels.setPixelColor(i, colorLerp(0.15, pixels.getPixelColor(i), 0));
  }

  for(int i = 0; i < NUMPARTICLES; i++){
    if(time % parts[i].speed != 0)continue;
    parts[i].pos += parts[i].dir;
    if(parts[i].pos < 0 || parts[i].pos > NUMPIXELS){
      parts[i] = randomParticle();
    }
    pixels.setPixelColor(parts[i].pos, colorAdd(0.75, 0.75, pixels.getPixelColor(i), parts[i].color));
    
  }


  pixels.show();
  delay(25);
}

uint32_t maxColor(uint32_t color){
  double r = (color >> 16) & 0xff;
  double g = (color >> 8) & 0xff;
  double b = (color >> 0) & 0xff;

  double ratio = 255./max(r, max(g, b));

  int r_int = min((int)(r*ratio), 255);
  int g_int = min((int)(g*ratio), 255);
  int b_int = min((int)(b*ratio), 255);

  return pixels.Color(r_int, g_int, b_int);

}

uint32_t colorAdd(float p1, float p2, uint32_t c1, uint32_t c2){
  int r1 = (c1 >> 16) & 0xff;
  int g1 = (c1 >> 8) & 0xff;
  int b1 = (c1 >> 0) & 0xff;

  int r2 = (c2 >> 16) & 0xff;
  int g2 = (c2 >> 8) & 0xff;
  int b2 = (c2 >> 0) & 0xff;

  int r = min(255, p1 * r1 + p2 * r2);
  int g = min(255, p1 * g1 + p2 * g2);
  int b = min(255, p1 * b1 + p2 * b2);

  return pixels.Color(r, g, b);
}

uint32_t colorLerp(float percent, uint32_t c1, uint32_t c2){
  int r1 = (c1 >> 16) & 0xff;
  int g1 = (c1 >> 8) & 0xff;
  int b1 = (c1 >> 0) & 0xff;

  int r2 = (c2 >> 16) & 0xff;
  int g2 = (c2 >> 8) & 0xff;
  int b2 = (c2 >> 0) & 0xff;

  int r = (1. - percent) * r1 + percent * r2;
  int g = (1. - percent) * g1 + percent * g2;
  int b = (1. - percent) * b1 + percent * b2;

  return pixels.Color(r, g, b);
}

