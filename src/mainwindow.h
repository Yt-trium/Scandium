#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QInputDialog>

// SPEC INCLUDE
#include <windows.h>
#include <tlhelp32.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    HWND hWnd;
    DWORD dwID;
    HANDLE hProcess;
    PROCESSENTRY32 entry;
    PROCESSENTRY32 entryStock[1000];
    HANDLE snapshot;

private slots:
    void on_newScanButton_clicked();

    void on_nextScanButton_clicked();

    void on_listProcessButton_clicked();

    void on_selectProcessButton_clicked();

    void on_saveAddrButton_clicked();

    void on_editValueButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
