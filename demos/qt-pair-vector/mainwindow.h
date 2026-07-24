#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QString>
#include <utility>
#include "pair.h"
#include "vector.h"

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
    void on_pushButton_push_back_clicked();
    void on_pushButton_pop_back_clicked();
    void on_pushButton_size_clicked();
    void on_pushButton_capacity_clicked();
    void on_pushButton_clear_clicked();
    void on_pushButton_show_clicked();
    void on_pushButton_max_size_clicked();
    void on_pushButton_resize_clicked();
    void on_pushButton_begin_clicked();
    void on_pushButton_cbegin_clicked();
    void on_pushButton_rbegin_clicked();
    void on_pushButton_end_clicked();
    void on_pushButton_rend_clicked();
    void on_pushButton_front_clicked();
    void on_pushButton_back_clicked();
    void on_pushButton_data_clicked();
    void on_pushButton_at_clicked();
    void on_pushButton_insert_clicked();
    void on_pushButton_erase_clicked();
    void on_pushButton_emplace_clicked();
    void on_pushButton_emplace_back_clicked();
    void on_pushButton_swap_clicked();
    void on_pushButton_show_vec2_clicked();
    void on_pushButton_pushback_vec2_clicked();
    void on_pushButton_show_matrix_clicked();


   private:
    void updateTextEdit();
    void updateTextEditvec2();
    void initializeMatrixData();
    Ui::MainWindow* ui_;
    Vector<int> vec_;
    Vector<int> vec2_;
    bool ok_, ok2_;
    Vector<pair<Vector<int>, Vector<pair<int, double>>>> matrix;
    void showMatrices();
};
#endif	// MAINWINDOW_H
