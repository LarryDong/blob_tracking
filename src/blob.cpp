
#include "blob.hpp"
#include <gflags/gflags.h>

DECLARE_int32(blob_active_thresold);
DECLARE_int32(blob_radius);

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
    events_ = deque<Event>();                   // clear the queue
    events_.push_back(e);                       // add the first events
}

void Blob::updateBlob(double ts){              // update blob's information when called.
    double sum_x = 0, sum_y = 0;
    // // delete both position and time.
    // for(auto it=events_.begin(); it!=events_.end();){
    //     // remove `old` and `far events to make the center update.
    //     if((it->ts < ts - dt_)                                 // TODO: should use the newest ts.
    //                 || (abs(it->x - x_) > FLAGS_blob_radius/2)
    //                 || (abs(it->y - y_) > FLAGS_blob_radius/2)){
    //         it = events_.erase(it);
    //     }
    //     else{
    //         sum_x += it->x;
    //         sum_y += it->y;
    //         it++;
    //     }
    // }

    // use a faster version. (no delete position)
    while(events_.size()!=0){
        if (events_.front().ts < ts - dt_)
            events_.pop_front();
        else
            break;
    }
    for(const Event& e:events_){
        sum_x += e.x;
        sum_y += e.y;
    }
    active_events_number_ = events_.size();
    x_ = sum_x/active_events_number_;
    y_ = sum_y/active_events_number_;

    if(!is_inited_){                            // init a blob.
        if (active_events_number_ >= FLAGS_blob_active_thresold){
            is_inited_ = true;
            id_ = next_id_++;                   // set ID.
            // cout << "Init blob: " << id_ << " at: " << x_ << ", " << y_ << endl;
        }
    }
}

int Blob::addEvent(const Event& evt){
    events_.push_back(evt);
    current_ts_ = evt.ts;
}

bool Blob::checkAndSetDead(double ts){
    if(events_.size()==0){
        // cout << "Error. Unexpected HERE!!!" << endl;
        is_dead_ = true;
        return true;
    }
    while(events_.size()!=0){                                           // remove old events
        // if(id_ == 4){
        //     cout << "Debug for id4" << endl;
        //     cout << events_.front().ts << ", " << current_ts_ - dt_ << endl;
        // }
        if (events_.front().ts < ts - dt_)
            events_.pop_front();
        else
            break;
    }
    if(events_.size() >= FLAGS_blob_active_thresold)
        is_dead_ = false;
    else
        is_dead_ = true;
    return is_dead_;
}




////////////////  BlobManager  ////////////////
BlobManager::BlobManager(int radius) : radius_(radius), blobs_(deque<Blob>()) { ; }

// find nearest blob's id
int BlobManager::findNearestBlob(const Event &e){
    int nearest_index = -1;                             // this is the nearest blob's index, not the blob's ID.
    int min_distance = 1e8;
    // first check for inited blobs. Find nearest.
    for (int i=0; i<blobs_.size(); ++i){
        const Blob& b = blobs_[i];
        if(b.is_dead_ || !b.is_inited_)
            continue;
        int dist = (b.x_-e.x)*(b.x_-e.x) + (b.y_-e.y)*(b.y_-e.y);
        // int dist = abs(b.x_-e.x) + abs(b.y_-e.y);
        if (dist < min_distance){
            nearest_index = i;
            min_distance = dist;
        }
    }
    if(min_distance < radius_*radius_)
        return nearest_index;

    // if not belong to any active blob, then find a near (un-inited) blob.
    for (int i=0; i<blobs_.size(); ++i){
        const Blob& b = blobs_[i];
        if(b.is_dead_ || b.is_inited_)
            continue;
        int dist = (b.x_-e.x)*(b.x_-e.x) + (b.y_-e.y)*(b.y_-e.y);
        // int dist = abs(b.x_-e.x) + abs(b.y_-e.y);
        if (dist < 20*20)           // use a smaller size for init.
            return i;
    }
    return -1;
}


int BlobManager::createBlob(const Event& e){
    blobs_.push_back(Blob(e));
    return blobs_.size()-1;                     // return new_blob's id.
}


int BlobManager::updateAllBlobs(double ts){
    for(int i=0; i<blobs_.size(); ++i){         // Attention! Can't use for(auto b:blobs), this is only a reference.
        blobs_[i].updateBlob(ts);
    }
}


deque<Blob> BlobManager::getActiveBlobs(void){
    deque<Blob> active_blobs;
    for(const Blob& b:blobs_){
        if(b.is_inited_)
            active_blobs.push_back(b);
        if(b.is_dead_)
            cout << "Warning. `Dead` blobs should be removed already." << endl;
    }
    return active_blobs;
}


int BlobManager::removeDeadBlobs(double ts){
    int num = 0;
    // cout << "blob size: " << blobs_.size() << endl;
    for(auto it=blobs_.begin(); it!=blobs_.end();){
        if(it->checkAndSetDead(ts)){
            it = blobs_.erase(it);              // TODO: segmentation fault here!!
            num++;
        }
        else{
            it++;
            // cout << "iter++" << endl;
        }
    }
    return num;
}
