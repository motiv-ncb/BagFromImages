// #include<iostream>
// #include<ros/ros.h>
// #include<rosbag/bag.h>
// #include<rosbag/view.h>
// #include<sensor_msgs/Image.h>
// #include<std_msgs/Time.h>
// #include<std_msgs/Header.h>
// #include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>
// #include <cv_bridge/cv_bridge.h>
// #include <sensor_msgs/image_encodings.h>

// #include "Thirdparty/DLib/FileFunctions.h"


// using namespace std;

// int main(int argc, char **argv)
// {
//     ros::init(argc, argv, "BagFromImages");

//     if(argc!=5)
//     {
//         cerr << "Usage: rosrun BagFromImages BagFromImages <path to image directory> <image extension .ext> <frequency> <path to output bag>" << endl;
//         return 0;
//     }

//     ros::start();

//     // Vector of paths to image
//     vector<string> filenames =
//             DUtils::FileFunctions::Dir(argv[1], argv[2], true);

//     cout << "Images: " << filenames.size() << endl;

//     // Frequency
//     double freq = atof(argv[3]);

//     // Output bag
//     rosbag::Bag bag_out(argv[4],rosbag::bagmode::Write);

//     ros::Time t = ros::Time::now();

//     const float T=1.0f/freq;
//     ros::Duration d(T);

//     for(size_t i=0;i<filenames.size();i++)
//     {
//         if(!ros::ok())
//             break;
//         cv::Mat greyMat;
//         cv::Mat im = cv::imread(filenames[i], cv::IMREAD_COLOR);
//         cv::cvtColor(im, greyMat, CV_BGR2GRAY);

//         cv_bridge::CvImage cvImage;
//         cvImage.image = greyMat;
//         cvImage.encoding = sensor_msgs::image_encodings::MONO8;
//         cvImage.header.stamp = t;
//         bag_out.write("/camera/image_raw",ros::Time(t),cvImage.toImageMsg());
//         t+=d;
//         cout << i << " / " << filenames.size() << endl;
//     }

//     bag_out.close();

//     ros::shutdown();

//     return 0;
// }

///// MODIFICATION-1 /////

// #include <rosbag/bag.h>
// #include <rosbag/view.h>
// #include <sensor_msgs/CompressedImage.h>
// #include <sensor_msgs/Image.h>
// #include <cv_bridge/cv_bridge.h>
// #include <opencv2/opencv.hpp>

// int main(int argc, char** argv) {
//     if (argc != 3) {
//         std::cerr << "Usage: " << argv[0] << " <input_bagfile> <output_bagfile>" << std::endl;
//         return 1;
//     }

//     std::string input_bagfile = argv[1];
//     std::string output_bagfile = argv[2];

//     rosbag::Bag inbag, outbag;
//     try {
//         // Open input and output bag files
//         inbag.open(input_bagfile, rosbag::bagmode::Read);
//         outbag.open(output_bagfile, rosbag::bagmode::Write);

//         // Topics to process
//         std::string input_image_topic = "/usb_cam_synced/image_raw/compressed";
//         std::string output_image_topic = "/camera/image_raw";
//         std::string imu_topic = "/vectornav/IMU";  // Adjust this according to your bag file

//         // CV Bridge for image conversion
//         cv_bridge::CvImage cv_img_bridge;

//         // Iterate through messages in the input bag
//         for (rosbag::MessageInstance const& msg : rosbag::View(inbag)) {
//             // Handle CompressedImage messages
//             if (msg.getTopic() == input_image_topic) {
//                 sensor_msgs::CompressedImage::ConstPtr compressed_image_msg = msg.instantiate<sensor_msgs::CompressedImage>();
//                 if (compressed_image_msg != nullptr) {
//                     try {
//                         // Decompress image
//                         cv::Mat cv_image = cv::imdecode(cv::Mat(compressed_image_msg->data), cv::IMREAD_COLOR);

//                         // Convert to grayscale
//                         cv::Mat gray_image;
//                         cv::cvtColor(cv_image, gray_image, cv::COLOR_BGR2GRAY);

//                         // Convert grayscale image to sensor_msgs::Image
//                         cv_img_bridge.encoding = sensor_msgs::image_encodings::MONO8;
//                         cv_img_bridge.image = gray_image;
//                         sensor_msgs::ImagePtr gray_image_msg = cv_img_bridge.toImageMsg();

