#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QDebug>
#include <QComboBox>
#include <QStringList>
#include <QString>
#include <QtSql>
#include <iostream>


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
    void save();
    void load();
    void addRow();
    void deleteRow();

private:
    Ui::MainWindow *ui;

    void editTable(QString data);
};
#endif // MAINWINDOW_H
