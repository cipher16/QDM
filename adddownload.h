#ifndef ADDDOWNLOAD_H
#define ADDDOWNLOAD_H

#include <QDialog>

namespace Ui {
    class AddDownload;
}

class AddDownload : public QDialog
{
    Q_OBJECT

public:
    explicit AddDownload(QWidget *parent = 0);
    ~AddDownload();
    QString getUrl();
    QString getFileName();
    QString getFilePath();
public slots:
    void accept();

private:
    Ui::AddDownload *ui;
};

#endif // ADDDOWNLOAD_H