//                         // Copy header from compressed image message
//                         gray_image_msg->header = compressed_image_msg->header;

//                         // Write grayscale image to output bag
//                         outbag.write(output_image_topic, gray_image_msg->header.stamp, gray_image_msg);
//                     } catch (cv_bridge::Exception& e) {
//                         ROS_ERROR("CV Bridge Exception: %s", e.what());
//                     }
//                 }
//             }
//             // Handle IMU messages
//             else if (msg.getTopic() == imu_topic) {
//                 // Write IMU messages unchanged to output bag
//                 outbag.write(imu_topic, msg.getTime(), msg);
//             }
//             // Write other messages unchanged to output bag
//             else {
//                 outbag.write(msg.getTopic(), msg.getTime(), msg);
//             }
//         }

//         // Close bags
//         inbag.close();
//         outbag.close();

//     } catch (rosbag::BagIOException const& e) {
//         ROS_ERROR("Error reading or writing bag: %s", e.what());
//         return 1;
//     }

//     return 0;
// }


///// MODIFICATION-2 /////


// #include <rosbag/bag.h>
// #include <rosbag/view.h>
// #include <sensor_msgs/CompressedImage.h>
// #include <sensor_msgs/Image.h>
// #include <cv_bridge/cv_bridge.h>
// #include <opencv2/opencv.hpp>

// int main(int argc, char** argv) {
//     if (argc != 4) {
//         std::cerr << "Usage: " << argv[0] << " <input_bagfile> <output_camera_bagfile> <output_imu_bagfile>" << std::endl;
//         return 1;
//     }

//     std::string input_bagfile = argv[1];
//     std::string output_camera_bagfile = argv[2];
//     std::string output_imu_bagfile = argv[3];

//     rosbag::Bag inbag;
//     try {
//         // Open input bag file
//         inbag.open(input_bagfile, rosbag::bagmode::Read);

//         // Topics to process
//         std::string input_image_topic = "/usb_cam_synced/image_raw/compressed";
//         std::string output_image_topic = "/camera/image_raw";
//         std::string imu_topic = "/vectornav/IMU";  // Adjust this according to your bag file

//         // CV Bridge for image conversion
//         cv_bridge::CvImage cv_img_bridge;

//         rosbag::Bag camera_outbag, imu_outbag;
//         try {
//             camera_outbag.open(output_camera_bagfile, rosbag::bagmode::Write);
//             imu_outbag.open(output_imu_bagfile, rosbag::bagmode::Write);

//             // Iterate through messages in the input bag
//             for (rosbag::MessageInstance const& msg : rosbag::View(inbag)) {
//                 // Handle CompressedImage messages
//                 if (msg.getTopic() == input_image_topic) {
//                     sensor_msgs::CompressedImage::ConstPtr compressed_image_msg = msg.instantiate<sensor_msgs::CompressedImage>();
//                     if (compressed_image_msg != nullptr) {
//                         try {
//                             // Decompress image
//                             cv::Mat cv_image = cv::imdecode(cv::Mat(compressed_image_msg->data), cv::IMREAD_COLOR);

//                             // Convert to grayscale
//                             cv::Mat gray_image;
//                             cv::cvtColor(cv_image, gray_image, cv::COLOR_BGR2GRAY);

//                             // Convert grayscale image to sensor_msgs::Image
//                             cv_img_bridge.encoding = sensor_msgs::image_encodings::MONO8;
//                             cv_img_bridge.image = gray_image;
//                             sensor_msgs::ImagePtr gray_image_msg = cv_img_bridge.toImageMsg();

//                             // Copy header from compressed image message
//                             gray_image_msg->header = compressed_image_msg->header;

//                             // Write grayscale image to output bag
//                             camera_outbag.write(output_image_topic, gray_image_msg->header.stamp, gray_image_msg);
//                         } catch (cv_bridge::Exception& e) {
//                             ROS_ERROR("CV Bridge Exception: %s", e.what());
//                         }
//                     }
//                 }
//                 // Handle IMU messages
//                 else if (msg.getTopic() == imu_topic) {
//                     // Write IMU messages unchanged to output bag
//                     imu_outbag.write(imu_topic, msg.getTime(), msg);
//                 }
//                 // Write other messages unchanged to output bags
//                 else {
//                     camera_outbag.write(msg.getTopic(), msg.getTime(), msg);
//                     imu_outbag.write(msg.getTopic(), msg.getTime(), msg);
//                 }
//             }

