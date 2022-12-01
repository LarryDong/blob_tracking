# blob_tracking
event-based blob tracking. Tracking a blob of moving events, which can be anything, people/cars (see the picture below).
<div align=center>
<img src="https://user-images.githubusercontent.com/14933902/204938139-7dcfbbf9-599f-482d-a514-ab3339bfac82.png" width="720" alt="blob_demo"/><br/>
</div>

## Features
- Blob initialization, blob tracking, and blob kill
- Up to about 2Mb events/s processed on an Intel i7 CPU, single thread


## Usage

**dependencies**

`gflags` is needed for parameter settings. Download and compile: [here](https://github.com/gflags/gflags).  
`OpenCV` is optional only for visulization. Install it before or comment the codes for visualization.  
Tested on Ubuntu 18.04, OpenCV 3.2.

**Clone, compile and run.**  
```bash
# clone 
git clone https://github.com/LarryDong/blob_tracking.git

# modifty the `path` in main.cpp

# compile 
mkdir build && cd build
cmake .. && make

# run:
./blob_tracking <--parameters>      # check the sorce file to see parameters.
```

## Limitations & Issues
- The blob size is fixed to be a circle with constant radius, but can be easily modofied.
- Cannot merge/split two blobs when two blobs come together/apart, easily to be implemented.
- No denoise process, which may cause computation burden on noisy environment.

Feel free to pull issues/requests, or contact Larrydong: jfjxxdy2007@126.com.
