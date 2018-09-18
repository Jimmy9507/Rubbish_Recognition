
// rubbish_detectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "rubbish_detect.h"
#include "rubbish_detectDlg.h"
#include "afxdialogex.h"
#include <opencv2/opencv.hpp>
#include "CvvImage.h"
#include "Img_Processor.h"

#include<iostream>
#include<cstring>

using namespace std; 
using namespace cv;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CvCapture* Capture;
CvCapture* m_Video;
IplImage* frame;
IplImage *frame_detected;
IplImage* chos_frame;
IplImage *chos_frame_pred;
IplImage* m_grabframe;
Mat src, src_gray, src_filt, src_edges, src_cont, src_ori_cont;
CRect rect;
CRect rect2;
CDC *pDC1;
CDC*pDC2;
CDC *pDC3;
CDC *pDC4;
CDC *pDC5;
CDC *pDC6;
CDC *pDC7;
Mat test_src;
IplImage* test_frame;
HDC hDC1, hDC2, hDC3,hDC4, hDC5;
HDC hDC6, hDC7;
CWnd *pwnd;
int ImgNum = 0;
//Img_Processor imgProc;
int edgeThresh = 1;
int lowThreshold = 170;
int const max_lowThreshold = 200;
int ratio = 3;
int kernel_size = 3;
RNG rng(12345);
int capture = 1;
int img_seq = 1000;
int save_i=1000;
//matlab model and net prototxt file .
const string rpn_prototxt_file = "F:/faster_rcnn/caffe-master/windows/rubbish_detect/rubbish_detect/models/proposal_test.prototxt";
const string rpn_model_file = "F:/faster_rcnn/caffe-master/windows/rubbish_detect/rubbish_detect/models/proposal_final";
const string fcnn_prototxt_file = "F:/faster_rcnn/caffe-master/windows/rubbish_detect/rubbish_detect/models/detection_test.prototxt";
const string fcnn_model_file = "F:/faster_rcnn/caffe-master/windows/rubbish_detect/rubbish_detect/models/detection_final";

//mean_image.bmp is convert from the model.mat  in  matlab model directory 
//you can save mean_image by command  imwrite(uint8(proposal_detection_model.image_means),"mean_image.bmp")
const string mean_image_file = "F:/faster_rcnn/caffe-master/windows/rubbish_detect/rubbish_detect/models/mean_image.bmp";
const string img_path = "F:/faster_rcnn/caffe-master/windows/rubbish_detect/rubbish_detect/imgs/";
Mat img_detect(Mat src);
int ImgStrong(Mat&img, Mat&result);
IplImage* CVQueryFrame(int Capture);

IplImage *the_frame;
void img_process(IplImage* chos_frame);
IplImage* cvtCOLOR(IplImage* src);
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Crubbish_detectDlg dialog



Crubbish_detectDlg::Crubbish_detectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Crubbish_detectDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Crubbish_detectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Crubbish_detectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_BN_CLICKED(IDC_BUTTON1, &Crubbish_detectDlg::OnBnClickedButton1)
ON_BN_CLICKED(IDC_BUTTON1, &Crubbish_detectDlg::On_OpenCamera)
ON_WM_TIMER()
ON_BN_CLICKED(IDC_BUTTON2, &Crubbish_detectDlg::On_CloseCamera)
ON_STN_CLICKED(IDC_PIC6_STATIC, &Crubbish_detectDlg::OnStnClickedPic6Static)
ON_STN_CLICKED(IDC_PIC5_STATIC, &Crubbish_detectDlg::OnStnClickedPic5Static)
ON_STN_CLICKED(IDC_PIC3_STATIC, &Crubbish_detectDlg::OnStnClickedPic3Static)
END_MESSAGE_MAP()


// Crubbish_detectDlg message handlers

