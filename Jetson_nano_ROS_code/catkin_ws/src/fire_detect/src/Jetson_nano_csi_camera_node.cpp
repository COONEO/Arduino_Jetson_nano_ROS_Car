#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sstream>
#include <iostream>
 
using namespace cv;
 
std::string gstreamer_pipeline (int sensor_id, int sensor_mode, int capture_width, int capture_height, int display_width, int display_height, int frameate, int flip_method) {
    return "nvarguscamerasrc sensor_id=" + std::to_string(sensor_id) + " sensor_mode=" + std::to_string(sensor_mode) + " ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(capture_width) + ", height=(int)" +
           std::to_string(capture_height) + ", format=(string)NV12, frameate=(fraction)" + std::to_string(frameate) +
           "/1 ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! video/x-raw, width=(int)" + std::to_string(display_width) + ", height=(int)" +
           std::to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}
 
int main(int argc, char** argv) {
    int capture_width = 1280 ;
    int capture_height = 720 ;
    int display_width = 1280 ;
    int display_height = 720 ;
    int frameate = 60 ;
    int flip_method = 0 ;	
 
    ros::init(argc, argv, "Jetson_nano_csi_camera_node");
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    image_transport::Publisher pub  = it.advertise("csi_cam/image_raw", 1);
 
    std::string pipeline_cam = gstreamer_pipeline(0, 3, capture_width,
	capture_height,
	display_width,
	display_height,
	frameate,
	flip_method);
    std::cout << "Using pipeline_cam: \n\t" << pipeline_cam << "\n";

    cv::VideoCapture cap(pipeline_cam, cv::CAP_GSTREAMER);
    if (!cap.isOpened()) {
        ROS_INFO("cannot open video device0\n");
        return 1;
    }	    
 
    sensor_msgs::ImagePtr msg_cam;
    ros::Rate loop_rate(30);      //hz
    cv::Mat frame;
    cv::Mat result_frame;
    while (nh.ok()) 
    {
    	if (!cap.read(frame)) {
		    std::cout << "Capture left read error" << std::endl;
		    break;
	    }
	    ros::Time time= ros::Time::now ();
        std::cout << "frame "<<"frame.cols: "<<frame.cols <<"; frame.rows: "<<frame.rows<<std::endl;
        // 图像翻转
        // 参数 <0 代表左右上下颠倒 ； 0 代表上下颠倒 ； >0 代表左右颠倒 ;
        cv::flip(frame , result_frame , 0);
        	
        cv::Point origin; 
	    origin.x = result_frame.cols - 200;
	    origin.y = result_frame.rows - 100;
        cv::putText(result_frame, "COONEO", origin , cv::FONT_HERSHEY_COMPLEX, 1.0 , cv::Scalar(0, 0, 0), 2);

	    if (!result_frame.empty()) {  
            msg_cam = cv_bridge::CvImage(std_msgs::Header(), "bgr8", result_frame).toImageMsg(); 
	        msg_cam->header.stamp = time;
            pub.publish(msg_cam);  
        }
	    // ROS_INFO("Publishing csi_cam/image_raw ROS topic !! ");
        ros::spinOnce();  
        loop_rate.sleep();           //与ros::Rate loop_rate相对应
 
    }
    cap.release();
}