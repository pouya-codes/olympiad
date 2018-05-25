#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "barcodereader.h"
#include "filecollector.h"
#include "omitemptys.h"
#include <QDesktopServices>



std::string MainWindow::createRecurciveDirectory(QString path) {
    QStringList strs = path.split('/') ;
    QString outpath = "" ;
    for (int idx = 0; idx < strs.size(); ++idx) {
        outpath+=strs[idx] +"/" ;
        if(!QDir(outpath).exists()) {
            QDir().mkdir(outpath);
        }
    }
    return outpath.toStdString() ;
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    setting_ = new setting();

    ui->lineEdit_out->setText(setting_->get_out_path());
    ui->lineEdit_removed->setText(setting_->get_removed_path());

    createRecurciveDirectory(setting_->get_out_path()) ;
    createRecurciveDirectory(setting_->get_removed_path()) ;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    QString source = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    setting_->get_open_path(),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(source!="") {
        running = true ;
        setting_->set_open_path(source) ;

        createRecurciveDirectory(setting_->get_out_path()) ;
        FileCollector *fileCollector = new FileCollector(setting_->get_out_path()) ;
        int result = fileCollector->processImages(source,ui->checkBoxMultiThread->isChecked(),*ui->progressBar,*ui->statusBar) ;
        if (result ==1 )
            QMessageBox::information(this,"Result","All Files Procseed");
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QString source = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                       setting_->get_open_path(),
                                                       QFileDialog::ShowDirsOnly
                                                       | QFileDialog::DontResolveSymlinks);
    if(source!="") {
        setting_->set_open_path(source);

        OmitEmptys *fileCollector = new OmitEmptys(setting_->get_out_path(),setting_->get_removed_path()) ;
        int result = fileCollector->processImages(source,ui->checkBoxMultiThread->isChecked(),*ui->progressBar,*ui->statusBar) ;
        if (result ==1 )
            QMessageBox::information(this,"Result","All Files Procseed");
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    QString source = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                       setting_->get_open_path(),
                                                       QFileDialog::ShowDirsOnly
                                                       | QFileDialog::DontResolveSymlinks);
    if(source!="") {
        setting_->set_open_path(source);
        QString out_path = setting_->get_out_path() + "/";
        createRecurciveDirectory(out_path) ;

        BarcodeReader *barcodeReader = new BarcodeReader(out_path) ;
        int result =barcodeReader->processImages(source,ui->checkBoxMultiThread->isChecked(),*ui->progressBar,*ui->statusBar) ;
        if (result ==1 )
            QMessageBox::information(this,"Result","All Files Procseed");
    }

}



void MainWindow::on_pushButton_out_clicked()
{
    QString source = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                       setting_->get_out_path(),
                                                       QFileDialog::ShowDirsOnly
                                                       | QFileDialog::DontResolveSymlinks);
    if(source!="") {
        ui->lineEdit_out->setText(source);
        setting_->set_out_path(source);

    }
}

void MainWindow::on_pushButton_removed_clicked()
{
    QString source = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                       setting_->get_removed_path(),
                                                       QFileDialog::ShowDirsOnly
                                                       | QFileDialog::DontResolveSymlinks);
    if(source!="") {
        ui->lineEdit_removed->setText(source);
        setting_->set_removed_path(source);

    }
}


void MainWindow::on_pushButton_select_roi_clicked()
{
    cv::Mat image = cv::imread("D:/pic.jpg") ;
    roi_selecter select(image) ;
    std::cout << select.get_ROIs().size() << std::endl ;
}
int detect_circle(Mat img)
{

    Mat gray;
    img.copyTo(gray);
    //    medianBlur(img, gray, 5);
    vector<Vec3f> circles;

    HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 1,
                 gray.rows/16, // change this value to detect circles with different distances to each other
                 100, 30, 15, 30 // change the last two parameters
                 // (min_radius & max_radius) to detect larger circles
                 );
    for( size_t i = 0; i < circles.size(); i++ )
    {
        Vec3i c = circles[i];
        circle( img, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, 8);
        circle( img, Point(c[0], c[1]), 2, Scalar(0,255,0), 3, 8);
    }
    std::cout << circles.size() << std::endl ;

    cv::Mat resize ;
    cv::resize(img,resize,cv::Size(600,1000));
    imshow("detected circles", resize);
    waitKey();

    return 0;

}

void MainWindow::on_pushButton_open_out_clicked()
{
    QDesktopServices::openUrl( QUrl::fromLocalFile( ui->lineEdit_out->text())) ;
}

void MainWindow::on_pushButton_open_remove_clicked()
{
     QDesktopServices::openUrl( QUrl::fromLocalFile( ui->lineEdit_removed->text())) ;
}
