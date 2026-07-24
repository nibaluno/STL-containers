#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QString>
#include <QStringListModel>

#include "map.h"
#include "set.h"
#include "unordered_map.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

   private slots:
    // Map operations
    void on_pushButton_insert_map_clicked();
    void on_pushButton_erase_map_clicked();
    void on_pushButton_find_map_clicked();
    void on_pushButton_clear_map_clicked();

    // Set operations
    void on_pushButton_insert_set_clicked();
    void on_pushButton_erase_set_clicked();
    void on_pushButton_find_set_clicked();
    void on_pushButton_clear_set_clicked();

    // Unordered map operations
    void on_pushButton_insert_unmap_clicked();
    void on_pushButton_erase_unmap_clicked();
    void on_pushButton_find_unmap_clicked();
    void on_pushButton_clear_unmap_clicked();

   private:
    // Display update methods
    void updateDisplay();
    void updateMapDisplay();
    void updateSetDisplay();
    void updateUnorderedMapDisplay();

    // Helper methods
    bool validateKeyInput(bool* ok);
    bool validateValueInput();

    // UI components
    Ui::MainWindow* ui_;

    // Data containers
    Map<int, QString> map_;
    Set<int> set_;
    unordered_map<int, QString> unordered_map_;

    // Display models
    QStringListModel* map_model_;
    QStringListModel* set_model_;
    QStringListModel* unmap_model_;
};
#endif	// MAINWINDOW_H
