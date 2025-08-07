#ifndef FILEMETADATA_H
#define FILEMETADATA_H

#include <QString>

class FileMetadata
{
public:
    /**
     * @brief FileMetadata Constructor of the class
     * @param name The name of the file
     * @param chunkSize Size of a file chunk
     * @param chunkCount Number of file chunks
     * @param directoryPath The path to store the file
     */
    FileMetadata(QString name, int chunkSize, int chunkCount, QString directoryPath);


    /**
     * @brief toString Returns the file metadata as a string
     * @return string with format <name>;<chunkSize>;<chunkCount>
     */
    QString toString();

    /**
     * @brief getName Returns the file name
     * @return The file name
     */
    QString getName();

    /**
     * @brief getChunkSize Returns the size of a file chunk
     * @return The size of a file chunk
     */
    int getChunkSize();

    /**
     * @brief getChunkCount Returns the number of file chunks
     * @return The number of file chunks
     */
    int getChunkCount();


    /**
     * @brief getDirectoryPath Returns the directory path
     * @return The directory path
     */
    QString getDirectoryPath();

    /**
     * @brief getFullFilePath Returns the full path of the file
     * @return directoryPath/name
     */
    QString getFullFilePath();





private:

    /**
     * @brief name  Name of the file to be transferred/retrieved
     */
    QString name;


    /**
     * @brief chunksize Size of a file chunk
     */
    int chunkSize;



    /**
     * @brief chunCount Number of file chunks
    */
    int chunkCount;

    /**
     * @brief directoryPath The path to store the file
     */
    QString directoryPath;
};

#endif // FILEMETADATA_H
