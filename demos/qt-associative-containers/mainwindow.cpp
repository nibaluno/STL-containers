#include "mainwindow.h"
#include <QMessageBox>
#include <QStringListModel>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui_(new Ui::MainWindow),
      map_model_(new QStringListModel(this)),
      set_model_(new QStringListModel(this)),
      unmap_model_(new QStringListModel(this)) {
    ui_->setupUi(this);

    // Apply beautiful pastel styling
    QString styleSheet = R"(
        /* Main window styling */
        QMainWindow {
             background-color: #e1e5eb;
            font-family: 'Segoe UI', 'Arial', sans-serif;
        }

        /* Pastel blue for map section */
        #pushButton_insert_map, #pushButton_erase_map,
        #pushButton_find_map, #pushButton_clear_map {
            background-color: #a8d8ea;
            color: #333333;
            border-radius: 8px;
            padding: 8px 16px;
            font-size: 14px;
            font-weight: 500;
            border: 1px solid #97c7d9;
        }

        #pushButton_insert_map:hover, #pushButton_erase_map:hover,
        #pushButton_find_map:hover, #pushButton_clear_map:hover {
            background-color: #97c7d9;
        }

        /* Pastel orange for set section */
        #pushButton_insert_set, #pushButton_erase_set,
        #pushButton_find_set, #pushButton_clear_set {
            background-color: #f8c3cd;
            color: #333333;
            border-radius: 8px;
            padding: 8px 16px;
            font-size: 14px;
            font-weight: 500;
            border: 1px solid #e7b2bc;
        }

        #pushButton_insert_set:hover, #pushButton_erase_set:hover,
        #pushButton_find_set:hover, #pushButton_clear_set:hover {
            background-color: #e7b2bc;
        }

        /* Pastel purple for unordered_map section */
        #pushButton_insert_unmap, #pushButton_erase_unmap,
        #pushButton_find_unmap, #pushButton_clear_unmap {
            background-color: #c7c7e2;
            color: #333333;
            border-radius: 8px;
            padding: 8px 16px;
            font-size: 14px;
            font-weight: 500;
            border: 1px solid #b6b6d1;
        }

        #pushButton_insert_unmap:hover, #pushButton_erase_unmap:hover,
        #pushButton_find_unmap:hover, #pushButton_clear_unmap:hover {
            background-color: #b6b6d1;
        }

        /* List views styling */
        QListView {
            background-color: #ffffff;
            border: 1px solid #e0e0e0;
            border-radius: 8px;
            padding: 6px;
            font-size: 15px;
            color: #333333 ;
            selection-background-color: #e0e0e0;
        }

        /* Line edits styling */
        QLineEdit {
            background-color: #ffffff;
            border: 1px solid #d0d0d0;
            border-radius: 6px;
            padding: 6px;
            font-size: 14px;
            color: #444444;
        }

        /* Labels styling */
        QLabel {
            font-size: 14px;
            color: #666666;
            font-weight: 500;
        }

        /* Section titles */
        #SizeLabel, #label_4, #label_5 {
            font-size: 16px;
            font-weight: 600;
            color: #555555;
            margin-bottom: 8px;
        }

        /* Size labels */
        #mapSizeLabel, #setSizeLabel, #unmapSizeLabel {
            font-size: 13px;
            font-style: italic;
            color: #777777;
        }
    )";

    this->setStyleSheet(styleSheet);

    // Initialize models
    ui_->mapListView->setModel(map_model_);
    ui_->setListView->setModel(set_model_);
    ui_->unmapListView->setModel(unmap_model_);
}


MainWindow::~MainWindow() {
    delete ui_;
}

// Map Operations
void MainWindow::on_pushButton_insert_map_clicked() {
    bool ok;
    int key = ui_->lineEdit_key->text().toInt(&ok);


    if (!ok) {
        QMessageBox::warning(this, "Input Error", "Invalid key value");
        return;
    }

    QString value = ui_->lineEdit_value->text();
    map_[key] = value;
    updateDisplay();
}

void MainWindow::on_pushButton_erase_map_clicked() {
    bool ok;
    int key = ui_->lineEdit_key->text().toInt(&ok);


    if (!ok) {
        QMessageBox::warning(this, "Input Error", "Invalid key value");
        return;
    }

    map_.erase(key);
    updateDisplay();
}