//             // Close output bags
//             camera_outbag.close();
//             imu_outbag.close();

//         } catch (rosbag::BagIOException const& e) {
//             ROS_ERROR("Error opening or writing output bags: %s", e.what());
//             return 1;
//         }

//         // Close input bag
//         inbag.close();

//     } catch (rosbag::BagIOException const& e) {
//         ROS_ERROR("Error reading input bag: %s", e.what());
//         return 1;
//     }

//     return 0;
// }

///// MODIFICATION-3 /////

// #include <rosbag/bag.h>
// #include <rosbag/view.h>
// #include <sensor_msgs/CompressedImage.h>
// #include <sensor_msgs/Image.h>
// #include <cv_bridge/cv_bridge.h>
// #include <opencv2/opencv.hpp>

// int main(int argc, char** argv) {
//     if (argc != 4) {
//         std::cerr << "Usage: " << argv[0] << " <input_bagfile> <output_camera_bagfile> <output_imu_bagfile>" << std::endl;
//         return 1;
//     }

//     std::string input_bagfile = argv[1];
//     std::string output_camera_bagfile = argv[2];
//     std::string output_imu_bagfile = argv[3];

//     rosbag::Bag inbag;
//     try {
//         // Open input bag file
//         inbag.open(input_bagfile, rosbag::bagmode::Read);

//         // Topics to process
//         std::string input_image_topic = "/usb_cam_synced/image_raw/compressed";
//         std::string output_image_topic = "/camera/image_raw";
//         std::string imu_topic = "/vectornav/IMU";  // Adjust this according to your bag file

//         // CV Bridge for image conversion
//         cv_bridge::CvImage cv_img_bridge;

//         rosbag::Bag camera_outbag, imu_outbag;
//         try {
//             camera_outbag.open(output_camera_bagfile, rosbag::bagmode::Write);
//             imu_outbag.open(output_imu_bagfile, rosbag::bagmode::Write);

//             // Iterate through messages in the input bag
//             for (rosbag::MessageInstance const& msg : rosbag::View(inbag)) {
//                 // Handle CompressedImage messages
//                 if (msg.getTopic() == input_image_topic) {
//                     sensor_msgs::CompressedImage::ConstPtr compressed_image_msg = msg.instantiate<sensor_msgs::CompressedImage>();
//                     if (compressed_image_msg != nullptr) {
//                         try {
//                             // Decompress image
//                             cv::Mat cv_image = cv::imdecode(cv::Mat(compressed_image_msg->data), cv::IMREAD_COLOR);

//                             // Convert to grayscale
//                             cv::Mat gray_image;
//                             cv::cvtColor(cv_image, gray_image, cv::COLOR_BGR2GRAY);

//                             // Convert grayscale image to sensor_msgs::Image
//                             cv_img_bridge.encoding = sensor_msgs::image_encodings::MONO8;
//                             cv_img_bridge.image = gray_image;
//                             sensor_msgs::ImagePtr gray_image_msg = cv_img_bridge.toImageMsg();

//                             // Copy header from compressed image message
//                             gray_image_msg->header = compressed_image_msg->header;

//                             // Write grayscale image to output bag
//                             camera_outbag.write(output_image_topic, gray_image_msg->header.stamp, gray_image_msg);
//                         } catch (cv_bridge::Exception& e) {
//                             ROS_ERROR("CV Bridge Exception: %s", e.what());
//                         }
//                     }
//                 }
//                 // Handle IMU messages
//                 else if (msg.getTopic() == imu_topic) {
//                     // Write IMU messages unchanged to output bag
//                     imu_outbag.write(imu_topic, msg.getTime(), msg);
//                 }
//                 // Skip other messages
//                 else {
//                     continue;
//                 }
//             }

//             // Close output bags
//             camera_outbag.close();
//             imu_outbag.close();

//         } catch (rosbag::BagIOException const& e) {
//             ROS_ERROR("Error opening or writing output bags: %s", e.what());
//             return 1;
//         }

//         // Close input bag
//         inbag.close();

//     } catch (rosbag::BagIOException const& e) {
//         ROS_ERROR("Error reading input bag: %s", e.what());
//         return 1;
//     }

//     return 0;
// }

///// MODIFICATION-4 /////

