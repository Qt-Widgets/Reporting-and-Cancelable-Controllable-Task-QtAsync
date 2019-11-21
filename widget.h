#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QFutureWatcher>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void task();

    void taskStarted();
    void taskCanceled();
    void taskFinished();
    void taskPaused();
    void taskResumed();
    void taskDestroyed();
    void taskProgressValueChanged(int value);
    void taskProgressRangeChanged(int minimum, int maximum);

    void on_pushButton_clicked();

private:
    Ui::Widget *ui;

    QFuture<int> future;
    QFutureWatcher<int> futureWatcher;
};
#endif // WIDGET_H