BOOL Crubbish_detectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void Crubbish_detectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Crubbish_detectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Crubbish_detectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Crubbish_detectDlg::On_OpenCamera()
{
	
	//Capture = cvCreateCameraCapture(0); //打开摄像头


	//if (Capture == 0)
	//{
	//	MessageBox(_T("无法连接摄像头！！！"));
	//	return;
	//}
	Capture = cvCaptureFromAVI("C:\\Users\\SEELE\\Desktop\\1.mp4");

	frame = cvQueryFrame(Capture); //从摄像头或者文件中抓取并返回一帧  
	frame_detected = CVQueryFrame(capture);
	
	
	pDC1 = GetDlgItem(IDC_PIC1_STATIC)->GetDC();//GetDlgItem(IDC_PIC_STATIC)意思为获取显示控件的句柄（句柄就是指针），获取显示控件的DC  
	GetDlgItem(IDC_PIC1_STATIC)->GetClientRect(&rect);
	hDC1 = pDC1->GetSafeHdc();//获取显示控件的句柄  
	CvvImage ori_CvvImage;
	ori_CvvImage.CopyOf(frame, 1); //复制该帧图像     
	ori_CvvImage.DrawToHDC(hDC1, &rect); //显示到设备的矩形框内  
	
	/*
	src = imread("F:\\rubbish\\4a.jpg", 1);
	frame = &IplImage(src);
	pDC1 = GetDlgItem(IDC_PIC1_STATIC)->GetDC();//GetDlgItem(IDC_PIC_STATIC)意思为获取显示控件的句柄（句柄就是指针），获取显示控件的DC  
	GetDlgItem(IDC_PIC1_STATIC)->GetClientRect(&rect);
	hDC1 = pDC1->GetSafeHdc();//获取显示控件的句柄  
	CvvImage ori_CvvImage;
	ori_CvvImage.CopyOf(frame, 1); //复制该帧图像     
	ori_CvvImage.DrawToHDC(hDC1, &rect); //显示到设备的矩形框内  
	*/
	ReleaseDC(pDC1);

	SetTimer(1, 25, NULL); 
	SetTimer(3, 500, NULL);
	SetTimer(2, 3200, NULL); 
	
	// TODO: Add your control notification handler code here
}


