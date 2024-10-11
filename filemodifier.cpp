#include "filemodifier.h"

FileModifier::FileModifier(QObject *parent) : QObject(parent), deleteInputFiles(false), overwriteFiles(false), timerEnabled(false), timerInterval(1000)
{
    connect(&timer, &QTimer::timeout, this, &FileModifier::onTimeout);
}

void FileModifier::setFileMask(const QString &mask) {
    fileMask = mask;
}

void FileModifier::setDeleteInput(bool deleteInput) {
    deleteInputFiles = deleteInput;
}

void FileModifier::setSavePath(const QString &savePath) {
    saveDirectory = savePath;
    QDir dir(savePath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
}

void FileModifier::setOverwriteBehavior(bool overwrite) {
    overwriteFiles = overwrite;
}

void FileModifier::setTimerEnabled(bool enabled) {
    timerEnabled = enabled;
}

void FileModifier::setTimerInterval(int interval) {
    timerInterval = interval;
}

void FileModifier::setXORKey(const QByteArray &key) {
    xorKey = key;
}

void FileModifier::startProcessing() {
    if (timerEnabled) {
        timer.start(timerInterval);
    } else {
        processFiles();
    }
}

void FileModifier::onTimeout() {
    processFiles();
}

void FileModifier::processFiles() {
    inputDir.setNameFilters(QStringList() << fileMask);
    QStringList files = inputDir.entryList(QDir::Files);
    for (const QString &file : files) {
        QString filePath = inputDir.absoluteFilePath(file);
        if (modifyFile(filePath)) {
            if (deleteInputFiles) {
                QFile::remove(filePath);
            }
        }
    }
}

bool FileModifier::modifyFile(const QString &filePath) {
    QFile inputFile(filePath);
    if (!inputFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open file for reading: " << filePath;
        return false;
    }

    QByteArray fileData = inputFile.readAll();
    inputFile.close();

    // XOR Operation
    for (int i = 0; i < fileData.size(); i++) {
        fileData[i] ^= xorKey[i % xorKey.size()];
    }

    QFileInfo fileInfo(filePath);
    QString outputFilePath = saveDirectory + "/" + fileInfo.fileName();

    if (!overwriteFiles && QFile::exists(outputFilePath)) {
        // Add a counter to the file name if overwriting is disabled
        int counter = 1;
        while (QFile::exists(outputFilePath)) {
            outputFilePath = saveDirectory + "/" + fileInfo.completeBaseName() + "_" + QString::number(counter) + "." + fileInfo.suffix();
            counter++;
        }
    }

    QFile outputFile(outputFilePath);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Cannot open file for writing: " << outputFilePath;
        return false;
    }

    outputFile.write(fileData);
    outputFile.close();
    return true;
}