void MainWindow::on_pushButton_find_map_clicked() {
    bool ok;
    int key = ui_->lineEdit_key->text().toInt(&ok);


    if (!ok) {
        QMessageBox::warning(this, "Input Error", "Invalid key value");
        return;
    }

    auto it = map_.find(key);
    if (it != map_.end()) {
        QMessageBox::information(
            this, "Map Find",
            QString("Found: Key=%1, Value=%2").arg(key).arg(it->second));
    } else {
        QMessageBox::information(this, "Map Find", "Key not found");
    }
}

void MainWindow::on_pushButton_clear_map_clicked() {
    map_.clear();
    updateDisplay();
}

// Set Operations
void MainWindow::on_pushButton_insert_set_clicked() {
    bool ok;
    int value = ui_->lineEdit_key->text().toInt(&ok);


    if (!ok) {
        QMessageBox::warning(this, "Input Error", "Invalid value");
        return;
    }

    set_.insert(value);
    updateDisplay();
}

void MainWindow::on_pushButton_erase_set_clicked() {
    bool ok;
    int value = ui_->lineEdit_key->text().toInt(&ok);


    if (!ok) {
        QMessageBox::warning(this, "Input Error", "Invalid value");
        return;
    }

    auto it = set_.find(value);
    if (it != set_.end()) {
        set_.erase(it);
    }
    updateDisplay();
}

void MainWindow::on_pushButton_find_set_clicked() {
    bool ok;
    int value = ui_->lineEdit_key->text().toInt(&ok);


    if (!ok) {
        QMessageBox::warning(this, "Input Error", "Invalid value");
        return;
    }

    bool found = set_.contains(value);
    QMessageBox::information(
        this, "Set Find",
        found ? "Value found in set" : "Value not found in set");
}

void MainWindow::on_pushButton_clear_set_clicked() {
    set_.clear();
    updateDisplay();
}

// Unordered Map Operations
void MainWindow::on_pushButton_insert_unmap_clicked() {
    bool ok;
    int key = ui_->lineEdit_key->text().toInt(&ok);


    if (!ok) {
        QMessageBox::warning(this, "Input Error", "Invalid key value");
        return;
    }

    QString value = ui_->lineEdit_value->text();
    unordered_map_[key] = value;
    updateDisplay();
}

void MainWindow::on_pushButton_erase_unmap_clicked() {
    bool ok;
    int key = ui_->lineEdit_key->text().toInt(&ok);


    if (!ok) {
        QMessageBox::warning(this, "Input Error", "Invalid key value");
        return;
    }

    unordered_map_.erase(key);
    updateDisplay();
}

void MainWindow::on_pushButton_find_unmap_clicked() {
    bool ok;
    int key = ui_->lineEdit_key->text().toInt(&ok);


    if (!ok) {
        QMessageBox::warning(this, "Input Error", "Invalid key value");
        return;
    }

    auto it = unordered_map_.find(key);
    if (it != unordered_map_.end()) {
        QMessageBox::information(
            this, "Unordered Map Find",
            QString("Found: Key=%1, Value=%2").arg(key).arg(it->second));
    } else {
        QMessageBox::information(this, "Unordered Map Find", "Key not found");
    }
}

void MainWindow::on_pushButton_clear_unmap_clicked() {
    unordered_map_.clear();
    updateDisplay();
}

// Update all displays
void MainWindow::updateDisplay() {
    updateMapDisplay();
    updateSetDisplay();
    updateUnorderedMapDisplay();
}

void MainWindow::updateMapDisplay() {
    QStringList items;


    for (const auto& pair : map_) {
        items << QString("Key: %1, Value: %2").arg(pair.first).arg(pair.second);
    }
    map_model_->setStringList(items);
    ui_->mapSizeLabel->setText(QString("Size: %1").arg(map_.size()));
}

void MainWindow::updateSetDisplay() {
    QStringList items;
    // Use const_iterator explicitly
    for (auto it = set_.begin(); it != set_.end(); ++it) {
        items << QString("Value: %1").arg(*it);
    }
    set_model_->setStringList(items);
    ui_->setSizeLabel->setText(QString("Size: %1").arg(set_.size()));
}

void MainWindow::updateUnorderedMapDisplay() {
    QStringList items;


    for (const auto& pair : unordered_map_) {
        items << QString("Key: %1, Value: %2").arg(pair.first).arg(pair.second);
    }
    unmap_model_->setStringList(items);
    ui_->unmapSizeLabel->setText(
        QString("Size: %1").arg(unordered_map_.size()));
}
