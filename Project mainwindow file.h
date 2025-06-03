#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QStack>
#include <QLabel>
#include<QQueue>
#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_comboBox1_currentTextChanged(const QString &selectedText);
    void onValueTextChanged(const QString &text);
    void on_push_clicked();
    void on_pop_clicked();
    void on_search_clicked();
    void on_clear_clicked();

private:
    Ui::MainWindow *ui;
 QString commonStyle;
    void fadeInWidget(QWidget *widget);
    void fadeOutWidget(QWidget *widget);
     QStack<QString> stackValues;  // Use QStack for stack values
     QList<QLabel*> stackWidgets;
     QQueue<QString> linearQueueValues;
     QQueue<QLabel*> linearQueueWidgets;    // Store widgets representing stack items


    void pushToStackVisual(const QString &value);
     void popFromStackVisual();
     void fadeOutAndDeleteWidget(QWidget *widget);
    void searchInStackVisual(const QString &value);
    void pushToLinearQueue(const QString &value);
     void popFromLinearQueue();
    void searchInLinearQueue(const QString &value);
     void clearLinearQueue();
    QVector<QPoint> positions;
    QVector<QLabel*> boxes;       // Holds up to 5 QLabel pointers
    int currentIndex = 0;         // Points to the next insertion slot
    const int QUEUE_SIZE = 5;
    int front=0;
    void pushToCircularQueue(const QString &value);
   void popFromCircularQueue();

 void clearCircularQueue();
    void searchInCircularQueue(const QString &value);
 protected:
     bool eventFilter(QObject *watched, QEvent *event) override;


};

#endif // MAINWINDOW_H
