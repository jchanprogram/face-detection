#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>

/* Test for using the .xml generate with Haar cascade in Opencv file */


//////////////////////////////////////////////////////////////////////////////////////
void detectFaceOpenCVHaar(cv::CascadeClassifier faceCascade, cv::CascadeClassifier eyeCascade, cv::Mat &frameOpenCVHaar, int inHeight = 300, int inWidth = 0)

{
	int frameHeight = frameOpenCVHaar.rows;
	int frameWidth = frameOpenCVHaar.cols;
	if (!inWidth)
		inWidth = (int)((frameWidth / (float)frameHeight) * inHeight);

	float scaleHeight = frameHeight / (float)inHeight;
	float scaleWidth = frameWidth / (float)inWidth;

	cv::Mat frameOpenCVHaarSmall, frameGray;
	resize(frameOpenCVHaar, frameOpenCVHaarSmall, cv::Size(inWidth, inHeight));
	cvtColor(frameOpenCVHaarSmall, frameGray, cv::COLOR_BGR2GRAY);

	std::vector<cv::Rect> faces;
	faceCascade.detectMultiScale(frameGray, faces);

	for (size_t i = 0; i < faces.size(); i++)
	{
		int x1_1 = (int)(faces[i].x * scaleWidth);
		int y1_1 = (int)(faces[i].y * scaleHeight);
		int x2_1 = (int)((faces[i].x + faces[i].width) * scaleWidth);
		int y2_1 = (int)((faces[i].y + faces[i].height) * scaleHeight);
		rectangle(frameOpenCVHaar, cv::Point(x1_1, y1_1), cv::Point(x2_1, y2_1), cv::Scalar(0, 0, 255), (int)(frameHeight / 150.0), 4);
	}
	

	std::vector<cv::Rect> eye;
	eyeCascade.detectMultiScale(frameGray, eye);

	for (size_t i = 0; i < eye.size(); i++)
	{
		int x1 = (int)(eye[i].x * scaleWidth);
		int y1 = (int)(eye[i].y * scaleHeight);
		int x2 = (int)((eye[i].x + eye[i].width) * scaleWidth);
		int y2 = (int)((eye[i].y + eye[i].height) * scaleHeight);
		rectangle(frameOpenCVHaar, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(255, 0, 0), (int)(frameHeight / 150.0), 4);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)

{
	cv::CascadeClassifier faceCascade;
	cv::CascadeClassifier eyeCascade;
	std::string cascadepath = "../opencv/build/etc/haarcascades/";
	std::string cascadenameeye = "haarcascade_eye_tree_eyeglasses.xml";
	std::string cascadenamefrontface = "haarcascade_frontalface_default.xml";

	if (!faceCascade.load(cascadepath + cascadenamefrontface)) // Check for failure
	{
		std::cout << "Error loading face cascade" << std::endl;
		system("pause"); //wait for any key press
		return -1;
	}

	if (!eyeCascade.load(cascadepath + cascadenameeye)) // Check for failure
	{
		std::cout << "Error loading eye cascade" << std::endl;
		system("pause"); //wait for any key press
		return -1;
	}
	// Read the image file

	cv::Mat image = cv::imread("../lena.jpg");
	if (image.empty()) // Check for failure
	{
		std::cout << "Could not open or find the image" << std::endl;
		system("pause"); //wait for any key press
		return -1;
	}

	
	cv::String windowNameImage = "Original image"; //Name of the window
	namedWindow(windowNameImage); // Create a window
	imshow(windowNameImage, image); // Show our image inside the created window.

	cv::waitKey(0); // Wait for any keystroke in the window
	
	detectFaceOpenCVHaar(faceCascade, eyeCascade, image);

	imshow(windowNameImage, image); // Show our image inside the created window.
	cv::waitKey(0); // Wait for any keystroke in the window
	cv::destroyAllWindows(); //destroy the created window

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Detection with the webcam*/
	
	cv::VideoCapture cap(0);
		
	// if not success, exit program
	if (cap.isOpened() == false)
	{
		std::cout << "Cannot open the video camera" << std::endl;
		std::cin.get(); //wait for any key press
		return -1;
	}

	double dWidth = cap.get(cv::CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	double dHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

	std::cout << "Resolution of the video : " << dWidth << " x " << dHeight << std::endl;
	
	while (true)
	{
		cv::Mat frame;
		bool bSuccess = cap.read(frame); // read a new frame from video 

		//Breaking the while loop if the frames cannot be captured
		if (bSuccess == false)
		{
			std::cout << "Video camera is disconnected" << std::endl;
			std::cin.get(); //Wait for any key press
			break;
		}

		std::string windowNameVideo = "My Video";
		cv::namedWindow(windowNameVideo); //create a window called "My Camera Feed"
		imshow(windowNameVideo, frame); // Show our image inside the created window.

		double tt_opencvHaar = 0;
		double fpsOpencvHaar = 0;
		double t = cv::getTickCount();
		detectFaceOpenCVHaar(faceCascade, eyeCascade, frame);
		
		tt_opencvHaar = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
		fpsOpencvHaar = 1 / tt_opencvHaar;
		putText(frame, cv::format("OpenCV HAAR ; FPS = %.2f", fpsOpencvHaar), cv::Point(10, 50), cv::FONT_HERSHEY_SIMPLEX, 1.4, cv::Scalar(0, 0, 255), 4);
		imshow("OpenCV - HAAR Face Detection", frame);

		if (cv::waitKey(10) == 27)
		{
			std::cout << "Esc key is pressed by user. Stoppig the video" << std::endl;
			break;
		}

	}

		return 0;
}
