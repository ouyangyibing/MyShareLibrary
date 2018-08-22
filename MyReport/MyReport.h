#ifndef CFILE_H
#define CFILE_H

#include <QIODevice>

class QString;
class QStringList;
class QDir;

/***********************************************************************************************
 * 本类提供了 csv 和 txt 写入的方法
 *
 * 以下为写入csv的demo，txt同理，只是txt不用设置表头
 * CFile report;
 * report.setReportName(appTitle);
 * report.setCsvHeader(QStringList);
 * report.writeTxt(QStringList);
 *
 * 函数说明：
 * setReportName() 推荐使用APP标题名作为函数参数，便于生成统一文件路径和文件名(类似串口对象定义时初始化一次)
 * 或者在定义对象时，使用构造函数传入 _appTitle 参数
 *
 * getTxtName() 需在调用函数setReportName()之后才能获取文件路径
 *
 * getTxtPath() 参考getTxtName()
 *
 * getCsvName() 参考getTxtName()
 *
 * getCsvPath() 参考getTxtName()
 *
 * getCsvHeader() 需在调用函数 setCsvHeader()之后才能获取文件路径
 *
 * setCsvHeader(const QString&) 设置csv表头
 *
 * setCsvHeader(const QStringList&) 设置csv表头，以逗号为分隔符，末尾加"\n"
 *
 * readFile() 本函数为静态函数，可给出文件路径，直接返回文件数据
 *
 * writeTxt() 本函数以追加方式写入文件，需要提前调用函数 setReportName()
 *
 * static writeTxt() 本函数为静态函数，重载writeTxt() ，无需提前提前调用函数 setReportName()，
 * 但需提供文件名和文件路径
 *
 * writeCsv(const QString&) 写入cxv 表格数据，需要提前调用函数 setReportName()
 * writeCsv(const QStringList&) 重载writeCsv(const QString&)，以逗号为分隔符，末尾加"\n"
 * static writeCsv() 本函数为静态函数，重载 writeCsv(),无需提前调用函数 setReportName()，
 * 但需提供文件名和文件路径
 ***********************************************************************************************/

class  MyReport
{
public:
    MyReport(const QString _appTitle);
    MyReport(){}
    void setReportName(const QString _appTitle);
    QString getTxtName(const QString &barcode);
    QString getTxtPath();
    QString getCsvName();
    QString getCsvPath();
    QString getCsvHeader() const;
    void setCsvHeader(const QString &value);
    void setCsvHeader(const QStringList &value);
    static QByteArray readFile(QString filePathName);

    void writeTxt(const QByteArray &fileData, const QByteArray &result, const QString &barcode = "");
    static void writeTxt(const QByteArray &fileData, const QByteArray &result, const QString &fileName, const QString &filePath);
    static void writeTxt(const QByteArray &fileData, const QString &fileName, QIODevice::OpenMode flags );

    void writeCsv(const QString &fileData);
    void writeCsv(const QStringList &fileData);
    static void writeCsv(const QString &header, const QString &fileData, const QString &fileName, const QString &filePath);

    QString getBasePath() const;
    void setBasePath(const QString &value);

private:
    QString appTitle;
    QString basePath = "D:/";
    QString csvHeader;
};

#endif // CFILE_H
