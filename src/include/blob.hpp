#ifndef __BLOB_HPP_
#define __BLOB_HPP_

#include <iostream>
#include <vector>
#include <deque>

#include "event.hpp"

using namespace std;

class Blob{
public:
    Blob() { cout << "Blob empyt init not allowed." << endl; std::abort(); }
    Blob(const Event& e);
    ~Blob() {}
    void updateBlob(double ts);
    int addEvent(const Event& evt);            // 
    bool checkAndSetDead(double ts);          // check the blob is dead or not.

    inline void printInfo(void) const { 
        cout << "id: " << id_ << ", inited/dead:"<<is_inited_<<"/"<<is_dead_<<", (" << x_ << ", " << y_ << "), active: " << active_events_number_ << endl;
    }

    Blob &operator=(const Blob &b) { 
        this->id_ = b.id_; 
        this->events_ = b.events_;
        this->x_ = b.x_;
        this->y_ = b.y_;
        this->current_ts_ = b.current_ts_;
        this->dt_ = b.dt_;
        this->is_inited_ = b.is_inited_;
        this->is_dead_ = b.is_dead_;
        this->active_events_number_ = b.active_events_number_;
    }

public:
    static int next_id_;
    int id_;
    deque<Event> events_;
    double x_, y_;
    double current_ts_, dt_;
    bool is_inited_, is_dead_;
    int active_events_number_;
};


class BlobManager{
public:
    BlobManager(int radius);
    int checkBlob(const Event& e);
    int createBlob(const Event& e);
    int findNearestBlob(const Event &e);
    int updateAllBlobs(double ts);
    deque<Blob> getActiveBlobs(void) const;
    int removeDeadBlobs(double ts);

    inline void printBlobInfo(bool only_active) {
        cout << "---------------" << endl;
        if(only_active){
            deque<Blob> active_blobs = getActiveBlobs();
            cout << "Valid blobs: " << active_blobs.size() << endl;
            for(int i=0; i<active_blobs.size(); ++i){
                active_blobs[i].printInfo();
            }
            return ;
        }
        cout << "Total Blobs: " << blobs_.size() << endl;
        for(int i=0; i<blobs_.size(); ++i){
            cout <<"No: " << i <<" blob: ";
            blobs_[i].printInfo();
        }
    }

public:
    deque<Blob> blobs_;
    double radius_;

private:
    inline bool checkInRange(const Event &e, const Blob &b, double r) const { return ((abs(e.x - b.x_) < r && abs(e.y - b.y_) < r)); }
};



#endif
