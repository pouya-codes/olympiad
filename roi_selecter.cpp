#include "roi_selecter.h"

cv::Rect ROI ;
int ROI_x = -1 ,ROI_y = -1,ROI_w = -1,ROI_h = -1 ;
std::string wndname = "ROI Select" ;
std::vector<cv::Rect> ROIs ;
cv::Mat image,image_temp,image_draw ;
bool draw = false ;
roi_selecter::roi_selecter(cv::Mat image_in)
{
    image= image_in ;
    image.copyTo(image_draw);
    ROISelectDialog();
}
void drawROIs () {
    cv::RNG rng( 0xFFFFFFFF );
    foreach (cv::Rect rect, ROIs) {
        cv::Scalar color = cv::Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255));
        cv::rectangle(image_draw,rect,color,2) ;
    }
    cv::imshow(wndname,image_draw);
}

void mouseHandler (int event, int x, int y, int flags, void *userdata){
    image_draw.copyTo(image_temp);
    if(event==cv::EVENT_LBUTTONDOWN ) {
        ROI_x = x ;
        ROI_y = y ;
        cv::circle(image_temp,cv::Point(x,y),2,cv::Scalar(0,255,0),-1) ;
        cv::imshow(wndname,image_temp);
    }
    if(event==cv::EVENT_LBUTTONUP ) {
        if (ROI_x!=-1) {
            ROI_w = x-ROI_x ;
            ROI_h = y-ROI_y ;
            cv::Rect rect = {ROI_x,ROI_y,ROI_w,ROI_h} ;
            cv::rectangle(image_temp,rect,cv::Scalar(0,255,0),2) ;
            cv::imshow(wndname,image_temp);
            draw = false ;
        }
    }
    if(event==cv::EVENT_MBUTTONDOWN ) {
        if(ROI_x!=-1 && ROI_w !=-1) {
            cv::Rect rect = {ROI_x,ROI_y,ROI_w,ROI_h} ;
            ROIs.push_back(rect);
            drawROIs() ;
        }
    }
    if(event==cv::EVENT_RBUTTONDOWN ) {
       if (ROIs.size()!=0) {
           ROIs.pop_back();
           image.copyTo(image_draw);
           drawROIs() ;
       }
    }
}
std::vector<cv::Rect> roi_selecter::get_ROIs() {
   return ROIs ;
}


void roi_selecter::ROISelectDialog() {
    cv::namedWindow( wndname,  1 );
    cv::setMouseCallback(wndname,mouseHandler) ;
    cv::imshow(wndname,image) ;
    try {
        char c = (char)cv::waitKey();
        if( c == 27 ) {
            cv::destroyAllWindows();
            return ;

        }
    }
    catch (const std::exception e){

    }
}
