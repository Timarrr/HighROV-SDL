#include "RovCameraWidget.hpp"

#include <QCameraInfo>
#include <QCameraImageProcessing>
#include <QVBoxLayout>
#include <QDir>
#include <QDateTime>

using namespace std;

RovCameraWidget::RovCameraWidget(QWidget* parent)
    : QStackedWidget(parent)
    //, m_cameraView(new QCameraViewfinder(this))
    , m_cameraLabel(new QLabel(this))
    //, m_cameraCapture(new QtCameraCapture(this))
    //, m_cameraList(new QComboBox)
    //, m_refreshButton(new QPushButton(tr("Обновить")))
{
    //createCameraSelectionWidget();
    //createCameraViewWidget();
    //createConnections();
    //setCurrentIndex(WidgetType::CameraSelectionWidget);

    proccessTimer = new QTimer(this);
    proccessTimer->setInterval(50);
    proccessTimer->start();

    shipTimer = new QTimer(this);
    shipTimer->setInterval(50);
    shipTimer->start();

    garageTimer = new QTimer(this);
    garageTimer->setInterval(50);
    garageTimer->start();

    connect(proccessTimer, &QTimer::timeout, this, &RovCameraWidget::proccessCamera);
    connect(shipTimer, &QTimer::timeout, this, &RovCameraWidget::countShip);
    connect(garageTimer, &QTimer::timeout, this, &RovCameraWidget::startStopGarage);

    addWidget(m_cameraLabel.data());
}

void RovCameraWidget::startCapture()
{
    cap.open(0);
    isProccessCamera = true;
}

void RovCameraWidget::proccessCamera()
{
    if (isProccessCamera)
    {
        cv::Mat originalImage;
        cap.read(originalImage);
        cv::cvtColor(originalImage, originalImage, cv::COLOR_BGR2RGB);
        QImage qOriginalImage((uchar*)originalImage.data, originalImage.cols,
            originalImage.rows, originalImage.step, QImage::Format_RGB888);
        m_cameraLabel.data()->setPixmap(QPixmap::fromImage(qOriginalImage));
        m_cameraLabel.data()->setScaledContents(true);
        m_cameraLabel.data()->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    }
}

void RovCameraWidget::stopCapture()
{
    isProccessCamera = false;
    cap.release();
}

QCamera::Status RovCameraWidget::cameraStatus()
{
    if (m_camera) {
        return m_camera->status();
    }
    return QCamera::Status::UnavailableStatus;
}

void RovCameraWidget::createCameraSelectionWidget()
{
    QWidget* cameraWidget = new QWidget;

    QWidget* topSpacer = new QWidget;
    QWidget* bottomSpacer = new QWidget;
    QWidget* leftSpacer = new QWidget;
    QWidget* rightSpacer = new QWidget;

    topSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    bottomSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    leftSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    rightSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHBoxLayout* listLayout = new QHBoxLayout;

    listLayout->addWidget(leftSpacer);
    listLayout->addWidget(m_cameraLabel.data());
    listLayout->addWidget(m_cameraList.data());
    listLayout->addWidget(m_refreshButton.data());
    listLayout->addWidget(rightSpacer);

    m_refreshButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_cameraList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    const QList<QCameraInfo> availableCameras = QCameraInfo::availableCameras();
    for (const QCameraInfo& cameraInfo : availableCameras) {
        m_cameraList->addItem(cameraInfo.description());
    }

    QVBoxLayout* cameraListLayout = new QVBoxLayout;
    cameraListLayout->addWidget(topSpacer);
    cameraListLayout->addLayout(listLayout);
    cameraListLayout->addWidget(bottomSpacer);
    cameraWidget->setLayout(cameraListLayout);
    addWidget(cameraWidget);
}

void RovCameraWidget::createCameraViewWidget()
{
    addWidget(m_cameraView.data());
}

void RovCameraWidget::createConnections()
{
    QObject::connect(m_refreshButton.data(), &QPushButton::pressed, [this]() {
        m_cameraList->clear();
        const QList<QCameraInfo> availableCameras = QCameraInfo::availableCameras();
        for (const QCameraInfo& cameraInfo : availableCameras) {
            m_cameraList->addItem(cameraInfo.description());
        }
    });
}

//mosaic
void RovCameraWidget::takeMosaicPhoto()
{
    QDir dir("../photo/mosaic");
    if (!dir.exists()) dir.mkpath(".");

    if (isProccessCamera)
    {
        if (mosaicPhotoNum >= 9) mosaicPhotoNum = 1;
        cv::Mat img;
        cap.read(img);
        cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
        QImage image(img.data, img.cols, img.rows, QImage::Format_RGB888);
        image.save("../photo/mosaic/" + QString::number(mosaicPhotoNum) + ".png");
        if (mosaicPhotoNum >= 8) qDebug() << "all photos collected";
        mosaicPhotoNum += 1;

        showFrame(img);
    }
}

void RovCameraWidget::mosaicPhotoBack()
{
    if (mosaicPhotoNum < 2) {
        mosaicPhotoNum -= 1;
        QDir dir("../photo/mosaic");
        dir.remove(QString::number(mosaicPhotoNum) + ".png");
    }
}

void RovCameraWidget::shipPhotoBack()
{
    if (shipNum < 2) {
        shipNum -= 1;
        QDir dir("../photo/mosaic");
        dir.remove(QString::number(shipNum) + ".png");
    }
}


