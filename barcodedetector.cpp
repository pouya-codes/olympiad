#include "barcodedetector.h"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

BarcodeDetector::BarcodeDetector(bool olympiad)
{
    this->olympiad = olympiad ;
    decoder.setDecoder(QZXing::DecoderFormat_QR_CODE | QZXing::DecoderFormat_CODE_128  );

}
QString BarcodeDetector::readBarcode(cv::Mat img,std::vector<cv::Rect> ROIs) {
    QString bar_code ;
    foreach (cv::Rect ROI, ROIs) {
        //        cv::imshow("sa",img(ROI)) ;
        //        cv::waitKey(0) ;
        bar_code = decoder.decodeImage(ASM::cvMatToQImage(img(ROI)));
        if (olympiad) {
            if (bar_code.indexOf('-')==-1 && bar_code.size()!=0)
                bar_code= bar_code.mid(0,6) +"-"+ QString::number(bar_code.mid(6,2).toInt()) +"-"+ QString::number(bar_code.mid(8,2).toInt()) +"-"+ QString::number(bar_code.mid(10,2).toInt())   ;

            if (bar_code.size()>9 && bar_code.indexOf('-')!=-1)
                return bar_code ;
        }
        else {
            if ( bar_code.size()==0)
                bar_code = decoder.decodeImage(ASM::cvMatToQImage(img));
            return bar_code ;
        }

    }

    return "" ;
}
