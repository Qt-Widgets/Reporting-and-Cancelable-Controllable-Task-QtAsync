#ifndef CONTROLLABLETASKS_H
#define CONTROLLABLETASKS_H

#include <QFuture>
#include <QThreadPool>

class TaskControl
{
public:
    TaskControl(QFutureInterfaceBase* f = nullptr) : m_f(f)  {   }

    bool isCancellationRequested() const
    {
        return m_f != nullptr && m_f->isCanceled();
    }

    void setProgressValue(int value) const
    {
        if(m_f != nullptr)
            m_f->setProgressValue(value);
    }

    void setProgressRange(int minimum, int maximum) const
    {
        if(m_f != nullptr)
            m_f->setProgressRange(minimum, maximum);
    }

private:
    QFutureInterfaceBase *m_f;
};

template <class T>
class ControllableTask
{
public:
    virtual ~ControllableTask() {}
    virtual T run(TaskControl& control) = 0;
};

template <typename T>
class RunControllableTask : public QFutureInterface<T> , public QRunnable
{
public:
    RunControllableTask(ControllableTask<T>* tsk) : task(tsk) { }
    virtual ~RunControllableTask() { delete task; }

    QFuture<T> start()
    {
        this->setRunnable(this);
        this->reportStarted();
        QFuture<T> future = this->future();
        QThreadPool::globalInstance()->start(this, /*m_priority*/ 0);
        return future;
    }

    void run()
    {
        if (this->isCanceled()) {
            this->reportFinished();
            return;
        }
        TaskControl control(this);
        result = this->task->run(control);
        if (!this->isCanceled()) {
            this->reportResult(result);
        }
        this->reportFinished();
    }

    T  result;
    ControllableTask<T> *task;
};

class TaskExecutor {
public:
    template <class T>
    static QFuture<T> run(ControllableTask<T>* task) {
        return (new RunControllableTask<T>(task))->start();
    }

};


#include <QDateTime>

template <class T>
class SomeControllableTask : public ControllableTask<T>
{
public:
    virtual T run(TaskControl& control)
    {
        //long time task..
        control.setProgressRange(0, 75);

        int i = 0;
       // while(!control.isCancellationRequested()) // uncommend this line to test cancel functionality.
        {
            control.setProgressValue(i++);

            qDebug() << QDateTime::currentDateTime();
            QThread::msleep(100);

            qDebug()<<"value: " << i;
            qDebug()<<"Thread: " << QThread::currentThreadId();
        }
        qDebug() << "finished demo task!";
        return 333;
    }
};

#endif // CONTROLLABLETASKS_H
