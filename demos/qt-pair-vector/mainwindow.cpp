#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "pair.h"
#include "vector.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow) {
    ui_->setupUi(this);

    Vector<int> vecInt;
    vecInt.PushBack(1);
    vecInt.PushBack(2);
    vecInt.PushBack(3);

    // Создаем и заполняем второй вектор пар {int, double}
    Vector<pair<int, double>> vecPair;
    vecPair.PushBack(pair<int, double>(10, 10.1));
    vecPair.PushBack(pair<int, double>(20, 20.2));
    vecPair.PushBack(pair<int, double>(30, 30.3));

    // Формируем элемент: pair< Vector<int>, Vector< pair<int, double> > >
    pair<Vector<int>, Vector<pair<int, double>>> element(vecInt, vecPair);

    // Добавляем элемент в контейнер matrix
    matrix.PushBack(element);
}

MainWindow::~MainWindow() {
    delete ui_;
}

void MainWindow::updateTextEdit() {
    ui_->textEdit->clear();	 ////////

    for (size_t i = 0; i < vec_.Size(); ++i) {
        ui_->textEdit->append(QString::number(vec_[i]));
    }
}
void MainWindow::updateTextEditvec2() {
    ui_->textEdit->clear();	 ////////

    for (size_t i = 0; i < vec2_.Size(); ++i) {
        ui_->textEdit->append(QString::number(vec2_[i]));
    }
}

void MainWindow::on_pushButton_push_back_clicked() {
    int value = ui_->lineEdit_push_back->text().toInt(&ok_);


    if (!ok_) {
        QMessageBox::warning(this, "Error", "Enter a valid number");
        return;
    }
    vec_.PushBack(value);
    updateTextEdit();
}


void MainWindow::on_pushButton_pop_back_clicked() {
    vec_.PopBack();
    updateTextEdit();
}

void MainWindow::on_pushButton_clear_clicked() {
    vec_.Clear();
    updateTextEdit();
}
void MainWindow::on_pushButton_size_clicked() {
    int size = vec_.Size();
    ui_->textEdit->clear();
    ui_->textEdit->setText(QString::number(size));
}


void MainWindow::on_pushButton_capacity_clicked() {
    int cap = vec_.Capacity();
    ui_->textEdit->clear();
    ui_->textEdit->setText(QString::number(cap));
}

void MainWindow::on_pushButton_show_clicked() {
    updateTextEdit();
}

void MainWindow::on_pushButton_max_size_clicked() {
    int max_size = vec_.Max_Size();	 ///////////// не работаеты
    ui_->textEdit->clear();
    ui_->textEdit->setText(QString::number(max_size));
}

void MainWindow::on_pushButton_resize_clicked() {

    int size = ui_->lineEdit_resize_size->text().toInt(&ok_);
    int value = ui_->lineEdit_resize_value->text().toInt(&ok2_);


    if (!ok_ && !ok2_) {
        QMessageBox::warning(this, "Error", "Enter a valid number");
        return;
    }
    vec_.Resize(size, value);
    updateTextEdit();
}

void MainWindow::on_pushButton_begin_clicked() {
    auto it = vec_.Begin();
    ui_->textEdit->clear();
    ui_->textEdit->setText(QString::number(*it));
}


void MainWindow::on_pushButton_cbegin_clicked() {
    auto it = vec_.CBegin();
    ui_->textEdit->clear();
    ui_->textEdit->setText(QString::number(*it));
}


void MainWindow::on_pushButton_rbegin_clicked() {
    auto it = vec_.RBegin();
    ui_->textEdit->clear();
    ui_->textEdit->setText(QString::number(*it));
}


void MainWindow::on_pushButton_end_clicked() {
    Iterator<int> it = vec_.End();
    int last = *(it - 1);
    ui_->textEdit->clear();
    ui_->textEdit->setText(QString::number(last));
}


void MainWindow::on_pushButton_rend_clicked() {
    Iterator<int> it = vec_.REnd();
    ui_->textEdit->clear();
    ui_->textEdit->setText(QString::number(*(it + 1)));
}


void MainWindow::on_pushButton_front_clicked() {
    int value_front = vec_.Front();
    ui_->textEdit->clear();
    ui_->textEdit->setText(QString::number(value_front));
}


