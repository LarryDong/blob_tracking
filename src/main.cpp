

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


vector<Scalar> g_color_map;
void generateRandomColorMap(void){
    cv::RNG rng(351351);
    for(int i=0; i<1000; ++i){
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        g_color_map.push_back(color);
    }

}

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
    for(auto e:evts)
        img.at<uchar>(e.y, e.x) = 255;
    return img;
}

void drawBlobImage(Mat img, BlobManager bm){
    cv::RNG rng(5);
    if(img.type()==CV_8UC1)
        cvtColor(img, img, COLOR_GRAY2BGR);
    for(auto blob:bm.getActiveBlobs()){
        Point center(blob.x_, blob.y_);
        circle(img, center, bm.radius_, g_color_map[blob.id_], 2);
        // putText(img, "id:", center, cv::FONT_HERSHEY_PLAIN, 8, color);
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
    int batch_number=1000;
    generateRandomColorMap();
    for(int i=0; i<int(full_events.size()/batch_number); ++i){
        vector<Event> events;
        events.resize(batch_number);
        copy(full_events.begin()+i*batch_number, full_events.begin()+(i+1)*batch_number, events.begin());
        // cout << "-----> " << events.size() << endl;
        for (auto e : events){
            int blob_id = bm.checkBlob(e);
            if (blob_id == -1){
                // cout << "<-- Event not belong to any blobs, create a new blob." << endl;
                bm.createBlob(e);
            }
            else
                bm.blobs_[blob_id].addEvent(e);
        }
        // update all blobs
        bm.updateAllBlobs();
        bm.setDeadBlobs(events[events.size()-1].ts);
        bm.printBlobInfo(true);

        Mat img = getEventFrame(events);
        drawBlobImage(img, bm);
    }
    

    // // STEP 2. Check blobs.

    

    return 0;
}

