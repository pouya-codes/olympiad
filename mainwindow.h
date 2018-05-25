#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <setting.h>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QProgressBar>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <dirent.h>
#include <QZXing.h>
#include <asmOpenCV.h>
#include <QFile>
#include <QDir>
#include <thread>
#include <roi_selecter.h>

using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_out_clicked();

    void on_pushButton_removed_clicked();

    void on_pushButton_select_roi_clicked();

    void on_pushButton_open_out_clicked();

    void on_pushButton_open_remove_clicked();

private:
    Ui::MainWindow *ui;
    setting* setting_ ;
    int detect_circle(Mat img) ;
    QString read_bar(Mat img);
    QString read_bar(Mat img,std::vector<cv::Rect> ROIs) ;
    void process_image(cv::String file_path,int thread);
    std::string createRecurciveDirectory(QString path) ;
    int error_counter = 0 ;
    bool running =false ;

};

#endif // MAINWINDOW_H