void MainWindow::on_pushButton_back_clicked() {
    int back_value = vec_.Back();
    ui_->textEdit->clear();
    ui_->textEdit->setText(QString::number(back_value));
}


void MainWindow::on_pushButton_data_clicked() {
    auto it = vec_.Data();
    ui_->textEdit->clear();
    ui_->textEdit->setText(QString::number(*it));
}


void MainWindow::on_pushButton_at_clicked() {
    int index = ui_->lineEdit_at_index->text().toInt(&ok_);


    if (!ok_) {
        QMessageBox::warning(this, "Error", "Enter a valid number");
        return;
    }
    int value = vec_.At(index);
    ui_->textEdit->setText(QString::number(value));
}

void MainWindow::on_pushButton_insert_clicked() {
    int position = ui_->lineEdit_insert_position->text().toInt(&ok_);
    int value = ui_->lineEdit_value->text().toInt(&ok2_);


    if (!ok_ && !ok2_) {
        QMessageBox::warning(this, "Error", "Enter a valid number");
        return;
    }
    vec_.Insert(position, value);
    updateTextEdit();
}

void MainWindow::on_pushButton_erase_clicked() {
    int position = ui_->lineEdit_erase_position->text().toInt(&ok_);
    int count = ui_->lineEdit_count->text().toInt(&ok2_);


    if (!ok_ && !ok2_) {
        QMessageBox::warning(this, "Error", "Enter a valid number");
        return;
    }
    vec_.Erase(position, count);
    updateTextEdit();
}


void MainWindow::on_pushButton_emplace_clicked() {

    int arg = ui_->lineEdit_emplace_args->text().toInt(&ok_);


    if (!ok_) {
        QMessageBox::warning(this, "Error", "Enter a valid number");
        return;
    }
    vec_.Emplace(vec_.Begin(), arg);
    updateTextEdit();
}

void MainWindow::on_pushButton_emplace_back_clicked() {
    int value = ui_->lineEdit->text().toInt(&ok_);


    if (!ok_) {
        QMessageBox::warning(this, "Error", "Enter a valid number");
        return;
    }
    vec_.EmplaceBack(value);
    updateTextEdit();
}

void MainWindow::on_pushButton_swap_clicked() {
    vec_.Swap(vec2_);
    ui_->textEdit->clear();
}


void MainWindow::on_pushButton_show_vec2_clicked() {
    updateTextEditvec2();
}


void MainWindow::on_pushButton_pushback_vec2_clicked() {
    int value = ui_->lineEdit_pushback_vec2->text().toInt(&ok_);


    if (!ok_) {
        QMessageBox::warning(this, "Error", "Enter a valid number");
        return;
    }
    vec2_.PushBack(value);
    updateTextEditvec2();
}

void MainWindow::showMatrices() {
    // Очищаем QTextEdit
    ui_->textEdit->clear();

    QString output;

    // Пробегаемся по контейнеру matrix,
    // где каждый элемент — это pair<Vector<int>, Vector<pair<int, double>>>
    for (size_t i = 0; i < matrix.Size(); ++i) {
        output += QString("Элемент %1:\n").arg(i + 1);

        // Первая матрица: выводим содержимое Vector<int>
        output += "Матрица 1 (Vector<int>): ";
        const Vector<int>& vecInt = matrix[i].first;
        for (size_t j = 0; j < vecInt.Size(); ++j) {
            output += QString::number(vecInt[j]) + " ";
        }
        output += "\n";

        // Вторая матрица: выводим содержимое Vector<pair<int, double>>
        output += "Матрица 2 (Vector<pair<int, double>>): ";
        const Vector<pair<int, double>>& vecPair = matrix[i].second;
        for (size_t j = 0; j < vecPair.Size(); ++j) {
            output += "{" + QString::number(vecPair[j].first) + ", " +
                      QString::number(vecPair[j].second) + "} ";
        }
        output += "\n\n";
    }

    // Выводим собранный текст в QTextEdit
    ui_->textEdit->setPlainText(output);
}

void MainWindow::on_pushButton_show_matrix_clicked() {}
