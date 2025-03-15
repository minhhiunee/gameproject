#include "Bullet.h"

Bullet::Bullet() : x(0), y(0), active(false) {}

void Bullet::update() {
    if (active) {
        x += 10;
    }
}

void Bullet::shoot(int startX, int startY) {
    x = startX;
    y = startY;
    active = true;
}
