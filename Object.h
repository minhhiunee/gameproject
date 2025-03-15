#ifndef OBJECT_H
#define OBJECT_H

class Object {
private:
    int x, y;
    bool active;
    static const int OBJECT_SIZE = 20;

public:
    Object(int windowWidth, int windowHeight);
    void update();
    int getX() const { return x; }
    int getY() const { return y; }
    bool isActive() const { return active; }
    static int getSize() { return OBJECT_SIZE; }
    void reset(int windowWidth);
};

#endif
