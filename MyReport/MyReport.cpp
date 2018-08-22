#include "MyReport.h"
#include <QString>
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QDebug>
#include <QRegularExpression>

MyReport::MyReport(const QString _appTitle) :
    appTitle(_appTitle)
{

}

void MyReport::setReportName(const QString _appTitle)
{
    appTitle = _appTitle;
}

QString MyReport::getTxtName(const QString &barcode)
{
    return appTitle+"_Report_"+QDateTime::currentDateTime().toString("hhmmss_")+barcode+".txt";
}

QString MyReport::getTxtPath()
{
    return basePath +"Text/"+ appTitle + "_Report_"+QDateTime::currentDateTime().toString("yyyy-MM-dd")+"/";
}

QString MyReport::getCsvName()
{
    return  appTitle + "_Report_" +QDateTime::currentDateTime().toString("yyyy-MM-dd")+".csv";
}

QString MyReport::getCsvPath()
{
    return  basePath+"/CSV/";
}

/****************************************************
 * 本函数会创建一个 text 文档，用于保存原始报表
 * fileData:将要写入文件中的数据
 * result:本次测试结果
 * fileName:报表文件名，如 file.txt
 * filePath:报表所要储存的路径，格式：“E:/Qt_Report/”
 ****************************************************/
void MyReport::writeTxt(const QByteArray &fileData, const QByteArray &result, const QString &fileName, const QString &filePath)
{
    QDir dir;
    dir.mkpath(filePath);

    QFile file(filePath+fileName);
    if (file.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append)) {  //以追加的模式打开文件
        file.write(QByteArray("Test start-----------------------------------------------------------------------------\n\n"
                             +fileData
                             +"\n\n****************************\n"
                              "\n***********"+result+"*************\n"
                              "\n****************************\n\n\n\n\n"));
    }
    file.close();
}

void MyReport::writeTxt(const QByteArray &fileData, const QString &fileName, QIODevice::OpenMode flags)
{
    QFile file(fileName);
    if (file.open(flags)) {  //以追加的模式打开文件
        file.write(fileData);
    }
    file.close();
}

void MyReport::writeTxt(const QByteArray &fileData, const QByteArray &result, const QString &barcode)
{
    QDir dir;
    dir.mkpath(getTxtPath());

    QFile file(getTxtPath()+getTxtName(barcode));
    if (file.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append)) {  //以追加的模式打开文件
        file.write(QByteArray("Test start-----------------------------------------------------------------------------\n\n"
                             +fileData
                             +"\n\n****************************\n"
                              "\n***********"+result+"*************\n"
                              "\n****************************\n\n\n\n\n"));
    }
    file.close();
}

void MyReport::writeCsv(const QString &fileData)
{
    QDir dir;
    dir.mkpath(getCsvPath());
    QFile file(getCsvPath()+getCsvName());
    if(!file.open(QIODevice::ReadOnly))
    {
        file.close();
        if (file.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append))
        {
            file.write(csvHeader.toLocal8Bit());
        }
    }
    file.close();

    if (file.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append))   //以追加的模式打开文件
    {
        file.write(fileData.toLocal8Bit());
    }
    file.close();
}

void MyReport::writeCsv(const QStringList &fileData)
{
   writeCsv(fileData.join(",")+"\n");
}

QByteArray MyReport::readFile(QString filePathName)
{
    QByteArray fileData;
    QFile file(filePathName);
    if(file.open(QIODevice::ReadOnly)) {
        fileData = file.readAll();
    }
    else {
        fileData = "错误：文件读取失败！！！";
    }
    return fileData;
}

/****************************************************
 * 本函数会创建一个 csv 报表
 * fileData:将要写入文件中的数据
 * fileName:报表文件名，如 "file_" ,本函数将会把日期添加到文件名,
 * 并添加后缀，如“ file_170928.csv”
 * filePath:报表所要储存的路径，格式：“E:/Qt_Report/”
 ****************************************************/
void MyReport::writeCsv(const QString &header, const QString &fileData, const QString &fileName, const QString &filePath)
{
    QDir dir;
    dir.mkpath(filePath);
    QFile file(filePath+fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        file.close();
        if (file.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append))
        {
            file.write(header.toLocal8Bit());
        }
    }
    file.close();

    if (file.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append))   //以追加的模式打开文件
    {
        file.write(fileData.toLocal8Bit());
    }
    file.close();
}

QString MyReport::getBasePath() const
{
    return basePath;
}

void MyReport::setBasePath(const QString &value)
{
    basePath = value;
    QRegularExpression re("/$");
    if (!basePath.contains(re)) {
        basePath += "/";
    }
}

QString MyReport::getCsvHeader() const
{
    return csvHeader;
}

void MyReport::setCsvHeader(const QStringList &value)
{
    csvHeader = value.join(",")+"\n";
}

void MyReport::setCsvHeader(const QString &value)
{
    csvHeader = value;
}
