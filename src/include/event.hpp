
#ifndef __EVENT_H_
#define __EVENT_H_

#include <iostream>
#include <string>


using namespace std;

class Event{

public:
    Event() { cout << "init an empty events" << endl; }
    Event(double ts, int x, int y): x(x), y(y), ts(ts)  {}
    void print(void){cout << "[" << ts << ", " << x << ", " << y << "]" << endl;}
    string info(void) const { return ("(" + to_string(int(ts * 1e3)) + ", " + to_string(x) + ", " + to_string(y) + ")"); }

    Event &operator=(const Event &e) { this->x = x, this->y = y, this->ts = ts; }

public:
    int x;
    int y;
    // int p;
    double ts;      // ts in ms
};


// class EventStream{
// public:
//     EventStream(){};
//     EventStream(vector<Event> events) : events(events) {}
// public:
//     vector<Event> events;
// }


#endif
