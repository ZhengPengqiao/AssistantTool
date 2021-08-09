#ifndef DATAUTIL_H
#define DATAUTIL_H

#include <QString>

class DataUtil
{
public:
    DataUtil();
    /**
     * @brief readAscFile
     * 将文件内容读取到字符串列表中
     * @param fileName:文件名及路径
     * @return:包含文件内容的字符串列表
     */
    static QStringList readAscFile(QString fileName);

    /**
     * @brief DataUtil::readAscSignal
     * @param strList:所有asc数据
     * @param signalIndex:有效数据的偏移位置
     * @param idOffset:ID的偏移位置
     * @param signalID:信号ID
     * @param signalOffset:信号值的偏移量
     * @param signalLen:信号值占用位数
     * @param time:时间轴
     * @param val:数据轴
     * @param Tx:是否检测Tx方向， True检测，False不检测
     * @param Rx:是否检测Tx方向， True检测，False不检测
     * @return:过滤出来的数据个数
     */
    static int readAscSignal(QStringList strList, int signalIndex, int idOffset, int signalID, int signalOffset, int signalLen, QVector<double> *time, QVector<double> *val, bool Tx, bool Rx);

    /**
     * @brief DataUtil::getArrayOffsetValue
     * 从字符串列表中获取对应的数据
     * @param strList:字符串列表
     * @param signalIndex:真实有效数据偏移位置
     * @param offset:提取数 = 据在有效数据中 位的偏移位置， 位高端 0bxxx0 0000,xxx就是offset=7 len=3
     * @param len:待提取的位数
     * @return :提取出来的数值， >0:正常-1:异常
     */
    static int getArrayOffsetValue(QStringList strList,  int signalIndex, int offset, int len);


    static QString getAscVersion(QStringList strList);
};

#endif // DATAUTIL_H
