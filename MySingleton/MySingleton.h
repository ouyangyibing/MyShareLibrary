#ifndef MYSINGLETON_H
#define MYSINGLETON_H

#include <QMutex>
#include <QDebug>

/*******************************************************
 * 此类废弃
 * 含ui 类在继承此类时
 * 如使用了 QComboBox
 * 程序在退出时会崩溃
 * 故废弃
 * 其实Qt 也不推荐继承模板类
 * ****************************************************/



#ifndef mySingleton
#define mySingleton(CLASS) MySingleton<CLASS>::Instance()
#endif  //mySingleton

template<typename T>
class MySingleton
{
public:
    //静态方法获取实例
    static  T* Instance();

protected:
    MySingleton()
    {
        // 用于释放单例
        static GC gc;
        //qDebug() << "MySingleton = "<<this;
    }

    ~MySingleton()
    {
        //如果被删除
        self = nullptr;
        //qDebug() << "~MySingleton = "<<this;
    }

private:
    //防止拷贝构造一个实例
    MySingleton(const MySingleton&) {}
    //防止赋值出另一个实例
    MySingleton& operator=(const MySingleton&){}

    static T* self;

    //程序结束时，回收资源
    class GC
    {
    public:
        ~GC()
        {
            if (self != nullptr) {
                //qDebug() << "~GC" << this;
                delete self;
                self = nullptr;
            }
        }
    };
};

/************************
 * 模板不支持分离编译，
 * 所以函数不可以放cpp文件
 * *********************/
template<typename T>
T* MySingleton<T>::Instance()
{
    if(!self) {
        //static QMutex muter;
        //QMutexLocker clocker(&muter);

        if(!self) {
            self = new T();
        }
    }

    return self;
}

template<typename T>
T* MySingleton<T>::self = nullptr;

#endif // MYSINGLETON_H
