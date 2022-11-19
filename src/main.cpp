

#include <iostream>
#include <istream>
#include <vector>
#include <opencv2/opencv.hpp>

#include "event.hpp"
#include "blob.hpp"



using namespace std;



// load events from a file.
vector<Event> loadEvents(string filename){
    ifstream fin(filename, ios::in);
    if (!fin){
        cout << "[Error] Cannot open file: " << filename << endl;
        std::abort();
    }
    vector<Event> events;
    string line_str;
    int counter = 0;
    while(getline(fin, line_str)){
        // cout << line_str << endl;
        stringstream ss(line_str);
        string str;
        vector<string> evt;
        while(getline(ss, str, ',')){
            evt.push_back(str);
        }
        events.push_back(Event(stoi(evt[0])*1e-3f, stoi(evt[1]), stoi(evt[2])));
    }
    // for (auto e=events.begin(); e!=events.end(); ++e){
    //     e->print();
    // }
    return events;
}




int main(){
    cout << "Blob tracking." << endl;

    // STEP 1. Load events
    // string path = "/home/larrydong/codeGit/blob_tracking/data/1.csv";
    string path = "/home/larrydong/codeGit/blob_tracking/src/test.csv";
    vector<Event> events = loadEvents(path);
    cout << "--> loaded " << events.size() << " events." << endl;

    // STEP 2. Check blobs.
    BlobManager bm;
    for(auto e:events){
        int blob_id = bm.checkBlob(e);
        if(blob_id == -1){
            cout << "<-- Event not belong to any blobs, create a new blob." << endl;
            int new_blob_id = bm.createBlob(e);
            // cout << "New blob info: " << endl;
            // bm.blobs_[new_blob_id].printInfo();
        }
        else{
            bm.blobs_[blob_id].addEvent(e);
        }
    }

    // update all blobs
    bm.updateAllBlobs();
    bm.printBlobInfo();
    

    return 0;
}

