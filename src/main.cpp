

#include <iostream>
#include <istream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <gflags/gflags.h>
#include "event.hpp"
#include "blob.hpp"



using namespace std;
using namespace cv;

#define IMAGE_WIDTH 346
#define IMAGE_HEIGHT 260


// parameters for blob's
DEFINE_int32(blob_radius, 50, "blob radius");
DEFINE_int32(blob_active_thresold, 100, "number of events for active blob");
DEFINE_int32(blob_lifetime, 1e2, "life of a blob since last update, in ms");


// parameters for drawing
DEFINE_int32(max_load_events, 1e6, "load events from file to avoid to huge file");
DEFINE_int32(batch_number, 1e4, "process number of events for view/update");



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
    for(const Event& e:evts){
        if(e.y >= IMAGE_HEIGHT || e.x >= IMAGE_WIDTH){
            cout << "Error. Pixel out-of image size. Please check." << endl;
            std::abort();
        }
        img.at<uchar>(e.y, e.x) = 255;
    }
        
    return img;
}

void drawBlobImage(Mat img, BlobManager bm){
    cv::RNG rng(5);
    if (img.type() == CV_8UC1)
        cvtColor(img, img, COLOR_GRAY2BGR);
    for(const Blob& blob : bm.getActiveBlobs()){
        auto color = g_color_map[blob.id_%100];
        double r = FLAGS_blob_radius;
        Point center(blob.x_, blob.y_);
        circle(img, center, 2, color, -1);          // center point
        circle(img, center, r, color, 1);           // draw circle
        putText(img, "id:"+to_string(blob.id_), center+Point(r, -r), cv::FONT_ITALIC, 0.4, color);
    }
    imshow("blobs", img);
    waitKey(1);
    int key = waitKey(0);
    if(key == 'q')
        std::abort();
}


int main(int argc, char** argv){
	google::ParseCommandLineFlags(&argc, &argv, true);
    cout << "Blob tracking." << endl;
    
    // Load events
    // string path = "/home/larrydong/codeGit/blob_tracking/data/1.csv";
    string path = "/home/larrydong/codeGit/blob_tracking/src/data_output.csv";
    vector<Event> full_events = loadEvents(path);
    cout << "--> loaded " << full_events.size() << " events." << endl;


    BlobManager bm(FLAGS_blob_radius);
    int batch_number=1000;
    generateRandomColorMap();
    int idx = 0;
    double ts = 0;
    for(int i=0; i<int(full_events.size()/batch_number); ++i){
        cout << " -->  Processing batch: " << i << ", ts: " << ts << endl;
        vector<Event> events;
        // TODO: optimize this copy process. Not necessary.
        events.resize(batch_number);
        copy(full_events.begin()+i*batch_number, full_events.begin()+(i+1)*batch_number, events.begin());
        ///////////////////////////////////////////////////////////////
        for (auto e : events){
            int blob_id = bm.findNearestBlob(e);
            if (blob_id == -1)
                bm.createBlob(e);
            else
                bm.blobs_[blob_id].addEvent(e);
        }
        // update all blobs
        ts = events[events.size()-1].ts;
        bm.updateAllBlobs(ts);
        bm.removeDeadBlobs(ts);
        // bm.printBlobInfo(true);

        Mat img = getEventFrame(events);
        drawBlobImage(img, bm);
    }
    return 0;
}

