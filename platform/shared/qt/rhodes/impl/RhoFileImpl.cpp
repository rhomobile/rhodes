#include "common/RhoPort.h"
#undef null
#include <QDir>

bool RemoveDirectory(QString &path, bool deleteDir)
{
    QDir aDir(path);
    bool has_err = false;
    if (aDir.exists()) {
        QFileInfoList entries = aDir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
        int count = entries.size();
        for (int idx = 0; ((idx < count) && (!has_err)); idx++) {
            QFileInfo entryInfo = entries[idx];
            QString path = entryInfo.absoluteFilePath();
            if (entryInfo.isDir()) {
                has_err = RemoveDirectory(path, true);
            } else {
                QFile file(path);
                if (!file.remove())
                    has_err = true;
            }
        }
        if (deleteDir && (!aDir.rmdir(aDir.absolutePath())))
            has_err = true;
    }
    return has_err;
}

extern "C" {

void delete_files_in_folder(const char *szFolderPath)
{
    QString path = QString::fromUtf8(szFolderPath);
    RemoveDirectory(path, false);
}

} //extern "C"
