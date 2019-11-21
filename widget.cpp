#include "widget.h"
#include "ui_widget.h"

#include "ControllableTasks.h"

#include <QProgressBar>
#include <QDateTime>
#include <QDebug>
#include <QTimer>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);


    QObject::connect(&futureWatcher, SIGNAL(started()),
                     this, SLOT(taskStarted()));
    QObject::connect(&futureWatcher, SIGNAL(canceled()),
                     this, SLOT(taskCanceled()));
    QObject::connect(&futureWatcher, SIGNAL(finished()),
                     this, SLOT(taskFinished()));
    QObject::connect(&futureWatcher, SIGNAL(paused()),
                     this, SLOT(taskPaused()));
    QObject::connect(&futureWatcher, SIGNAL(resumed()),
                     this, SLOT(taskResumed()));
    QObject::connect(&futureWatcher, SIGNAL(destroyed()),
                     this, SLOT(taskDestroyed()));
    QObject::connect(&futureWatcher, SIGNAL(progressValueChanged(int)),
                     this, SLOT(taskProgressValueChanged(int)));
    QObject::connect(&futureWatcher, SIGNAL(progressRangeChanged(int,int)),
                     this, SLOT(taskProgressRangeChanged(int,int)));


    QObject::connect(ui->pushButton_start, SIGNAL(clicked()),
                     this, SLOT(task()));

    QObject::connect(ui->pushButton_pause, SIGNAL(clicked()),
                     &futureWatcher, SLOT(pause()));
    QObject::connect(ui->pushButton_resume, SIGNAL(clicked()),
                     &futureWatcher, SLOT(resume()));
    QObject::connect(ui->pushButton_cancel, SIGNAL(clicked()),
                     &futureWatcher, SLOT(cancel()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::task()
{
    if(future.isRunning())
    {
        return;
    }

    future = TaskExecutor::run(new SomeControllableTask<int>);

    futureWatcher.setFuture(future);
}

/*============================================*/
/*==================== SLOTS==================*/
/*============================================*/

void Widget::taskStarted()
{
    QString msg = "\nTask Started";
    ui->plainTextEdit_console->appendPlainText(msg);
}

void Widget::taskCanceled()
{
    QString msg = "Task Canceled";
    ui->plainTextEdit_console->appendPlainText(msg);
}

void Widget::taskFinished()
{
    QString msg = "Task Finished";
    ui->plainTextEdit_console->appendPlainText(msg);

    if(!future.isCanceled())
        qDebug()<<"Value: "<< future.result();
}

void Widget::taskPaused()
{
    QString msg = "Task Paused";
    ui->plainTextEdit_console->appendPlainText(msg);
}

void Widget::taskResumed()
{
    QString msg = "Task Resumed";
    ui->plainTextEdit_console->appendPlainText(msg);
}

void Widget::taskDestroyed()
{
    QString msg = "Task Destroyed";
    ui->plainTextEdit_console->appendPlainText(msg);
}

void Widget::taskProgressValueChanged(int value)
{
    ui->lineEdit_progressValue->setText(QString::number(value));
}

void Widget::taskProgressRangeChanged(int minimum, int maximum)
{
    ui->lineEdit_minimum->setText(QString::number(minimum));
    ui->lineEdit_maximum->setText(QString::number(maximum));
}

void Widget::on_pushButton_clicked()
{
    future.cancel();
}
