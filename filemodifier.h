#ifndef FILEMODIFIER_H
#define FILEMODIFIER_H

#include "libs.h"

class FileModifier : public QObject
{
    Q_OBJECT
public:
    explicit FileModifier(QObject *parent = nullptr);

    void setFileMask(const QString &mask);
    void setDeleteInput(bool deleteInput);
    void setSavePath(const QString &savePath);
    void setOverwriteBehavior(bool overwrite);
    void setTimerEnabled(bool enabled);
    void setTimerInterval(int interval);
    void setXORKey(const QByteArray &key);

    void startProcessing();

private slots:
    void onTimeout();

private:
    QString fileMask;
    bool deleteInputFiles;
    QString saveDirectory;
    bool overwriteFiles;
    bool timerEnabled;
    int timerInterval;
    QByteArray xorKey;

    QTimer timer;
    QDir inputDir;

    void processFiles();
    bool modifyFile(const QString &filePath);
};

#endif // FILEMODIFIER_H
