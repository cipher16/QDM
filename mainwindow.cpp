#include "mainwindow.h"
#include <QMenu>
#include <QMenuBar>
#include <QTreeWidget>
#include <QItemDelegate>
#include <QApplication>
#include <QUrl>
#include <QDragMoveEvent>
#include <QToolBar>
#include <QHeaderView>
#include <adddownload.h>

#include <QDebug>
// DownloadView extends QTreeWidget to allow drag and drop.
class DownloadView : public QTreeWidget
{
    Q_OBJECT
public:
    DownloadView(QWidget *parent):QTreeWidget(parent)
    {
        setAcceptDrops(true);
    }
protected:
    void dragMoveEvent(QDragMoveEvent *event)
    {
        // Accept url startin with http.
        QUrl url(event->mimeData()->text());
        if (url.isValid() && url.scheme().toLower() == "http")
            event->acceptProposedAction();
    }
};

// DownloadViewDelegate is used to draw the progress bars.
class DownloadViewDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    inline DownloadViewDelegate(MainWindow *mainWindow) : QItemDelegate(mainWindow) {}

    inline void paint(QPainter *painter, const QStyleOptionViewItem &option,
                      const QModelIndex &index ) const
    {
        if (index.column() != 3) {
            QItemDelegate::paint(painter, option, index);
            return;
        }

        // Set up a QStyleOptionProgressBar to precisely mimic the
        // environment of a progress bar.
        QStyleOptionProgressBar progressBarOption;
        progressBarOption.state = QStyle::State_Enabled;
        progressBarOption.direction = QApplication::layoutDirection();
        progressBarOption.rect = option.rect;
        progressBarOption.fontMetrics = QApplication::fontMetrics();
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.textAlignment = Qt::AlignCenter;
        progressBarOption.textVisible = true;

        // Set the progress and text values of the style option.
        int progress = Download::downloadList.at(index.row())->getProgress();

        progressBarOption.progress = progress < 0 ? 0 : progress;
        progressBarOption.text = QString().sprintf("%d%%", progressBarOption.progress);

        // Draw the progress bar onto the view.
        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
    }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("QTDownload Manager");
    this->resize(600,200);

    // Initialize some static strings
    QStringList headers;
    headers << tr("Name") << tr("Status") << tr("Speed") << tr("Progress");

    // Main torrent list
    downloadView = new DownloadView(this);
    downloadView->setItemDelegate(new DownloadViewDelegate(this));
    downloadView->setHeaderLabels(headers);
    downloadView->setSelectionBehavior(QAbstractItemView::SelectRows);
    downloadView->setAlternatingRowColors(true);
    downloadView->setRootIsDecorated(false);
    setCentralWidget(downloadView);

    //header size
    QFontMetrics fm = fontMetrics();
    QHeaderView *header = downloadView->header();
    header->resizeSection(0, fm.width("typical-name-for-a-download.download"));
    header->resizeSection(1, fm.width("  Downloading  "));
    header->resizeSection(2, fm.width("  Speed  "));

    // Create common actions
    QAction *newDownloadAction = new QAction(QIcon(":/icons/bottom.png"), tr("Add &new download"), this);
    QAction *removeDownloadAction = new QAction(QIcon(":/icons/player_stop.png"), tr("&Remove download"), this);

    // File menu
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newDownloadAction);
    fileMenu->addAction(removeDownloadAction);
    fileMenu->addSeparator();
    fileMenu->addAction(QIcon(":/icons/exit.png"), tr("E&xit"), this, SLOT(close()));

    // Help menu
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("&About"), this, SLOT(about()));
    helpMenu->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));

    // Top toolbar
    QToolBar *topBar = new QToolBar(tr("Tools"));
    addToolBar(Qt::TopToolBarArea, topBar);
    topBar->setMovable(false);
    topBar->addAction(newDownloadAction);
    topBar->addAction(removeDownloadAction);
    topBar->addSeparator();
    //downActionTool = topBar->addAction(QIcon(tr(":/icons/1downarrow.png")), tr("Move down"));
    //upActionTool = topBar->addAction(QIcon(tr(":/icons/1uparrow.png")), tr("Move up"));

    connect(newDownloadAction,SIGNAL(triggered()),this,SLOT(addDownload()));
}

MainWindow::~MainWindow()
{
    delete downloadView;
}

void MainWindow::addDownload()
{
    AddDownload ad(this);
    if(ad.exec()==QDialog::Accepted)
    {
        Download *d=new Download(downloadView);
        d->setUrl(ad.getUrl());
        if(ad.getFilePath().size()!=0)
            d->setPath(ad.getFilePath());
        d->setFile(ad.getFileName());
        d->startDownload();
    }
}

#include "mainwindow.moc"
