#ifndef BULLET_H
#define BULLET_H

class Bullet {
private:
    int x, y;
    bool active;

public:
    Bullet();
    void update();
    void shoot(int startX, int startY);
    bool isActive() const { return active; }
    int getX() const { return x; }
    int getY() const { return y; }
    void deactivate() { active = false; }
};

#endif
