#ifndef BARCODEREADER_H
#define BARCODEREADER_H
#include <QString>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <roi_selecter.h>
#include <QProgressBar>
#include <QStatusBar>
class BarcodeReader
{
private :
    QString out_path ;
    int processImage(std::string img_path,int t) ;
    cv::Size pic_size ;
    int error_ ;
    std::vector<cv::Rect> ROIs ;
    bool running = true ;
public:
    BarcodeReader(QString out_path);
    int processImages(QString in_path,bool parallel,QProgressBar &progressBar,QStatusBar &statusBar) ;
};

#endif // BARCODEREADER_H
