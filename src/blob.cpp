
#include "blob.hpp"

int Blob::next_id_ = 0;
Blob::Blob(const Event& e)
        :x_(e.x), 
        y_(e.y), 
        current_ts_(e.ts), 
        dt_(1e2), 
        is_inited_(false), 
        is_dead_(false), 
        id_(-1),
        active_events_number_(0)
{
    events_ = vector<Event>();                  // clear the queue
    events_.push_back(e);                       // add the first events
}

void Blob::updateBlob(void){              // update blob's information when called.
    // update x_, y_, status;
    // cout << " updating blob: " << id_ << endl;
    double sum_x=0, sum_y = 0;
    int active_event_count = 0;
    int counter = 0;
    for (int i=events_.size()-1; i>=0; --i){
        Event e = events_[i];
        if(e.ts > current_ts_ - dt_){
            counter++;
            sum_x += e.x;
            sum_y += e.y;
        }
        else{
            break;
        }
    }
    active_events_number_ = counter;
    x_ = sum_x/active_events_number_;
    y_ = sum_y/active_events_number_;

    int th = 200;
    if(!is_inited_){
        if(active_events_number_>=th){
            is_inited_ = true;
            id_ = next_id_++;
        }
    }
}

int Blob::addEvent(Event evt){
    events_.push_back(evt);
    current_ts_ = evt.ts;
}

bool Blob::checkDead(double ts){
    int counter = 0;
    for (int i=events_.size()-1; i>=0; --i){
        Event e = events_[i];
        if (e.ts > ts - dt_){
            counter++;
        }
        else
            break;
    }
    int th = 200;
    if(counter >= th)
        is_dead_ = false;
    else
        is_dead_ = true;
    return is_dead_;
}



////////////////  BlobManager  ////////////////
BlobManager::BlobManager(void) : radius_(60), blobs_(vector<Blob>()) { ; }

int BlobManager::checkBlob(const Event& e){
    // cout << "--> Check: " << e.info() << " is in existing " << blobs_.size() <<" blobs or not..." << endl;
    int id = -1;
    for (int i=0; i<blobs_.size(); ++i){      // TODO: what if an event belongs to two blob??
        if(checkInRange(e, blobs_[i], radius_)){
            // cout << "<-- Yes! belong to blob: " << i << endl;
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


vector<Blob> BlobManager::getActiveBlobs(void){
    vector<Blob> active_blobs;
    for(auto b:blobs_){
        if(b.is_inited_ && (b.is_dead_==false))
            active_blobs.push_back(b);
    }
    return active_blobs;
}

int BlobManager::setDeadBlobs(double ts){
    for(int i=0; i<blobs_.size(); ++i)
        blobs_[i].checkDead(ts);
}

