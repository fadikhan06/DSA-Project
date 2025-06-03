#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QTimer>
#include <QMessageBox>
#include <QQueue>
#include <QtMath>
#include <QGraphicsDropShadowEffect>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set layout for stackVisualArea if not already done
    QVBoxLayout *layout = new QVBoxLayout(ui->stackVisualArea);
    ui->stackVisualArea->setLayout(layout);  // Ensure a layout is set
    QVBoxLayout *layout1 = new QVBoxLayout(ui->linearqueuearea);
    ui->stackVisualArea->setLayout(layout);
    QVBoxLayout *layout2 = new QVBoxLayout(ui->circularqueuearea);
    ui->stackVisualArea->setLayout(layout);
     ui->stackVisualArea->setLayout(layout1);
      ui->stackVisualArea->setLayout(layout2);

     QList<QPushButton*> buttons = {
         ui->push, ui->pop, ui->clear,ui->search // add all your buttons here
     };
     for (QPushButton* btn : buttons) {
         btn->installEventFilter(this);
     }

    // Initialize comboBox1 with options
    ui->comboBox1->addItems({"Stack", "Queue", "Linked List"});

    // Connect comboBox1 signal to the respective slot
    connect(ui->comboBox1, &QComboBox::currentTextChanged, this, &MainWindow::on_comboBox1_currentTextChanged);

    // Initialize comboBox2 options based on comboBox1 selection
    on_comboBox1_currentTextChanged(ui->comboBox1->currentText());
    connect(ui->lineEditValue, &QLineEdit::textChanged, this, &MainWindow::onValueTextChanged);

    commonStyle = R"(
        QLabel {
            color: white;
            font-weight: bold;
            font-size: 16px;
            border-radius: 10px;
            border: 2px solid #16a085;
            background: qlineargradient(
                x1:0, y1:0, x2:0, y2:1,
                stop:0 #1abc9c,
                stop:1 #16a085
            );
            padding: 8px;
        }
        QLabel:hover {
            background: qlineargradient(
                x1:0, y1:0, x2:0, y2:1,
                stop:0 #48c9b0,
                stop:1 #1abc9c
            );
            border-color: #1abc9c;
            color: #f0f0f0;
        }
    )";


    }

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fadeOutWidget(QWidget *widget) {
    if (!widget->isVisible()) return;

    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(widget);
    widget->setGraphicsEffect(effect);

    QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(500);  // 0.5 second fade out
    animation->setStartValue(1.0);
    animation->setEndValue(0.0);

    // Hide the widget after animation ends
    connect(animation, &QPropertyAnimation::finished, widget, [widget]() {
        widget->hide();
        widget->setGraphicsEffect(nullptr); // optional cleanup
    });

    animation->start(QAbstractAnimation::DeleteWhenStopped);
}


void MainWindow::fadeInWidget(QWidget *widget)
{
    // Set up opacity effect
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(widget);
    widget->setGraphicsEffect(effect);

    // Create animation
    QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(500);  // 500ms fade-in
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    widget->show();
}

void MainWindow::on_comboBox1_currentTextChanged(const QString &selectedText) {
    ui->comboBox2->clear();

    // Hide all push buttons by default
    ui->push->hide();
    ui->pop->hide();
    ui->search->hide();
    ui->front->hide();
    ui->frontdel->hide();
    ui->clear->hide();

    // Add items and unhide the necessary buttons based on selected combo box option
    if (selectedText == "Stack") {
        ui->comboBox2->addItem("No items to be selected");
        fadeInWidget(ui->push);
        fadeInWidget(ui->pop);    // Ensure pop button is shown
        fadeInWidget(ui->search);
        fadeInWidget(ui->clear);
    } else if (selectedText == "Queue") {
        ui->comboBox2->addItems({"linear", "circular"});
        fadeInWidget(ui->push);
        fadeInWidget(ui->pop);
        fadeInWidget(ui->search);
        fadeInWidget(ui->clear);
    } else if (selectedText == "Linked List") {
        ui->comboBox2->addItems({"linear", "doubly", "circular"});
        fadeInWidget(ui->push);
        fadeInWidget(ui->pop);
        fadeInWidget(ui->search);
        fadeInWidget(ui->front);
        fadeInWidget(ui->frontdel);
        fadeInWidget(ui->clear);
    }
}
void MainWindow::onValueTextChanged(const QString &text) {
    if (!text.trimmed().isEmpty()) {
        // Hide pop and frontdel buttons if the text field is not empty
        fadeOutWidget(ui->pop);
        fadeOutWidget(ui->frontdel);
        fadeOutWidget(ui->clear);

    } else {
        // Unhide the pop button again when input is cleared
        fadeInWidget(ui->pop);
        fadeInWidget(ui->clear);
    }
}

