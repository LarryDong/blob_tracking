

#include <iostream>
#include <istream>
#include <vector>
#include <opencv2/opencv.hpp>

#include "event.hpp"
#include "blob.hpp"



using namespace std;
using namespace cv;

#define IMAGE_WIDTH 346
#define IMAGE_HEIGHT 260

// load events from a file.
vector<Event> loadEvents(string filename, long int max_line=1e6){
    ifstream fin(filename, ios::in);
    if (!fin){
        cout << "[Error] Cannot open file: " << filename << endl;
        std::abort();
    }
    vector<Event> events;
    string line_str;
    int counter = 0;
    while (getline(fin, line_str) && counter++ < max_line){
        // cout << line_str << endl;
        stringstream ss(line_str);
        string str;
        vector<string> evt;
        while(getline(ss, str, ',')){
            evt.push_back(str);
        }
        events.push_back(Event(stoi(evt[0])*1e-3f, stoi(evt[1]), stoi(evt[2])));
    }
    return events;
}



Mat getEventFrame(const vector<Event>& evts){
    Mat img = Mat::zeros(Size(IMAGE_WIDTH, IMAGE_HEIGHT), CV_8UC1);
    for(auto e:evts){
        img.at<uchar>(e.y, e.x) = 255;
    }
    return img;
    // imshow("frame", img);
    // waitKey(1);
    // int key = waitKey(0);
    // if (key == 'q'){
    //     std::abort();
    // }
}

void drawBlobImage(Mat img, BlobManager bm){
    if(img.type()==CV_8UC1)
        cvtColor(img, img, COLOR_GRAY2BGR);
    for(auto blob:bm.getActiveBlobs()){
        circle(img, Point(blob.x_, blob.y_), 5, Scalar(0,0,255), 2);
    }
    imshow("blobs", img);
    waitKey(1);
    int key = waitKey(0);
    if(key == 'q')
        std::abort();
}


int main(){
    cout << "Blob tracking." << endl;

    // STEP 1. Load events
    // string path = "/home/larrydong/codeGit/blob_tracking/data/1.csv";
    string path = "/home/larrydong/codeGit/blob_tracking/src/data_output.csv";
    vector<Event> full_events = loadEvents(path);
    cout << "--> loaded " << full_events.size() << " events." << endl;


    BlobManager bm;
    int batch_number=1e4;
    for(int i=0; i<int(full_events.size()/batch_number); ++i){
        vector<Event> events;
        events.resize(batch_number);
        copy(full_events.begin()+i*batch_number, full_events.begin()+(i+1)*batch_number, events.begin());

        for (auto e : events){
            int blob_id = bm.checkBlob(e);
            if (blob_id == -1){
                cout << "<-- Event not belong to any blobs, create a new blob." << endl;
                int new_blob_id = bm.createBlob(e);
            }
            else
                bm.blobs_[blob_id].addEvent(e);
        }
        // update all blobs
        bm.updateAllBlobs();
        // bm.printBlobInfo();

        Mat img = getEventFrame(events);
        drawBlobImage(img, bm);
    }
    

    // // STEP 2. Check blobs.

    

    return 0;
}