#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <sensor_msgs/CompressedImage.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/Imu.h>
#include "/home/vitom/catkin_ws/devel/include/vectornav/Imu.h"
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_bagfile> <output_bagfile>" << std::endl;
        return 1;
    }

    std::string input_bagfile = argv[1];
    std::string output_bagfile = argv[2];

    rosbag::Bag inbag;
    try {
        // Open input bag file
        inbag.open(input_bagfile, rosbag::bagmode::Read);

        // Output bag for Euroc format
        rosbag::Bag outbag;
        try {
            outbag.open(output_bagfile, rosbag::bagmode::Write);

            // Topics to process
            std::string input_image_topic = "/usb_cam/image_raw/compressed";
            std::string imu_topic = "/vectornav/IMU";  // Adjust this according to your bag file

            // CV Bridge for image conversion
            cv_bridge::CvImage cv_img_bridge;

            // Iterate through messages in the input bag
            for (rosbag::MessageInstance const& msg : rosbag::View(inbag)) {
                // Handle CompressedImage messages
                if (msg.getTopic() == input_image_topic) {
                    sensor_msgs::CompressedImage::ConstPtr compressed_image_msg = msg.instantiate<sensor_msgs::CompressedImage>();
                    if (compressed_image_msg != nullptr) {
                        try {
                            // Decompress image
                            cv::Mat cv_image = cv::imdecode(cv::Mat(compressed_image_msg->data), cv::IMREAD_COLOR);

                            // Convert to grayscale
                            cv::Mat gray_image;
                            cv::cvtColor(cv_image, gray_image, cv::COLOR_BGR2GRAY);

                            // Convert grayscale image to sensor_msgs::Image
                            cv_img_bridge.encoding = sensor_msgs::image_encodings::MONO8;
                            cv_img_bridge.image = gray_image;
                            sensor_msgs::ImagePtr gray_image_msg = cv_img_bridge.toImageMsg();

                            // Copy header from compressed image message
                            gray_image_msg->header = compressed_image_msg->header;

                            // Write grayscale image to output bag
                            outbag.write("/cam0/image_raw", gray_image_msg->header.stamp, gray_image_msg);
                        } catch (cv_bridge::Exception& e) {
                            ROS_ERROR("CV Bridge Exception: %s", e.what());
                        }
                    }
                }
                // Handle IMU messages
                else if (msg.getTopic() == imu_topic) {
                    vectornav::Imu::ConstPtr imu_msg = msg.instantiate<vectornav::Imu>();
                    if (imu_msg != nullptr) {
                        // Convert vectornav::Imu to sensor_msgs::Imu
                        sensor_msgs::Imu sensor_imu_msg;
                        sensor_imu_msg.header = imu_msg->header;
                        sensor_imu_msg.orientation.x = imu_msg->orientation.x;
                        sensor_imu_msg.orientation.y = -1 * imu_msg->orientation.y;
                        sensor_imu_msg.orientation.z = -1 * imu_msg->orientation.z;
                        sensor_imu_msg.orientation.w = imu_msg->orientation.w;
                        sensor_imu_msg.angular_velocity.x = imu_msg->angular_velocity.x;
                        sensor_imu_msg.angular_velocity.y = -1 *imu_msg->angular_velocity.y;
                        sensor_imu_msg.angular_velocity.z = -1 *imu_msg->angular_velocity.z;
                        sensor_imu_msg.linear_acceleration.x = imu_msg->acceleration.x;
                        sensor_imu_msg.linear_acceleration.y = -1 * imu_msg->acceleration.y;
                        sensor_imu_msg.linear_acceleration.z = -1 * imu_msg->acceleration.z;

                        // Write sensor_msgs::Imu message to output bag
                        outbag.write("/imu0", sensor_imu_msg.header.stamp, sensor_imu_msg);
                    }
                }
                // Skip other messages
                else {
                     outbag.write(msg.getTopic(), msg.getTime(), msg);
                }
            }

            // Close output bag
            outbag.close();

        } catch (rosbag::BagIOException const& e) {
            ROS_ERROR("Error opening or writing output bag: %s", e.what());
            return 1;
        }

        // Close input bag
        inbag.close();

    } catch (rosbag::BagIOException const& e) {
        ROS_ERROR("Error reading input bag: %s", e.what());
        return 1;
    }

    return 0;
}