void MainWindow::pushToStackVisual(const QString &value) {
    QLabel *box = new QLabel(value);
    box->setFixedSize(120, 50);  // Slightly bigger for better visuals
    box->setAlignment(Qt::AlignCenter);

    // Use a gradient background with subtle color blending
    QString style = R"(
        QLabel {
            color: white;
            font-weight: bold;
            font-size: 16px;
            border-radius: 10px;
            border: 2px solid #16a085;
            background: qlineargradient(
                x1:0, y1:0, x2:0, y2:1,
                stop:0 #1abc9c,
                stop:1 #16a085
            );
            padding: 8px;
        }
        QLabel:hover {
            background: qlineargradient(
                x1:0, y1:0, x2:0, y2:1,
                stop:0 #48c9b0,
                stop:1 #1abc9c
            );
            border-color: #1abc9c;
            color: #f0f0f0;
        }
    )";
    box->setStyleSheet(style);

    // Add subtle drop shadow effect for 3D feel
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(box);
    shadow->setBlurRadius(15);
    shadow->setOffset(3, 3);
    shadow->setColor(QColor(0, 0, 0, 120));  // Slightly transparent black shadow
    box->setGraphicsEffect(shadow);

    // Add to the stack layout (to make it visible on the UI)
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->stackVisualArea->layout());
    if (layout) {
        layout->insertWidget(0, box);  // Insert at the top
    }

    // Store the widget reference in stackWidgets for future searches
    stackWidgets.push_front(box);
    stackValues.push_front(value);

    fadeInWidget(box);  // Fade in animation (assuming implemented)
}

void MainWindow::popFromStackVisual() {

    if (!stackValues.isEmpty() && !stackWidgets.isEmpty()) {
        // Remove the value from stackValues
        stackValues.pop_front();

        // Remove the widget from stackWidgets
        QLabel *box = stackWidgets.takeFirst();  // Take the first widget (top of the stack)

        // Get the layout of the stack visual area
        QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->stackVisualArea->layout());

        if (layout) {
            // Remove widget from layout
            layout->removeWidget(box);



            fadeOutAndDeleteWidget(box);
        }
    } else {

        QMessageBox::information(this, "pop", "stack is empty");
    }
}
void MainWindow::fadeOutAndDeleteWidget(QWidget *widget) {
    auto *effect = new QGraphicsOpacityEffect(widget);
    widget->setGraphicsEffect(effect);

    QPropertyAnimation *anim = new QPropertyAnimation(effect, "opacity");
    anim->setDuration(500);  // 500ms fade-out duration
    anim->setStartValue(1.0);
    anim->setEndValue(0.0);

    connect(anim, &QPropertyAnimation::finished, widget, [widget]() {
        widget->hide();
        widget->deleteLater();  // After animation, delete the widget
    });

    anim->start(QAbstractAnimation::DeleteWhenStopped);
}



void MainWindow::searchInStackVisual(const QString &value) {
    // Check if the value exists in stack (search the stackValues list)
    int index = stackValues.indexOf(value);

    if (index != -1) {

        QLabel *box = stackWidgets.at(index);


        box->setStyleSheet("background-color: #e74c3c; color: white; border: 2px solid white; border-radius: 5px;");


        QTimer::singleShot(1000, this, [box, this]() {
            box->setStyleSheet("background-color: #1abc9c; color: white; border: 2px solid white; border-radius: 5px;");
        });
    } else {

        QMessageBox::information(this, "Search", "Value not found in the stack.");
    }
}





