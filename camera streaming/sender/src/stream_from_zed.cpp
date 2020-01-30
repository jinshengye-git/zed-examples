///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2020, STEREOLABS.
//
// All rights reserved.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////

/************************************************************************************************
 ** This sample shows how to stream remotely the video of a ZED camera. Any application using   **
 ** the ZED SDK can receive and process this stream. See Camera Streaming/Receiver example.     **
 *************************************************************************************************/

// Standard includes
//#include <stdio.h>
//#include <string.h>

// ZED includes
#include <sl/Camera.hpp>

// OpenCV

#include <opencv2/opencv.hpp>

// Using namespace
//using namespace sl;
//using namespace std;

/**
* Conversion function between sl::Mat and cv::Mat, sl means stereolab I guess.
**/
cv::Mat slMat2cvMat(sl::Mat& input) {
    // Mapping between MAT_TYPE and CV_TYPE
    int cv_type = -1;
    switch (input.getDataType()) {
        case sl::MAT_TYPE::F32_C1: cv_type = cv::CV_32FC1; break;
        case sl::MAT_TYPE::F32_C2: cv_type = cv::CV_32FC2; break;
        case sl::MAT_TYPE::F32_C3: cv_type = cv::CV_32FC3; break;
        case sl::MAT_TYPE::F32_C4: cv_type = cv::CV_32FC4; break;
        case sl::MAT_TYPE::U8_C1: cv_type = cv::CV_8UC1; break;
        case sl::MAT_TYPE::U8_C2: cv_type = cv::CV_8UC2; break;
        case sl::MAT_TYPE::U8_C3: cv_type = cv::CV_8UC3; break;
        case sl::MAT_TYPE::U8_C4: cv_type = cv::CV_8UC4; break;
        default: break;
    }

    // Since cv::Mat data requires a uchar* pointer, we get the uchar1 pointer from sl::Mat (getPtr<T>())
    // cv::Mat and sl::Mat will share a single memory structure
    return cv::Mat(input.getHeight(), input.getWidth(), cv_type, input.getPtr<sl::uchar1>(sl::MEM::CPU));
}



cv::Mat getZedImg(sl::Camera& zedm, sl::InitParameters& initParameters) {
    // Create a ZED-Mini camera
    //sl::Camera zedm;

    // Set configuration parameters for the ZED
    // I think this part should be done by zed-ros-wrapper
    // I will try to find the way call an existing parameter settings.

    //sl::InitParameters initParameters;
    //initParameters.camera_resolution = sl::RESOLUTION::HD720;
    //initParameters.depth_mode = sl::DEPTH_MODE::NONE;

    ERROR_CODE err = zedm.open(initParameters);

    if (err != ERROR_CODE::SUCCESS) {
        sl::printf("%s\n", toString(err).c_str());
        zed.close();
        return 1; // Quit if an error occurred
    }

    // Set runtime parameters after opening the camera
    sl::RuntimeParameters runtime_parameters;
    runtime_parameters.sensing_mode = sl::SENSING_MODE::STANDARD; //sl::SENSING_MODE::FILL


    // Prepare new image size to retrieve half-resolution images
    sl::Resolution image_size = zedm.getCameraInformation().camera_resolution;
    
    int new_width = image_size.width; //  later use: image_size.width / 2;
    int new_height = image_size.height; // later use: image_size.height / 2;

    sl::Resolution new_image_size(new_width, new_height);
    sl::Mat image_zedm(new_width, new_height, sl::MAT_TYPE::U8_C4);

    cv::Mat image_ocv = slMat2cvMat(image_zedm);
    return image_ocv;
}




