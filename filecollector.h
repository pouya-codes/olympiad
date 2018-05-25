#ifndef FILECOLLECTOR_H
#define FILECOLLECTOR_H
#include <barcodereader.h>
#include <thread>

class FileCollector
{
private :
    QString out_path ;
    int processImage(std::string img_path,int t) ;
    cv::Size pic_size ;
    int error_ ;
    std::vector<cv::Rect> ROIs ;
    bool running = true ;
    std::string createRecurciveDirectory(QString path);

public:
    FileCollector(QString out_path);
    int processImages(QString in_path,bool parallel,QProgressBar &progressBar,QStatusBar &statusBar) ;
};

#endif // FILECOLLECTOR_H
