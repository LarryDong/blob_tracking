#ifndef __BLOB_HPP_
#define __BLOB_HPP_

#include <iostream>
#include <vector>
#include "event.hpp"

using namespace std;

class Blob{
public:
    Blob() { cout << "Blob empyt init not allowed." << endl; std::abort(); }
    Blob(const Event& e);

    void updateBlob(void);
    int addEvent(Event evt);

    inline void printInfo(void) const { 
        cout << "Blob id: " << id_ << ", active: "<<is_active_<<", at (" << x_ << ", " << y_ << "), events: " << events_.size() << endl;
    }

public:
    // static int next_id_;
    int id_;
    vector<Event> events_;
    double x_, y_;
    double current_ts_, dt_;        // dt_, duration
    bool is_active_;
    // double r_;
};


class BlobManager{
public:
    BlobManager();
    int checkBlob(const Event& e);
    int createBlob(const Event& e);
    int updateAllBlobs(void);

    inline void printBlobInfo(void) {
        cout << "---------------" << endl;
        cout << "Total Blobs: " << blobs_.size() << endl;
        for(int i=0; i<blobs_.size(); ++i){
            cout <<"No: " << i <<" blob: ";
            blobs_[i].printInfo();
        }
    }

public:
    vector<Blob> blobs_;
    double radius_;

private:
    inline bool checkInRange(const Event &e, const Blob &b, double r) const { return (abs(e.x - b.x_) < r && abs(e.y - b.y_) < r); }
};



#endif