void Crubbish_detectDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)//nIDEvent 为定时器事件ID，1，2，3  
	{
	case 1:
	{
		// TODO: Add your message handler code here and/or call default
		pDC1 = GetDlgItem(IDC_PIC1_STATIC)->GetDC();//GetDlgItem(IDC_PIC_STATIC)意思为获取显示控件的句柄（句柄就是指针），获取显示控件的DC  

		GetDlgItem(IDC_PIC1_STATIC)->GetClientRect(&rect);

		hDC1 = pDC1->GetSafeHdc();//获取显示控件的句柄  

		frame = cvQueryFrame(Capture); //从摄像头或者文件中抓取并返回一帧  
		CvvImage m_CvvImage;
		m_CvvImage.CopyOf(frame, 1); //复制该帧图像     

		m_CvvImage.DrawToHDC(hDC1, &rect); //显示到设备的矩形框内  
	}
	break;
	case 2:
	{   chos_frame_pred = CVQueryFrame(capture);
		chos_frame = cvQueryFrame(Capture);//获取一帧

		src = Mat(chos_frame, true);
		test_src = src.clone();
		src_ori_cont = src.clone();
		cvtColor(src, src_gray, CV_BGR2GRAY);  
		//IplImage * chos_frame_gray=cvtCOLOR(chos_frame);   //灰度化
		//src_gray = Mat(chos_frame_gray, true);
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		GaussianBlur(src_gray, src_filt, Size(3, 3), 1);   
		//Mat src_streng(src_filt.size(), src_filt.depth()); //高斯去噪
		//ImgStrong(src_filt, src_streng);
		Canny(src_filt, src_edges, lowThreshold, lowThreshold*ratio, kernel_size);   //边缘检测
		int mask_width = src_edges.size().width / 4;
		int mask_height = src_edges.size().height / 4;
		Rect mask_rect;
		mask_rect.x = mask_width;
		mask_rect.y = 0;
		mask_rect.width = int(mask_width * 2);
		mask_rect.height = src_edges.size().height;
		src_edges(mask_rect).setTo(0);
		findContours(src_edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));   //轮廓提取
		Mat src_cont = Mat::zeros(src_edges.size(), CV_8UC3);
		vector<vector<Point> > contours_poly(contours.size());
		vector<Rect> boundRect(contours.size());
		vector<Point2f>center(contours.size());
		int i = 0;
		vector <vector<Point>>::iterator iter = contours.begin();
		for (; iter != contours.end();)
		{
			
			approxPolyDP(Mat(*iter), contours_poly[i], 3, true);
			boundRect[i] = boundingRect(Mat(contours_poly[i]));
			printf("%d %d", boundRect[i].width, boundRect[i].height);
			if ((boundRect[i].height / boundRect[i].width) > 5)
			{
				iter=contours.erase(iter);
			}
			else
			{
				++iter;
			}
			i++;

		}

		for (int i = 0; i < contours.size(); i++)
		{

			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(src_cont, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());
			drawContours(src_ori_cont, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());
			rectangle(src_cont, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
			rectangle(src_ori_cont, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
		}

		drawContours(src_cont, contours, -1, Scalar(255), 1);
		drawContours(src_ori_cont, contours, -1, Scalar(255), 1);
		//Mat src_detected = img_detect(test_src);

		IplImage gray_Image = src_gray;
		IplImage filt_Image = src_filt;
		IplImage edges_Image = src_edges;
		IplImage cont_Image = src_cont;
		IplImage ori_cont_Image = src_ori_cont;
		//IplImage detected_Image = src_detected;

		CvvImage gray_CvvImage;
		CvvImage filt_CvvImage;
		CvvImage edges_CvvImage;
		CvvImage cont_CvvImage;
		CvvImage ori_cont_CvvImage;
		CvvImage det_CvvImage;

		gray_CvvImage.CopyOf(&gray_Image, 1);
		filt_CvvImage.CopyOf(&filt_Image, 1);
		edges_CvvImage.CopyOf(&edges_Image, 1);
		cont_CvvImage.CopyOf(&cont_Image, 1);
		ori_cont_CvvImage.CopyOf(&ori_cont_Image, 1);
		det_CvvImage.CopyOf(chos_frame_pred, 1);

		pDC2 = GetDlgItem(IDC_PIC2_STATIC)->GetDC();
		GetDlgItem(IDC_PIC2_STATIC)->GetClientRect(&rect2);
		hDC2 = pDC2->GetSafeHdc();

		pDC3 = GetDlgItem(IDC_PIC3_STATIC)->GetDC();
		GetDlgItem(IDC_PIC3_STATIC)->GetClientRect(&rect);
		hDC3 = pDC3->GetSafeHdc();
		pDC4 = GetDlgItem(IDC_PIC4_STATIC)->GetDC();
		GetDlgItem(IDC_PIC4_STATIC)->GetClientRect(&rect);
		hDC4 = pDC4->GetSafeHdc();
		pDC5 = GetDlgItem(IDC_PIC5_STATIC)->GetDC();
		GetDlgItem(IDC_PIC5_STATIC)->GetClientRect(&rect);
		hDC5 = pDC5->GetSafeHdc();
		pDC6 = GetDlgItem(IDC_PIC6_STATIC)->GetDC();
		GetDlgItem(IDC_PIC6_STATIC)->GetClientRect(&rect);
		hDC6 = pDC6->GetSafeHdc();
		pDC7 = GetDlgItem(IDC_PIC7_STATIC)->GetDC();
		GetDlgItem(IDC_PIC7_STATIC)->GetClientRect(&rect);
		hDC7 = pDC7->GetSafeHdc();
		det_CvvImage.DrawToHDC(hDC2, &rect2);
		gray_CvvImage.DrawToHDC(hDC3, &rect);
		filt_CvvImage.DrawToHDC(hDC4, &rect);
		edges_CvvImage.DrawToHDC(hDC5, &rect);
		cont_CvvImage.DrawToHDC(hDC6, &rect);
		ori_cont_CvvImage.DrawToHDC(hDC7, &rect);
	}
	break;
	case 3:
	{

		if (save_i == 1000){

			src = Mat(frame, true);
			test_src = src.clone();
			src_ori_cont = src.clone();//获取一帧
			cvtColor(src, src_gray, CV_BGR2GRAY);     // 灰度化
			//CString str3;
			//CString str4; 
			//str3.Format(_T("%d"), src_gray.size().width);
			//str4.Format(_T("%d"), src_gray.size().height);
			//MessageBox(str3);
			//MessageBox(str4);
			//imshow("1",src_streng);
			//waitKey(0);
			//IplImage * frame_gray = cvtCOLOR(frame);   //灰度化
			//frame = &IplImage(src); 
			//IplImage * frame_gray = cvtCOLOR(frame);
			//src_gray = Mat(frame_gray, true);
			vector<vector<Point> > contours;
			vector<Vec4i> hierarchy;
			GaussianBlur(src_gray, src_filt, Size(3, 3), 1);
			//Mat src_streng(src_filt.size(), src_filt.depth());//高斯去噪
			//ImgStrong(src_filt, src_streng);
			Canny(src_filt, src_edges, lowThreshold, lowThreshold*ratio, kernel_size);  //边缘检测
			int mask_width = src_edges.size().width / 4;
			int mask_height = src_edges.size().height / 4;
			Rect mask_rect;
			mask_rect.x = mask_width;
			mask_rect.y = 0;
			mask_rect.width = mask_width * 2;
			mask_rect.height = src_edges.size().height;
			src_edges(mask_rect).setTo(0);
			findContours(src_edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));   //轮廓提取

			Mat src_cont = Mat::zeros(src_edges.size(), CV_8UC3);
			vector<vector<Point> > contours_poly(contours.size());
			vector<Rect> boundRect(contours.size());
			vector<Point2f>center(contours.size());

			for (int i = 0; i < contours.size(); i++)
			{
				approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
				boundRect[i] = boundingRect(Mat(contours_poly[i]));
			}

			for (int i = 0; i < contours.size(); i++)
			{
				Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
				drawContours(src_cont, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());
				drawContours(src_ori_cont, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());
				rectangle(src_cont, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
				rectangle(src_ori_cont, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
			}

			drawContours(src_cont, contours, -1, Scalar(255), 1);
			drawContours(src_ori_cont, contours, -1, Scalar(255), 1);
			//Mat src_detected = img_detect(test_src);

			IplImage gray_Image = src_gray;
			IplImage filt_Image = src_filt;
			IplImage edges_Image = src_edges;
			IplImage cont_Image = src_cont;
			IplImage ori_cont_Image = src_ori_cont;
			//IplImage detected_Image = src_detected;

			CvvImage gray_CvvImage;
			CvvImage filt_CvvImage;
			CvvImage edges_CvvImage;
			CvvImage cont_CvvImage;
			CvvImage ori_cont_CvvImage;
			CvvImage det_CvvImage;

			gray_CvvImage.CopyOf(&gray_Image, 1);
			filt_CvvImage.CopyOf(&filt_Image, 1);
			edges_CvvImage.CopyOf(&edges_Image, 1);
			cont_CvvImage.CopyOf(&cont_Image, 1);
			ori_cont_CvvImage.CopyOf(&ori_cont_Image, 1);
			det_CvvImage.CopyOf(frame_detected, 1);

			pDC2 = GetDlgItem(IDC_PIC2_STATIC)->GetDC();
			GetDlgItem(IDC_PIC2_STATIC)->GetClientRect(&rect2);
			hDC2 = pDC2->GetSafeHdc();

			pDC3 = GetDlgItem(IDC_PIC3_STATIC)->GetDC();
			GetDlgItem(IDC_PIC3_STATIC)->GetClientRect(&rect);
			hDC3 = pDC3->GetSafeHdc();
			pDC4 = GetDlgItem(IDC_PIC4_STATIC)->GetDC();
			GetDlgItem(IDC_PIC4_STATIC)->GetClientRect(&rect);
			hDC4 = pDC4->GetSafeHdc();
			pDC5 = GetDlgItem(IDC_PIC5_STATIC)->GetDC();
			GetDlgItem(IDC_PIC5_STATIC)->GetClientRect(&rect);
			hDC5 = pDC5->GetSafeHdc();
			pDC6 = GetDlgItem(IDC_PIC6_STATIC)->GetDC();
			GetDlgItem(IDC_PIC6_STATIC)->GetClientRect(&rect);
			hDC6 = pDC6->GetSafeHdc();
			pDC7 = GetDlgItem(IDC_PIC7_STATIC)->GetDC();
			GetDlgItem(IDC_PIC7_STATIC)->GetClientRect(&rect);
			hDC7 = pDC7->GetSafeHdc();
			det_CvvImage.DrawToHDC(hDC2, &rect2);
			gray_CvvImage.DrawToHDC(hDC3, &rect);
			filt_CvvImage.DrawToHDC(hDC4, &rect);
			edges_CvvImage.DrawToHDC(hDC5, &rect);
			cont_CvvImage.DrawToHDC(hDC6, &rect);
			ori_cont_CvvImage.DrawToHDC(hDC7, &rect);
			save_i++;
		}
	}
	break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void Crubbish_detectDlg::On_CloseCamera()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your command handler code here  
	if (!Capture)
	{
		MessageBox(_T("没有打开摄像头！！！"));
		return;
	}

	cvReleaseCapture(&Capture);

	//pDC1 = GetDlgItem(IDC_PIC1_STATIC)->GetDC();//GetDlgItem(IDC_PIC_STATIC)意思为获取显示控件的句柄（句柄就是指针），获取显示控件的DC  

	//GetDlgItem(IDC_PIC1_STATIC)->GetClientRect(&rect);

	//hDC1 = pDC1->GetSafeHdc();//获取显示控件的句柄  

	//frame = cvLoadImage("D:\\Documents\\Visual Studio 2013\\Projects\\beijing.jpg"); //图片读取路径可以自己设定  
	//CvvImage m_CvvImage;
	//m_CvvImage.CopyOf(frame, 1); //复制该帧图像     
	//m_CvvImage.DrawToHDC(hDC1, &rect); //显示到设备的矩形框内  
	//ReleaseDC(pDC1);
}
Mat img_detect(Mat src)
{

	Caffe_Net net_(rpn_prototxt_file, rpn_model_file, fcnn_prototxt_file, fcnn_model_file, mean_image_file);
	
		if (src.empty()){
			cout << "input image error;"  << endl;
		}
		
		//cvWaitKey(0);
		Mat dst = net_.fasterrcnn(src);
		//cvWaitKey(0);
	return dst;
}

void Crubbish_detectDlg::OnStnClickedPic6Static()
{
	// TODO: Add your control notification handler code here
}


void Crubbish_detectDlg::OnStnClickedPic5Static()
{
	// TODO: Add your control notification handler code here
}



void Crubbish_detectDlg::OnStnClickedPic3Static()
{
	// TODO: Add your control notification handler code here
}

IplImage* CVQueryFrame(int Capture)
{
	
	the_frame = cvLoadImage((img_path + to_string(img_seq) + ".jpg").c_str());
		img_seq++;
		if (img_seq < 1007)
			return the_frame;
		else
			exit(0);

}//从摄像头或者文件中抓取并返回一帧  

IplImage* cvtCOLOR(IplImage* src)
{
	IplImage* GrayImage = cvCreateImage(cvGetSize(src), 8, 1);
	CvMat* pGrayMat= cvCreateMat(src->height, src->width, CV_32FC1);
	BYTE data1;       
	BYTE data2;
	BYTE data3;
	BYTE data7;
	for (int j = 0; j<src->height; j++)
	{
		for (int i = 0; i<src->width; i++)
		{
			data1 = (BYTE)src->imageData[j*src->widthStep + i * 3];     //B分量  
			data2 = (BYTE)src->imageData[j*src->widthStep + i * 3 + 1]; //G分量  
			data3 = (BYTE)src->imageData[j*src->widthStep + i * 3 + 2]; //R分量  
			data7 = (BYTE)(0.11*data1 + 0.59*data2 + 0.30*data3);
			cvmSet(pGrayMat, j, i, data7);
		}
	}
	cvConvert(pGrayMat, GrayImage);
	return GrayImage;
}

int ImgStrong(Mat&img, Mat&result)
{
	//***************  
	//p[]各个灰度级出现的概率  
	//p1[]各个灰度级之前的概率和  
	//各个灰度级出现的次数  
	//*****************  
	assert((img.cols == result.cols) && (img.rows == result.rows));
	double p[256], p1[256], num[256];
	int nheight = img.rows;
	int nwidth = img.cols;
	int total = nheight*nwidth;
	memset(p, 0, sizeof(p));
	memset(p1, 0, sizeof(p1));
	memset(num, 0, sizeof(num));
	//各个灰度级出现的次数  
	for (int i = 0; i < nheight; i++)
	{
		uchar *data = img.ptr<uchar>(i);
		for (int j = 0; j < nwidth; j++)
		{
			num[data[j]]++;
		}
	}

	//各个灰度级出现的概率  
	for (int i = 0; i < 256; i++)
	{
		p[i] = num[i] / total;
	}
	//各个灰度级之前的概率和  
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j <= i; j++)
		{
			p1[i] += p[j];
		}
	}

	//直方图变换  
	for (int i = 0; i < nheight; i++)
	{
		uchar *data = img.ptr<uchar>(i);
		uchar *data0 = result.ptr<uchar>(i);
		for (int j = 0; j < nwidth; j++)
		{
			data0[j] = p1[data[j]] * 255 + 0.5;
		}
	}
	return 0;
}