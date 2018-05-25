#include "setting.h"

setting::setting()
{
    setting_ = new QSettings("setting.ini", QSettings::IniFormat) ;
}
QString setting::get_out_path() {
    return setting_->value(DEFAULT_OUT_PATH_KEY,QDir::currentPath()).toString() ;
}
QString setting::get_open_path() {
    return setting_->value(DEFAULT_OPEN_PATH_KEY,QDir::currentPath()).toString() ;
}
QString setting::get_removed_path() {
    return setting_->value(DEFAULT_REMOVED_PATH_KEY,QDir::currentPath()).toString() ;
}

void setting::set_out_path(QString path) {
    setting_->setValue(DEFAULT_OUT_PATH_KEY,path) ;
//    setting_->setValue(DEFAULT_OPEN_PATH_KEY,path) ;
    return ;
}
void setting::set_open_path(QString path)  {
    setting_->setValue(DEFAULT_OPEN_PATH_KEY,path) ;
    return ;
}
void setting::set_removed_path(QString path)  {
    setting_->setValue(DEFAULT_REMOVED_PATH_KEY,path) ;
//    setting_->setValue(DEFAULT_OPEN_PATH_KEY,path) ;
    return ;
}
