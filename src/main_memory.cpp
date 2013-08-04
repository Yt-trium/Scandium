#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_listProcessButton_clicked()
{
    ui->processList->clear();

    QString tmp;
    int i = 0;

    entry.dwSize = sizeof(PROCESSENTRY32);
    snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    hProcess = NULL;

    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            tmp.clear();
            tmp = QString::fromWCharArray(entry.szExeFile, 260);
            ui->processList->addItem(tmp);
            entryStock[i] = entry;
            i++;
        }
    }
    CloseHandle(snapshot);
}


void MainWindow::on_selectProcessButton_clicked()
{
    QString tmp;

    entry = entryStock[ui->processList->currentRow()];

    tmp = QString::fromWCharArray(entry.szExeFile, 260);
    hProcess = NULL;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
    if(hProcess == NULL)
        QMessageBox::information(this, "Error", "Error");
    else
    {
        ui->processList->clear();
        ui->processList->addItem(tmp);
        QMessageBox::information(this, "Process Selected", "Process Selected : " + tmp);
    }

    // CloseHandle(hProcess);
}

void MainWindow::on_newScanButton_clicked()
{
    ui->resultTable->clear();

    int from    =   0x00000000;
    int to      =   0x00000000;

    from = ui->fromEdit->text().toInt(0, 16);
    to = ui->toEdit->text().toInt(0, 16);

    int addr;
    int buf;
    int target = ui->valueSpin->value();

    QString tmp;

    for(addr = from; addr < to; addr+=4)
    {
        ReadProcessMemory (hProcess, (LPVOID)addr, &buf,4,NULL);
        if(buf == target)
        {
            tmp = QString("%1").arg(addr, 0, 16);
            ui->resultTable->addItem(tmp);
        }
    }
}

void MainWindow::on_nextScanButton_clicked()
{
    QString tmp;
    bool ok;
    int i;
    int size = ui->resultTable->count();
    int addr;
    int buf;
    int target = ui->valueSpin->value();

    for(i=0;i<size;i++)
    {
        addr = ui->resultTable->takeItem(0)->text().toInt(&ok, 16);

        ReadProcessMemory (hProcess, (LPVOID)addr, &buf,4,NULL);

        if(buf == target)
        {
            tmp = QString("%1").arg(addr, 0, 16);
            ui->resultTable->addItem(tmp);
        }
    }
}


void MainWindow::on_editValueButton_clicked()
{
    bool ok;
    int addr;
    int buf;

    addr = ui->resultTable->selectedItems().takeFirst()->text().toInt(&ok, 16);
    ReadProcessMemory (hProcess, (LPVOID)addr, &buf,4,NULL);

    //int value = QInputDialog::getInt(this, "Set Value","Set Value :", buf, 0, 100, 1, &ok);
    int value = QInputDialog::getInt(this, "Set Value","Set Value :", buf);

    WriteProcessMemory(hProcess, (LPVOID)addr, (LPVOID) &value, sizeof(&value), NULL);
}

void MainWindow::on_saveAddrButton_clicked()
{
    ui->saveTable->addItem(ui->resultTable->currentItem()->text());
}
