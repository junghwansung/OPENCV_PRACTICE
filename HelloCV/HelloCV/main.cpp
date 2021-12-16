#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

struct MOUSE_DATA {
	cv::Mat _img;
	cv::Point _pt;
};

void on_mouse(int _event, int _x, int _y, int _flag, void* _data);

void on_brightness(int _pos, void* _data);

int StitchImages(const InputArray& _imgs);

int main()
{
	cout << "Hello, OpenCV" << CV_VERSION << endl;

	/*cv::Mat img;
	img = cv::imread("lenna.bmp");

	if (img.empty())
	{
		cerr << "Image load failed!" << endl;
		return -1;
	}

	cv::namedWindow("image");
	cv::imshow("image", img);*/

	/*cv::Mat img1 = imread("dog.bmp");

	if (img1.empty())
	{
		{
			cerr << "Image load failed!" << endl;
			return -1;
		}
	}

	cv::Mat img2 = img1;
	cv::Mat img3;
	img3 = img1;

	cv::Mat img4 = img1.clone();
	cv::Mat img5;
	img1.copyTo(img5);

	img1.setTo(Scalar(0, 255, 255));

	cv::imshow("img1", img1);
	cv::imshow("img2", img2);
	cv::imshow("img3", img3);
	cv::imshow("img4", img4);
	cv::imshow("img5", img5);*/

	/*
	cv::Mat cat_img = imread("cat.bmp");
	cv::Mat ext_img = cat_img(Rect(220, 120, 340, 240));
	//cv::Mat row_img = cat_img.rowRange({10, 300});

	ext_img = ~ext_img;
	//cout << cat_img.at<uchar>(1, 1) << endl;
	cout << "height : " << cat_img.rows << ", width : " << cat_img.cols << endl;
	cout << "channels : " << cat_img.channels() << endl;

	// Draw
	line(cat_img, Point(50, 50), Point(200, 50), Scalar(0, 0, 255));
	line(cat_img, Point(50, 100), Point(200, 100), Scalar(255, 0, 255), 3);
	line(cat_img, Point(50, 150), Point(200, 150), Scalar(255, 0, 0), 10);

	MOUSE_DATA user_data;
	user_data._img = cat_img;

	// Mouse Event
	namedWindow("cat");
	setMouseCallback("cat", on_mouse, (void*)(&user_data));

	cv::imshow("cat", cat_img);
	//cv::imshow("cat_ext", ext_img);
	//cv::imshow("row img", row_img);

	// Keyboard 
	while (true)
	{
		int key_type = cv::waitKey();
		if (key_type == 27)
			break;
	}
	*/
	

	/*Mat src = cv::imread("lenna.bmp", IMREAD_GRAYSCALE);
	if (src.empty())
	{
		cerr << "failed to read image" << endl;
		return -1;
	}

	namedWindow("brightness");
	createTrackbar("Brightness", "brightness", 0, 100, on_brightness, (void*)(&src));
	on_brightness(0, (void*)(&src));

	cv::waitKey();
	cv::destroyAllWindows();*/

	vector<cv::String> file_dirs = {"img1.jpg", "img2.jpg", "img3.jpg"};
	vector<cv::Mat> imgs;
	for_each(file_dirs.begin(), file_dirs.end(), [&imgs](const cv::String& _dir) {
		cv::Mat img = imread(_dir);
		if (!img.empty()) 
			imgs.push_back(img);
		});

	if (imgs.size() < 2)
	{
		cerr << "not enough to stitch images" << endl;
		return -1;
	}

	StitchImages(imgs);

	return 0;
}

void on_mouse(int _event, int _x, int _y, int _flag, void* _data)
{
	MOUSE_DATA* ori_data = (MOUSE_DATA*)(_data);

	switch (_event)
	{
	case cv::EVENT_LBUTTONDOWN:
	{
		ori_data->_pt = Point(_x, _y);
		cout << "EVENT_LBUTTONDOWN" << ori_data->_pt << endl;
	}	break;

	case cv::EVENT_LBUTTONUP:
	{
		cout << "EVENT_LBUTTONUP" << Point(_x, _y) << endl;
	}	break;

	case cv::EVENT_MOUSEMOVE:
	{
		if (_flag & cv::EVENT_FLAG_LBUTTON)
		{
			line(ori_data->_img, ori_data->_pt, Point(_x, _y), Scalar(0, 255, 255), 2);
			cv::imshow("cat", ori_data->_img);
			ori_data->_pt = Point(_x, _y);
		}
	}	break;

	default:
		break;
	}
}

void on_brightness(int _pos, void* _data)
{
	Mat* src_img = (Mat*)_data;

	Mat dst_img = *src_img + _pos;

	imshow("brightness", dst_img);
}

int StitchImages(const InputArray& _imgs)
{
	Ptr<Stitcher> stitcher = Stitcher::create();

	Mat dst;
	Stitcher::Status status = stitcher->stitch(_imgs, dst);

	if (status != Stitcher::Status::OK) {
		cerr << "Error on stitching!" << endl;
		return -1;
	}

	imwrite("result.jpg", dst);

	imshow("dst", dst);

	waitKey();
	return 0;
}