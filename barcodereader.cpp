#include "barcodereader.h"
#include <QApplication>
#include <QFile>
#include <barcodedetector.h>
#include <thread>
BarcodeReader::BarcodeReader(QString out_path)
{
    this->out_path = out_path ;
}
int BarcodeReader::processImages(QString in_path,bool parallel,QProgressBar &progressBar,QStatusBar &statusBar) {


    error_ = 0 ;

    std::vector<cv::String> fn;
    cv::glob(in_path.toStdString() +"/*.jpg", fn, false);

    size_t count = fn.size();
    if (count != 0) {
        cv::Mat img = cv::imread(fn[0], cv::IMREAD_GRAYSCALE);
        pic_size = cv::Size(img.cols,img.rows) ;
        roi_selecter select(img) ;
        ROIs = select.get_ROIs() ;

        progressBar.setMaximum((int) count-1);
    }
    if (parallel) {
        uint num_threads = std::thread::hardware_concurrency();
        size_t filecounter ;
        for (filecounter = 0; filecounter<count; filecounter+=num_threads){
            if(count- filecounter <num_threads || !running)
                break ;
            std::thread threads[num_threads];
            for (int t=0; t<num_threads; ++t)
                threads[t] = std::thread(&BarcodeReader::processImage,this,fn[filecounter+t],t);
            for (auto& th : threads) th.join();
            statusBar.showMessage("تعداد کل فایل ها: "+QString::number(count)+" , پردازش شده:"+QString::number(filecounter+num_threads)+" , باقی مانده:"+QString::number(count- filecounter-num_threads));
            progressBar.setValue((int) filecounter);
            QCoreApplication::processEvents();
        }
        int remainFiles = count-filecounter ;
        if (remainFiles!=0 && running) {
            std::thread threads[remainFiles];
            for (int i=0; i<remainFiles; ++i)
                threads[i] = std::thread(&BarcodeReader::processImage,this,fn[filecounter+i],i);
            for (auto& th : threads) th.join();
            statusBar.showMessage("تعداد کل فایل ها: "+QString::number(count)+" , پردازش شده:"+QString::number(filecounter+remainFiles)+" , باقی مانده:"+QString::number(count- filecounter-remainFiles));
            progressBar.setValue((int) filecounter+remainFiles -1);
            QCoreApplication::processEvents();
        }


    }
    else {
        for (size_t i=0; i<count; i++){
            progressBar.setValue((int) i);
            QCoreApplication::processEvents();

            cv::Mat img = imread(fn[i], cv::IMREAD_GRAYSCALE);
            if(img.cols != pic_size.width || img.rows != pic_size.height)
                cv::resize(img,img,pic_size) ;
            BarcodeDetector *detector= new BarcodeDetector(false);
            QString bar_code = detector->readBarcode(img,ROIs) ;

            if (bar_code!=" ")
                QFile::copy(QString::fromStdString(fn[i]),out_path+bar_code+".jpg");
            else {
                QFile::copy(QString::fromStdString(fn[i]),out_path+QString::number(error_)+".jpg");
                error_++ ;
            }
        }
    }


    return 0 ;
}
int BarcodeReader::processImage(std::string img_path,int t)  {

    cv::Mat img = imread(img_path, cv::IMREAD_GRAYSCALE);
    if(img.cols != pic_size.width || img.rows != pic_size.height)
        cv::resize(img,img,pic_size) ;
    BarcodeDetector *detector= new BarcodeDetector();
    QString bar_code = detector->readBarcode(img,ROIs) ;

    if (bar_code.isEmpty())
    {
        QFile::copy(QString::fromStdString(img_path),out_path+QString::number(error_)+"_"+QString::number(t)+".jpg");
        error_++ ;
    }
    else
        QFile::copy(QString::fromStdString(img_path),out_path+bar_code+".jpg");

}
