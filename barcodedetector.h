#ifndef BARCODEDETECTOR_H
#define BARCODEDETECTOR_H
#include <asmOpenCV.h>
#include <QZXing>
#include <opencv2/core.hpp>
class BarcodeDetector
{
private:
    QZXing decoder ;
    bool olympiad  ;

public:
    BarcodeDetector(bool olympiad=false);
    QString readBarcode(cv::Mat img, std::vector<cv::Rect> ROIs) ;

};

#endif // BARCODEDETECTOR_H
