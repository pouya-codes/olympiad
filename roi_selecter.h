#ifndef ROI_SELECTER_H
#define ROI_SELECTER_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QString>

class roi_selecter
{
public:
    roi_selecter(cv::Mat image);
    std::vector<cv::Rect> get_ROIs() ;

private:
    void ROISelectDialog() ;



};

#endif // ROI_SELECTER_H