// // linear queue

void MainWindow::pushToLinearQueue(const QString &value) {
    QLabel *box = new QLabel(value);
    box->setFixedSize(120, 50);
    box->setAlignment(Qt::AlignCenter);
    box->setStyleSheet(commonStyle);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(box);
    shadow->setBlurRadius(15);
    shadow->setOffset(3, 3);
    shadow->setColor(QColor(0, 0, 0, 120));
    box->setGraphicsEffect(shadow);

    QVBoxLayout *layout1 = qobject_cast<QVBoxLayout*>(ui->linearqueuearea->layout());
    if (layout1) {
        layout1->insertWidget(1, box);
    }

    linearQueueValues.enqueue(value);
    linearQueueWidgets.enqueue(box);

    fadeInWidget(box);
}
void MainWindow::popFromLinearQueue() {
    if (!linearQueueValues.isEmpty() && !linearQueueWidgets.isEmpty()) {
        linearQueueValues.dequeue();

        QLabel *box = linearQueueWidgets.takeFirst();  // Remove front
        QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->stackVisualArea->layout());

        if (layout) layout->removeWidget(box);
        fadeOutAndDeleteWidget(box);
    } else {
        QMessageBox::information(this, "Queue", "Queue is empty.");
    }
}
void MainWindow::searchInLinearQueue(const QString &value) {
    int index = linearQueueValues.indexOf(value);
    if (index != -1) {
        QLabel *box = linearQueueWidgets.at(index);
        box->setStyleSheet("background-color: #e67e22; color: white; border: 2px solid white; border-radius: 5px;");

        QTimer::singleShot(1000, this, [box]() {
            box->setStyleSheet("background-color: #3498db; color: white; border: 2px solid white; border-radius: 5px;");
        });
    } else {
        QMessageBox::information(this, "Search", "Value not found in the queue.");
    }
}
void MainWindow::clearLinearQueue() {
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->stackVisualArea->layout());

    for (QLabel *box : linearQueueWidgets) {
        if (layout) layout->removeWidget(box);
        fadeOutAndDeleteWidget(box);
    }

    linearQueueWidgets.clear();
    linearQueueValues.clear();
}
void MainWindow::on_push_clicked()
{
    QString value = ui->lineEditValue->text().trimmed();

    if (value.isEmpty()) return;

    QString ds = ui->comboBox1->currentText();
    QString type = ui->comboBox2->currentText();

    if (ds == "Stack") {
        pushToStackVisual(value);
    } else if (ds == "Queue" && type == "linear") {
        pushToLinearQueue(value);
    }
    else if (ds == "Queue" && type == "circular") {
        pushToCircularQueue(value);
    }

    ui->lineEditValue->clear();
}
void MainWindow::on_pop_clicked()
{
    QString ds = ui->comboBox1->currentText();
    QString type = ui->comboBox2->currentText();

    if (ds == "Stack") {
        popFromStackVisual();
    } else if (ds == "Queue" && type == "linear") {
        popFromLinearQueue();
    }
    else if (ds == "Queue" && type == "circular") {
        popFromCircularQueue();}
}
void MainWindow::on_search_clicked()
{
    QString value = ui->lineEditValue->text().trimmed();
    if (value.isEmpty()) return;

    QString ds = ui->comboBox1->currentText();
    QString type = ui->comboBox2->currentText();

    if (ds == "Stack") {
        searchInStackVisual(value);
    } else if (ds == "Queue" && type == "linear") {
        searchInLinearQueue(value);
    }
    else if (ds == "Queue" && type == "circular") {
        searchInCircularQueue(value);}
}
void MainWindow::on_clear_clicked()
{
    QString ds = ui->comboBox1->currentText();
    QString type = ui->comboBox2->currentText();

    if (ds == "Stack") {
        QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->stackVisualArea->layout());
        for (QLabel *box : stackWidgets) {
            if (layout) layout->removeWidget(box);
            fadeOutAndDeleteWidget(box);
        }
        stackWidgets.clear();
        stackValues.clear();
    } else if (ds == "Queue" && type == "linear") {
        clearLinearQueue();
    }
    else if (ds == "Queue" && type == "circular") {
        clearCircularQueue();
    }
}



