#include "filecollector.h"
#include "barcodedetector.h"

std::string FileCollector::createRecurciveDirectory(QString path) {
    QStringList strs = path.split('/') ;
    QString outpath = "" ;
    for (uint idx = 0; idx < strs.size(); ++idx) {
        outpath+=strs[idx] +"/" ;
        if(!QDir(outpath).exists()) {
            QDir().mkdir(outpath);
        }
    }
    return outpath.toStdString() ;
}


FileCollector::FileCollector(QString out_path)
{
 this->out_path= out_path ;
    ROIs.push_back(cv::Rect(400,200,600,700));
    ROIs.push_back(cv::Rect(1400,200,800,700));
    ROIs.push_back(cv::Rect(100,1000,900,1300));

}
int FileCollector::processImages(QString in_path, bool parallel, QProgressBar &progressBar, QStatusBar &statusBar) {
    std::vector<cv::String> fn;
    cv::glob(in_path.toStdString() +"/*.jpg", fn, false);
    size_t count = fn.size();
    progressBar.setMaximum((int) count-1);
    if (parallel) {
        uint num_threads = std::thread::hardware_concurrency();
        size_t filecounter ;
        for (filecounter = 0; filecounter<count; filecounter+=num_threads){
            if(count- filecounter <num_threads || !running)
                break ;
            std::thread threads[num_threads];
            for (int t=0; t<num_threads; ++t)
                threads[t] = std::thread(&FileCollector::processImage,this,fn[filecounter+t],t);
            for (auto& th : threads) th.join();
            statusBar.showMessage("تعداد کل فایل ها: "+QString::number(count)+" , پردازش شده:"+QString::number(filecounter+num_threads)+" , باقی مانده:"+QString::number(count- filecounter-num_threads));
            progressBar.setValue((int) filecounter);
            QCoreApplication::processEvents();
        }
        int remainFiles = count-filecounter ;
        if (remainFiles!=0 && running) {
            std::thread threads[remainFiles];
            for (int i=0; i<remainFiles; ++i)
                threads[i] = std::thread(&FileCollector::processImage,this,fn[filecounter+i],i);
            for (auto& th : threads) th.join();
            statusBar.showMessage("تعداد کل فایل ها: "+QString::number(count)+" , پردازش شده:"+QString::number(filecounter+remainFiles)+" , باقی مانده:"+QString::number(count- filecounter-remainFiles));
            progressBar.setValue((int) filecounter+remainFiles -1);
            QCoreApplication::processEvents();
        }


    }
    else
        for (size_t i=0; i<count; i++){
            processImage(fn[i],-1);
            progressBar.setValue((int) i);
            QCoreApplication::processEvents();

        }
    return 1 ;
}

int FileCollector::processImage(std::string img_path, int t) {
    QString candidID = "" ;
    QString sessionID =  "";
    QString qusNo =  "" ;
    QString pageNo ="" ;
    cv::Mat img = imread(img_path, cv::IMREAD_GRAYSCALE);
    if(img.empty())
    {
        return -1 ;
    }
    if(img.cols != 2496 || img.rows != 3520)
        cv::resize(img,img,cv::Size(2496,3520)) ;
    BarcodeDetector *detector= new BarcodeDetector(true);
    QString barcode = detector->readBarcode(img,ROIs) ;
    if(barcode.isEmpty()) { //error or empty
        cv::Mat thr ;
        cv::threshold(img,thr,100,255,cv::THRESH_BINARY_INV);
        double sum = cv::sum(thr).val[0] ;
        if(sum >500000) {
            if (qusNo=="0") return 0; // empty
            error_+=1 ;
            QString out_put_path_std = out_path +"/"+ sessionID + "/" ;

            out_put_path_std += "error/"  ;
            createRecurciveDirectory(out_put_path_std) ;
            out_put_path_std += QString::number(error_) ;
            QFile::copy(QString::fromStdString(img_path),out_put_path_std+QString::number(t)+".jpg");
        }
    }
    else {
        QString qr_code = barcode ;
        QStringList strs = qr_code.split("-") ;
        candidID = strs[0] ;
        sessionID = strs[1] ;
        qusNo = strs[2] ;
        pageNo = strs[3] ;
        QString out_put_path_std = out_path +"/"+ sessionID + "/" + candidID + "/" ;
        createRecurciveDirectory(out_put_path_std) ;
        if (pageNo!="0") { // question pages
            out_put_path_std += qusNo + "/"  ;
            createRecurciveDirectory(out_put_path_std) ;
            out_put_path_std += pageNo  ;
        }
        else // cover page
            out_put_path_std += candidID  ;

        QFile::copy(QString::fromStdString(img_path),out_put_path_std+".jpg");
    }
}
