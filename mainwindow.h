#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "download.h"
#include <QtGui/QMainWindow>
class DownloadView;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    DownloadView *downloadView;
private slots:
    void addDownload();
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