//shipLength
vector<vector<cv::Point>> findCnt(cv::Mat img, cv::Scalar low, cv::Scalar up) {
    cv::Mat hsv, mask;
    vector<vector<cv::Point>> cnt;
    cv::cvtColor(img, hsv, cv::COLOR_RGB2HSV);
    cv::inRange(hsv, low, up, mask);
    cv::findContours(mask, cnt, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    return cnt;
}

void RovCameraWidget::countShip()
{    
//     TODO: This is shit too, rwasap

//    if (isCountShip && isProccessCamera)
//    {
//        cv::Mat img;
//        cap.read(img);
//        img = imread("work.jpg");
//        cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
//        if(img.empty())return;

//        cv::Mat mask, mask1, hsv;
//        cv::Rect rect;
//        cv::Scalar low = cv::Scalar(100, 50, 160);
//        cv::Scalar up = cv::Scalar(140, 256, 256);
//        cv::Scalar low1 = cv::Scalar(0, 50, 100);
//        cv::Scalar up1 = cv::Scalar(60, 144, 197);
//        vector<vector<cv::Point>> cnt;
//        cv::Mat cntimg;

//        vector<vector<cv::Point>> cnt1;
//        cv::Mat cntimg1;

//        cntimg = img.clone();
//        cntimg1 = img.clone();

//        double m = 0, m1 = 0;
//        int area;
//        int w, h, r;

//        cnt = findCnt(img, low, up);
//        cnt1 = findCnt(img, low1, up1);

//        if (!cnt.empty() && !cnt1.empty()) {
//            cv::drawContours(cntimg, cnt, -1, cv::Scalar(255, 0, 0), 2);
//            cv::drawContours(cntimg1, cnt1, -1, cv::Scalar(0, 0, 255), 2);
//            cv::imshow("smth2", cntimg);
//            cv::imshow("smth3", cntimg1);
//        }

//        for (auto count : cnt) {
//            area = cv::contourArea(count);
//            if (abs(area) < 1) continue;
//            cv::approxPolyDP(count, count, cv::arcLength(count, true) * 0.02, true);
//            rect = cv::boundingRect(count); w = rect.width; h = rect.height;
//            if (w > h) m = w;
//            else m = h;
//        }

//        for (auto count : cnt1) {
//            area = cv::contourArea(count);
//            if (abs(area) < 10) continue;
//            cv::approxPolyDP(count, count, cv::arcLength(count, true) * 0.02, true);
//            rect = cv::boundingRect(count); w = rect.width; h = rect.height;
//            if (w > h) m1 = w;
//            else m1 = h;
//        }
//        if (m != 0) r = m1 / m * 30;
//        r += r * 0.25;
//        if (40 < r and r < 180) qDebug() << r;
//        else qDebug() << "not";
//    }
}


//fishLength
void RovCameraWidget::takeFishPhoto()
{
    if (isProccessCamera)
    {
        cv::Mat img;
        cap.read(img);
        cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
        QImage image((uchar*)img.data, img.cols,
        img.rows, img.step, QImage::Format_RGB888);
        image.save("../photo/fish/" + QString::number(currentFishNum) + ".png");
        currentFishNum += 1;
        qDebug() << currentFishNum;
//        if (currentFishNum == 7) { currentFishNum = 1; qDebug() << QString::number(currentFishNum) + " fish photos"; }

        showFrame(img);
    }
}

void RovCameraWidget::takeShipPhoto()
{
    if (isProccessCamera)
    {
        cv::Mat img;
        cap.read(img);
        cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
        QImage image((uchar*)img.data, img.cols,
        img.rows, img.step, QImage::Format_RGB888);
        image.save("../photo/ship/" + QString::number(shipNum) + ".png");
        shipNum += 1;
        showFrame(img);
    }
}


//playtime
void RovCameraWidget::showFrame(cv::Mat img)
{
    long long old_time = QDateTime::currentMSecsSinceEpoch();
    long long new_time = old_time;
    cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
    while (new_time - old_time < 200)
    {
        new_time = QDateTime::currentMSecsSinceEpoch();
        cv::imshow("img", img);
        cv::waitKey(50);
    }
    cv::destroyAllWindows();
}


//garage
void* gate(cv::Mat img)
{
    //TODO: SHITTY CODE, REWRITE ASAP
//    cv::Scalar low = cv::Scalar(0, 50, 5);
//    cv::Scalar up = cv::Scalar(50, 255, 255);
//    vector<vector<cv::Point>> cnt;
//    vector<cv::Rect> rect(cnt.size());
//    int list[4]={0, 0, 0, 0};
//    cnt = findCnt(img, low, up);
//    int x, y, w, h;
//    int kx = 1, ky = 1;
//    for (auto i : cnt)
//    {
//        rect[0] = cv::boundingRect(i);
//        w = rect[0].width; h = rect[0].height; x = rect[0].x; y = rect[0].y;
//        int width = img.rows; int height = img.cols;
//        int ex = width / 2 - x; int ey = height / 2 - y; int resx = ex * kx; int resy = ey * ky;
//        list[0] = w; list[1] = resy; list[2] = resx;

//    }
//    return (void*)*list;
    return nullptr;
}

void RovCameraWidget::startStopGarage()
{
    //TODO: SHITTY CODE, REWRITE ASAP
//    if (isGarage)
//    {
//        cv::Mat img;
//        cap.read(img);
//        int w, resx, resy;
//        vector<int> list = gate(img);

//        if (w != -1 && w < 50) ;//L = false;
//    }
}
