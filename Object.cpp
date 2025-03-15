#include "Object.h"

Object::Object(int windowWidth, int windowHeight)
    : x(windowWidth),
      y(windowHeight - OBJECT_SIZE - 50),
      active(true) {}

void Object::update() {
    x -= 5;
}

void Object::reset(int windowWidth) {
    x = windowWidth;
}
