#ifndef SETTING_H
#define SETTING_H
#include <QSettings>
#include <QString>
#include <QDir>

class setting
{
public:
    setting();
    QString get_out_path() ;
    QString get_open_path() ;
    QString get_removed_path() ;

    void set_out_path(QString path) ;
    void set_open_path(QString path) ;
    void set_removed_path(QString path) ;

private:
    QSettings* setting_ ;
    const QString DEFAULT_OUT_PATH_KEY = "defult_orginal_dir";
    const QString DEFAULT_REMOVED_PATH_KEY = "defult_error_dir";
    const QString DEFAULT_OPEN_PATH_KEY = "defult_open_dir";
};

#endif // SETTING_H
