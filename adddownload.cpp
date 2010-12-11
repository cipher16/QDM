#include "adddownload.h"
#include "ui_adddownload.h"
#include <QDebug>
AddDownload::AddDownload(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDownload)
{
    ui->setupUi(this);
    setWindowTitle(tr("Adding a Download Url"));
    setModal(true);
    ui->errorMessage->setText("");//reset error message
}

AddDownload::~AddDownload()
{
    delete ui;
}

QString AddDownload::getUrl()
{
    return ui->txtUrl->text();
}

QString AddDownload::getFileName()
{
    return ui->txtFileName->text();
}

QString AddDownload::getFilePath()
{
    return ui->txtFilePath->text();
}

void AddDownload::accept()
{
    if(ui->txtUrl->text().size()>0&&ui->txtUrl->text().startsWith("http"))
        done(QDialog::Accepted);
    else
        ui->errorMessage->setText(tr("Url field is mandatory, and must begin with 'http'"));
}
