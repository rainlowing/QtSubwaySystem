#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QString>

// 全局设置类
class AppConfig
{
public:
    AppConfig();
    static AppConfig& instance();

    bool initDir();
    QString getDataDir();
    QString getScriptsDir();

private:
    QString dataDir;
    QString scriptsDir;
};

#endif // APPCONFIG_H
