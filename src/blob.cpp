
#include "blob.hpp"


Blob::Blob(const Event& e)
        :x_(e.x), y_(e.y), current_ts_(e.ts), 
        dt_(1e3), is_active_(false), id_(0){
    events_ = vector<Event>();                  // clear the queue
    events_.push_back(e);                       // add the first events
    cout << "New Blob! Added the first event to a blob. Event: " << e.info() << endl;
    printInfo();
}

void Blob::updateBlob(void){              // update blob's information when called.
    // update x_, y_, status;
    cout << " updating blob: " << id_ << endl;
    double sum_x=0, sum_y = 0;
    int active_event_count = 0;
    for (int i=0; i<events_.size(); ++i){
        Event e = events_[i];
        if (e.ts < current_ts_ - dt_)        // skip old events.
            continue;
        active_event_count++;
        cout << sum_x << endl;
        sum_x += e.x;
        sum_y += e.y;
    }
    x_ = sum_x/events_.size();
    y_ = sum_y/events_.size();

    if(active_event_count > 5)              // check is active?
        is_active_ = true;
}

int Blob::addEvent(Event evt){
    events_.push_back(evt);
    dt_ = evt.ts;
}



////////////////  BlobManager  ////////////////
BlobManager::BlobManager(void) : radius_(10), blobs_(vector<Blob>()) { ; }

int BlobManager::checkBlob(const Event& e){
    cout << "--> Check: " << e.info() << " is in existing " << blobs_.size() <<" blobs or not..." << endl;
    int id = -1;
    for (int i=0; i<blobs_.size(); ++i){      // TODO: what if an event belongs to two blob??
        if(checkInRange(e, blobs_[i], radius_)){
            cout << "<-- Yes! belong to blob: " << i << endl;
            id = i;
            break;
        }
    }
    return id;
}

int BlobManager::createBlob(const Event& e){
    blobs_.push_back(Blob(e));
    return blobs_.size()-1;             // return new_blob's id.
}


int BlobManager::updateAllBlobs(void){
    for(int i=0; i<blobs_.size(); ++i){         // Attention! Can't use for(auto b:blobs), this is only a reference.
        blobs_[i].updateBlob();
    }
}
