#ifndef __EVENT_H_
#define __EVENT_H_

#include <iostream>
#include <string>

using namespace std;

class Event{
public:
    Event() {}
    Event(double ts, int x, int y): x(x), y(y), ts(ts)  {}
    ~Event() {}
    void print(void) const {cout << "[" << ts << ", " << x << ", " << y << "]" << endl;}
    string info(void) const { return ("(" + to_string(int(ts * 1e3)) + ", " + to_string(x) + ", " + to_string(y) + ")"); }
    Event &operator=(const Event &e) {this->x = e.x; this->y = e.y; this->ts = e.ts;}

public:
    int x;
    int y;
    double ts;      // ts in ms
};

#endif