int count =0;
void MainWindow::pushToCircularQueue(const QString &value)
{
    if (positions.isEmpty()) {
        int cx = 1200, cy = 200, radius = 120;
        for (int i = 0; i < QUEUE_SIZE; ++i) {
            double angle = 2 * M_PI * i / QUEUE_SIZE;
            int x = cx + radius * qCos(angle) - 120 / 2;  // width 120
            int y = cy + radius * qSin(angle) - 50 / 2;   // height 50
            positions.append(QPoint(x, y));
            boxes.append(nullptr);
        }
    }

    if (boxes[currentIndex]) {
        delete boxes[currentIndex];
        boxes[currentIndex] = nullptr;
    }

    QLabel *label = new QLabel(value, this);
    label->setFixedSize(120, 50);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet(commonStyle);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(label);
    shadow->setBlurRadius(15);
    shadow->setOffset(3, 3);
    shadow->setColor(QColor(0, 0, 0, 120));
    label->setGraphicsEffect(shadow);

    label->move(positions[currentIndex]);
    label->show();

    fadeInWidget(label);

    boxes[currentIndex] = label;

    if (linearQueueValues.size() < QUEUE_SIZE) {
        linearQueueValues.insert(currentIndex, value);
        currentIndex = (currentIndex + 1) % QUEUE_SIZE;
        count++;
    } else {
        linearQueueValues[currentIndex] = value;
        currentIndex = (currentIndex + 1) % QUEUE_SIZE;
        count++;
    }
}

void MainWindow::popFromCircularQueue()
{
        QLabel *label = boxes[front];
    if (count == 0) {
        QMessageBox::information(this, "Queue", "Circular queue is empty.");
        return;
    }
    if (label) {
        fadeOutAndDeleteWidget(label); // or just: delete label;
        boxes[front] = nullptr;
        count--;
    }


    QString removedValue =linearQueueValues[front];

   linearQueueValues[front] = "";

    front = (front + 1) % QUEUE_SIZE;



}

void MainWindow::searchInCircularQueue(const QString &value)
{
    int idx = front;
    for (int i = 0; i < count; ++i) {
        if (linearQueueValues[idx] == value) {
            QLabel *label = boxes[idx];
            if (label) {
                label->setStyleSheet("background-color: #e67e22; color: white; border: 2px solid white; border-radius: 5px;");
                QTimer::singleShot(1000, this, [label]() {
                    label->setStyleSheet("background-color: #3498db; color: white; border: 2px solid white; border-radius: 5px;");
                });
            }
            return;
        }
        idx = (idx + 1) % QUEUE_SIZE;
    }
    QMessageBox::information(this, "Search", "Value not found in the queue.");
}

void MainWindow::clearCircularQueue() {
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->circularqueuearea->layout());

    for (QLabel *box : boxes) {
        if (box) {
            if (layout) layout->removeWidget(box);
            fadeOutAndDeleteWidget(box);
        }
    }

    boxes.fill(nullptr);
    linearQueueValues.clear();
    currentIndex = 0;
    front = 0;
}


bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    QPushButton* hoveredBtn = qobject_cast<QPushButton*>(watched);
    if (hoveredBtn) {
        if (event->type() == QEvent::Enter) {
            auto* effect = new QGraphicsDropShadowEffect(this);
            effect->setBlurRadius(15);
            effect->setColor(QColor("#1abc9c"));  // Teal glow
            effect->setOffset(0, 0);
            hoveredBtn->setGraphicsEffect(effect);
        } else if (event->type() == QEvent::Leave) {
            hoveredBtn->setGraphicsEffect(nullptr); // Remove the effect
        }
    }
    return QMainWindow::eventFilter(watched, event);
}
