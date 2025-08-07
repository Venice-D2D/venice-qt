#include "include/metadata/filemetadata.h"

#include <QDir>

FileMetadata::FileMetadata(QString name, int chunkSize, int chunkCount, QString directoryPath) {

    this->name = name;
    this->chunkSize = chunkSize;
    this->chunkCount = chunkCount;
    this->directoryPath = directoryPath;
}



QString FileMetadata::toString(){
    return this->name+";"+QString::number(this->chunkSize)+";"+QString::number(this->chunkCount);
}


QString FileMetadata::getName(){
    return this->name;
}


int FileMetadata::getChunkSize(){
    return this->chunkSize;
}


int FileMetadata::getChunkCount(){
    return this->chunkCount;
}

QString FileMetadata::getDirectoryPath(){
    return this->directoryPath;
}

QString FileMetadata::getFullFilePath(){
    QDir fileDirectory(this->directoryPath);

    return fileDirectory.filePath(this->name);
}

