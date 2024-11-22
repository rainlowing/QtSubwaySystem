#include "appconfig.h"

#include <QFile>
#include <QDir>
#include <QApplication>

AppConfig::AppConfig() {}

AppConfig &AppConfig::instance() {
    static AppConfig instance;
    return instance;
}

bool AppConfig::initDir() {
    QString appDir = QCoreApplication::applicationDirPath();

    dataDir = QDir(appDir).filePath("data");
    scriptsDir = QDir(appDir).filePath("scripts");

    QDir dir;

    if (!dir.exists(scriptsDir)) {
        if (!dir.mkdir(scriptsDir)) {
            return false;
        } else {
            QString res = ":/scripts/script.py";
            QString target = QDir(scriptsDir).filePath("script.py");
            if (!QFile::exists(target)) {
                if (!QFile::copy(res, target)) {
                    return false;
                }
            }
        }
    }

    if (!dir.exists(dataDir)) {
        if (!dir.mkdir(dataDir)) {
            return false;
        } else {
            QString res = ":/data/data/default.txt";
            QString target = QDir(dataDir).filePath("default.txt");
            if (!QFile::exists(target)) {
                if (!QFile::copy(res, target)) {
                    return false;
                }
            }
        }
    }

    return true;
}

QString AppConfig::getDataDir() {
    return dataDir;
}

QString AppConfig::getScriptsDir() {
    return scriptsDir;
}
