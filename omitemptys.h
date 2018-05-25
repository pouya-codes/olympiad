#ifndef OMITEMPTYS_H
#define OMITEMPTYS_H
#include <QString>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <QProgressBar>
#include <QStatusBar>
#include <QDir>
#include <dirent.h>
#include <QCoreApplication>

class OmitEmptys
{
private :
    QString out_path ;
    QString omit_path ;
    int processFolder(QString folder_path,int t) ;
    cv::Size pic_size ;
    bool running = true ;
    int threshold_empty_low;
    int threshold_empty_high;
    std::vector<std::vector <int>> ques_page ;
    std::string createRecurciveDirectory(QString path) ;
    float is_page_empty(cv::Mat &img) ;
    QString out_put_path_session,sessionID,
    out_put_path_omitted_pages_session_zero,
    out_put_path_omitted_pages_session;
    cv::Mat translateImg(cv::Mat &img, int offsetx, int offsety);

public:
    OmitEmptys(QString out_path,QString omit_path);
    int processImages(QString in_path,bool parallel,QProgressBar &progressBar,QStatusBar &statusBar) ;
};

#endif // OMITEMPTYS_H
