#include "omitemptys.h"
#include <QDirIterator>
#include <QDebug>
#include <thread>
std::string OmitEmptys::createRecurciveDirectory(QString path) {
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

OmitEmptys::OmitEmptys(QString out_path,QString omit_path)
{
    this->out_path = out_path ;
    this->omit_path = omit_path ;

    threshold_empty_low= 100;
    threshold_empty_high= 100;
    std::vector <int> temp ;
    temp.push_back(0);
    ques_page.push_back(temp); //0
    temp.clear();
    temp.push_back(0);temp.push_back(1); temp.push_back(1); temp.push_back(1);temp.push_back(1); temp.push_back(2); temp.push_back(2); temp.push_back(1) ; temp.push_back(2);
    ques_page.push_back(temp); //1
    temp.clear();
    temp.push_back(0);temp.push_back(2);
    ques_page.push_back(temp); //2
    ques_page.push_back(temp); //3
    ques_page.push_back(temp); //4
    temp.clear();
    temp.push_back(0);temp.push_back(2);temp.push_back(0);temp.push_back(0);temp.push_back(1);temp.push_back(1);
    ques_page.push_back(temp); //5
    ques_page.push_back(temp); //6
    temp.clear();
    temp.push_back(0);temp.push_back(1); temp.push_back(1); temp.push_back(1);temp.push_back(1);
    ques_page.push_back(temp); //7
    temp.clear();
    temp.push_back(0);temp.push_back(1); temp.push_back(1); temp.push_back(1);
    ques_page.push_back(temp); //8
    temp.clear();
    temp.push_back(0);temp.push_back(1); temp.push_back(1); temp.push_back(1);temp.push_back(1);temp.push_back(1);temp.push_back(1);temp.push_back(1);temp.push_back(1);temp.push_back(1);temp.push_back(1);
    ques_page.push_back(temp); //9
    temp.clear();
    temp.push_back(0);temp.push_back(1); temp.push_back(1); temp.push_back(1);temp.push_back(1); temp.push_back(1); temp.push_back(1);
    ques_page.push_back(temp); //10
}

int OmitEmptys::processImages(QString in_path, bool parallel, QProgressBar &progressBar, QStatusBar &statusBar) {
    QStringList strs = in_path.split("/");
    sessionID = strs.takeLast() ;
    out_put_path_omitted_pages_session = omit_path  + "/" +sessionID +"/";
    out_put_path_omitted_pages_session_zero = out_put_path_omitted_pages_session  +  "Zero/";
    createRecurciveDirectory(out_put_path_omitted_pages_session_zero) ;
    QDir dirs( in_path );
    dirs.setFilter( QDir::AllEntries | QDir::NoDotAndDotDot );
    int total_files = dirs.count();
    progressBar.setMaximum(total_files-1);
    out_put_path_session= out_path+"/" + sessionID + "/" ;
    createRecurciveDirectory(out_put_path_session) ;





    QDir folders (in_path);
    folders.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    progressBar.setMaximum((int) folders.count());
    if (parallel) {
        uint num_threads = std::thread::hardware_concurrency();
        size_t filecounter ;
        for (filecounter = 0; filecounter<folders.count(); filecounter+=num_threads){
            if(folders.count()- filecounter <num_threads || !running)
                break ;
            std::thread threads[num_threads];
            for (int t=0; t<num_threads; ++t)
                threads[t] = std::thread(&OmitEmptys::processFolder,this,in_path+"/"+folders[filecounter+t],t);
            for (auto& th : threads) th.join();
            statusBar.showMessage("تعداد کل فایل ها: "+QString::number(folders.count())+" , پردازش شده:"+QString::number(filecounter+num_threads)+" , باقی مانده:"+QString::number(folders.count()- filecounter-num_threads));
            progressBar.setValue((int) filecounter);
            QCoreApplication::processEvents();
        }
        int remainFiles = folders.count()-filecounter ;
        if (remainFiles!=0 && running) {
            std::thread threads[remainFiles];
            for (int i=0; i<remainFiles; ++i)
                threads[i] = std::thread(&OmitEmptys::processFolder,this,in_path+"/"+folders[filecounter+i],i);
            for (auto& th : threads) th.join();
            statusBar.showMessage("تعداد کل فایل ها: "+QString::number(folders.count())+" , پردازش شده:"+QString::number(filecounter+remainFiles)+" , باقی مانده:"+QString::number(folders.count()- filecounter-remainFiles));
            progressBar.setValue((int) filecounter+remainFiles );
            QCoreApplication::processEvents();
        }

    }
    else {
        for (int counter=0;counter<folders.count(); counter++) {
            qDebug()<< in_path+"/"+folders[counter];
            QCoreApplication::processEvents();
            processFolder(in_path+"/"+folders[counter],0) ;
            progressBar.setValue((int) counter);
            statusBar.showMessage("تعداد کل پوشه ها: "+QString::number(folders.count())+" , پردازش شده:"+QString::number(counter+1)+" , باقی مانده:"+QString::number(folders.count()-counter-1));
        }
    }


    return 1;

}
int OmitEmptys::processFolder(QString folder_path, int t){
    QString candidID = folder_path.split('/').last() ;
    QString  out_put_path_std = out_put_path_session + candidID + "/" ;
    QDirIterator subDirectories(folder_path, QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);

    while(subDirectories.hasNext()){

        QString path_ques = subDirectories.next();

        QString ques_No=  path_ques.split('/').last() ;
        QString out_put_path_ques = out_put_path_std + ques_No + "/" ;
        createRecurciveDirectory(out_put_path_ques) ;
        int omit_page = ques_page.at(sessionID.toInt()).at(ques_No.toInt()) ;

        std::vector<cv::String> fn;
        cv::glob(path_ques.toStdString() +"/*.jpg", fn, false);
        size_t count = fn.size();
        for (size_t i=omit_page; i<count; i++){
            cv::Mat img = imread(fn[i], cv::IMREAD_GRAYSCALE);
            if(img.cols != 2496 || img.rows != 3520)
            {
                cv::resize(img,img,cv::Size(2496,3520)) ;
            }
            float empty_result = is_page_empty(img) ;
            if(empty_result >= threshold_empty_high) {
                QString out_put_path_file = out_put_path_ques + QString::number(i+omit_page) + ".jpg" ;
                cv::threshold(img,img,230,255,cv::THRESH_BINARY) ;
                cv::imwrite(out_put_path_file.toStdString(),img) ;
                //QFile::copy(QString::fromStdString(fn[i]),out_put_path_file);
            }
            else {
                if(empty_result >= threshold_empty_low) {
                    QString file_name = out_put_path_omitted_pages_session +sessionID+"-"+candidID +"-" + ques_No +"-" + QString::number(i+omit_page) + ".jpg" ;
//                    QFile::copy(QString::fromStdString(fn[i]),file_name);
                    cv::imwrite(file_name.toStdString(),img) ;
                }
                else {
                    QString file_name = out_put_path_omitted_pages_session_zero +sessionID+"-"+candidID +"-" + ques_No +"-" + QString::number(i+omit_page) + ".jpg" ;
//                    QFile::copy(QString::fromStdString(fn[i]),file_name);
                    cv::imwrite(file_name.toStdString(),img) ;

                }
            }

        }

    }

   return 0 ;
}

cv::Mat OmitEmptys::translateImg(cv::Mat &img, int offsetx, int offsety){
    cv::Mat trans_mat = (cv::Mat_<double>(2,3) << 1, 0, offsetx, 0, 1, offsety);
    cv::warpAffine(img,img,trans_mat,img.size(),cv::INTER_LINEAR,cv::BORDER_CONSTANT,cv::Scalar(255,255,255));
    return trans_mat;
}

float OmitEmptys::is_page_empty(cv::Mat &img)
{
    cv::Mat element = cv::getStructuringElement( cv::MORPH_ELLIPSE,
                                                 cv::Size( 5, 5 ));
    cv::Mat dilation_dst ,bin ;
    cv::rectangle(img,cv::Rect(0,0,img.cols,600),cv::Scalar(255,255,255),-1) ;
    cv::rectangle(img,cv::Rect(0,img.rows-300,img.cols,300),cv::Scalar(255,255,255),-1) ;
    cv::threshold(img(cv::Rect(300,850,1900,1900)),bin,220,255,cv::THRESH_BINARY_INV) ;
    cv::erode( bin, dilation_dst, element );
    translateImg(img,0,-150) ;
    double sum2 = cv::sum(dilation_dst).val[0]/1000 ;
    //    std::cout << sum2 << std::endl;
    return sum2;
}
