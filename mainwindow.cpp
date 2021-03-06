#include "mainwindow.h"
#include "spreadsheet.h"

#include <QIcon>
#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QApplication>
#include <QLabel>
#include <QStatusBar>
#include <QSettings>
#include <QMessageBox>
#include <QCloseEvent>
#include <QFileDialog>

static QString strippedName(const QString &fullFileName);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , spreadsheet(new Spreadsheet())
{
    //设置主窗体
    setCentralWidget(spreadsheet);
    setWindowIcon(QIcon(":/image/icon.png"));
    setWindowTitle(tr("Spread Sheet"));

    //先创建动作
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

}

void MainWindow::updateStatusBar()
{

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(okToContinue()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

//void MainWindow::closeEvent(QCloseEvent *event)
//{

//}

void MainWindow::newFile()
{
    if(okToContinue()) {
        spreadsheet->clear();
        setCurrentFile("");
    }
}

bool MainWindow::save()
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

void MainWindow::open()
{
    if (okToContinue()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Open Spreadsheet"), ".",
                                   tr("Spreadsheet files (*.sp)"));
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool MainWindow::saveAs()
{
    return true;
}

void MainWindow::createActions()
{
    newAction = new QAction(tr("&New"), this);
    newAction->setIcon(QIcon(":/image/new.png"));
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(tr("Create a new spreadsheet file"));
    connect(newAction, &QAction::triggered, this, &MainWindow::newFile);
    // 重复的界面功能组件
    {
        openAction = new QAction(tr("&Open..."), this);
        openAction->setIcon(QIcon(":/image/open.png"));
        openAction->setShortcut(QKeySequence::Open);
        openAction->setStatusTip(tr("Open an existing spreadsheet file"));
        connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

        saveAction = new QAction(tr("&Save"), this);
        saveAction->setIcon(QIcon(":/image/save.png"));
        saveAction->setShortcut(QKeySequence::Save);
        saveAction->setStatusTip(tr("Save the spreadsheet to disk"));
        connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

        saveAsAction = new QAction(tr("Save &As..."), this);
        saveAsAction->setIcon(QIcon(":/image/save.png"));
        saveAsAction->setStatusTip(tr("Save the spreadsheet under a new "
                                      "name"));
        connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));
    }

    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }
    // 重复的界面功能组件
    {
        exitAction = new QAction(tr("E&xit"), this);
        exitAction->setShortcut(tr("Ctrl+Q"));
        exitAction->setStatusTip(tr("Exit the application"));
        connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

        cutAction = new QAction(tr("Cu&t"), this);
        cutAction->setIcon(QIcon(":/image/cut.png"));
        cutAction->setShortcut(QKeySequence::Cut);
        cutAction->setStatusTip(tr("Cut the current selection's contents "
                                   "to the clipboard"));
        connect(cutAction, SIGNAL(triggered()), spreadsheet, SLOT(cut()));

        copyAction = new QAction(tr("&Copy"), this);
        copyAction->setIcon(QIcon(":/image/copy.png"));
        copyAction->setShortcut(QKeySequence::Copy);
        copyAction->setStatusTip(tr("Copy the current selection's contents "
                                    "to the clipboard"));
        connect(copyAction, SIGNAL(triggered()), spreadsheet, SLOT(copy()));

        pasteAction = new QAction(tr("&Paste"), this);
        pasteAction->setIcon(QIcon(":/image/paste.png"));
        pasteAction->setShortcut(QKeySequence::Paste);
        pasteAction->setStatusTip(tr("Paste the clipboard's contents into "
                                     "the current selection"));
        connect(pasteAction, SIGNAL(triggered()),
                spreadsheet, SLOT(paste()));

        deleteAction = new QAction(tr("&Delete"), this);
        deleteAction->setShortcut(QKeySequence::Delete);
        deleteAction->setStatusTip(tr("Delete the current selection's "
                                      "contents"));
        connect(deleteAction, SIGNAL(triggered()),
                spreadsheet, SLOT(del()));

        selectRowAction = new QAction(tr("&Row"), this);
        selectRowAction->setStatusTip(tr("Select all the cells in the "
                                         "current row"));
        connect(selectRowAction, SIGNAL(triggered()),
                spreadsheet, SLOT(selectCurrentRow()));

        selectColumnAction = new QAction(tr("&Column"), this);
        selectColumnAction->setStatusTip(tr("Select all the cells in the "
                                            "current column"));
        connect(selectColumnAction, SIGNAL(triggered()),
                spreadsheet, SLOT(selectCurrentColumn()));

        selectAllAction = new QAction(tr("&All"), this);
        selectAllAction->setShortcut(QKeySequence::SelectAll);
        selectAllAction->setStatusTip(tr("Select all the cells in the "
                                         "spreadsheet"));
        connect(selectAllAction, SIGNAL(triggered()),
                spreadsheet, SLOT(selectAll()));

        findAction = new QAction(tr("&Find..."), this);
        findAction->setIcon(QIcon(":/image/find.png"));
        findAction->setShortcut(QKeySequence::Find);
        findAction->setStatusTip(tr("Find a matching cell"));
        connect(findAction, SIGNAL(triggered()), this, SLOT(find()));

        goToCellAction = new QAction(tr("&Go to Cell..."), this);
        goToCellAction->setIcon(QIcon(":/image/gotocell.png"));
        goToCellAction->setShortcut(tr("Ctrl+G"));
        goToCellAction->setStatusTip(tr("Go to the specified cell"));
        connect(goToCellAction, SIGNAL(triggered()),
                this, SLOT(goToCell()));

        recalculateAction = new QAction(tr("&Recalculate"), this);
        recalculateAction->setShortcut(tr("F9"));
        recalculateAction->setStatusTip(tr("Recalculate all the "
                                           "spreadsheet's formulas"));
        connect(recalculateAction, SIGNAL(triggered()),
                spreadsheet, SLOT(recalculate()));

        sortAction = new QAction(tr("&Sort..."), this);
        sortAction->setStatusTip(tr("Sort the selected cells or all the "
                                    "cells"));
        connect(sortAction, SIGNAL(triggered()), this, SLOT(sort()));

        showGridAction = new QAction(tr("&Show Grid"), this);
        showGridAction->setCheckable(true);
        showGridAction->setChecked(spreadsheet->showGrid());
        showGridAction->setStatusTip(tr("Show or hide the spreadsheet's "
                                        "grid"));
        connect(showGridAction, SIGNAL(toggled(bool)),
                spreadsheet, SLOT(setShowGrid(bool)));
    #if QT_VERSION < 0x040102
        // workaround for a QTableWidget bug in Qt 4.1.1
        connect(showGridAction, SIGNAL(toggled(bool)),
                spreadsheet->viewport(), SLOT(update()));
    #endif

        autoRecalcAction = new QAction(tr("&Auto-Recalculate"), this);
        autoRecalcAction->setCheckable(true);
        autoRecalcAction->setChecked(spreadsheet->autoRecalculate());
        autoRecalcAction->setStatusTip(tr("Switch auto-recalculation on or "
                                          "off"));
        connect(autoRecalcAction, SIGNAL(toggled(bool)),
                spreadsheet, SLOT(setAutoRecalculate(bool)));

        aboutAction = new QAction(tr("&About"), this);
        aboutAction->setStatusTip(tr("Show the application's About box"));
        connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

        aboutQtAction = new QAction(tr("About &Qt"), this);
        aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
        connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    }

}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    //关联menu到action
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    separatorAction = fileMenu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
        fileMenu->addAction(recentFileActions[i]);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
    //重复的其他功能组件
    {
        editMenu = menuBar()->addMenu(tr("&Edit"));
        editMenu->addAction(cutAction);
        editMenu->addAction(copyAction);
        editMenu->addAction(pasteAction);
        editMenu->addAction(deleteAction);

        selectSubMenu = editMenu->addMenu(tr("&Select"));
        selectSubMenu->addAction(selectRowAction);
        selectSubMenu->addAction(selectColumnAction);
        selectSubMenu->addAction(selectAllAction);

        editMenu->addSeparator();
        editMenu->addAction(findAction);
        editMenu->addAction(goToCellAction);

        toolsMenu = menuBar()->addMenu(tr("&Tools"));
        toolsMenu->addAction(recalculateAction);
        toolsMenu->addAction(sortAction);

        optionsMenu = menuBar()->addMenu(tr("&Options"));
        optionsMenu->addAction(showGridAction);
        optionsMenu->addAction(autoRecalcAction);

        menuBar()->addSeparator();

        helpMenu = menuBar()->addMenu(tr("&Help"));
        helpMenu->addAction(aboutAction);
        helpMenu->addAction(aboutQtAction);
    }
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("&File"));
    fileToolBar->addAction(newAction);
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(saveAction);
    //重复的其他功能组件
    {
        editToolBar = addToolBar(tr("&Edit"));
        editToolBar->addAction(cutAction);
        editToolBar->addAction(copyAction);
        editToolBar->addAction(pasteAction);
        editToolBar->addSeparator();
        editToolBar->addAction(findAction);
        editToolBar->addAction(goToCellAction);
    }
}

void MainWindow::createContextMenu()
{
    spreadsheet->addAction(cutAction);
    spreadsheet->addAction(copyAction);
    spreadsheet->addAction(pasteAction);
    spreadsheet->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createStatusBar()
{
    locationLabel = new QLabel(" W999 ");
    locationLabel->setAlignment(Qt::AlignHCenter);
    locationLabel->setMinimumSize(locationLabel->sizeHint());

    formulaLabel = new QLabel;
    formulaLabel->setIndent(3);

    statusBar()->addWidget(locationLabel);
    statusBar()->addWidget(formulaLabel, 1);

    connect(spreadsheet, SIGNAL(currentCellChanged(int, int, int, int)),
            this, SLOT(updateStatusBar()));
    connect(spreadsheet, SIGNAL(modified()),
            this, SLOT(spreadsheetModified()));

    updateStatusBar();
}

void MainWindow::setCurrentFile(const QString &fileName)
{
//    curFile = fileName;
//    setWindowModified(false);

//    QString shownName = tr("Untitled");
//    if(!CurFile.isE)
}

bool MainWindow::saveFile(const QString &fileName)
{

}

bool MainWindow::loadFile(const QString &fileName)
{
    if (!spreadsheet->readFile(fileName)) {
        statusBar()->showMessage(tr("Loading canceled"), 2000);
        return false;
    }

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
    return true;
}

void MainWindow::readSettings()
{
    QSettings settings("Software Inc.", "Spreadsheet");

    restoreGeometry(settings.value("geometry").toByteArray());

    recentFiles = settings.value("recentFiles").toStringList();
    updateRecentFileActions();

    bool showGrid = settings.value("showGrid", true).toBool();
    showGridAction->setChecked(showGrid);

    bool autoRecalc = settings.value("autoRecalc", true).toBool();
    autoRecalcAction->setChecked(autoRecalc);
}

void MainWindow::writeSettings()
{
    QSettings settings("Software Inc.", "Spreadsheet");

    settings.setValue("geometry", saveGeometry());
    settings.setValue("recentFiles", recentFiles);
    settings.setValue("showGrid", showGridAction->isChecked());
    settings.setValue("autoRecalc", autoRecalcAction->isChecked());
}

void MainWindow::updateRecentFileActions()
{
    QMutableStringListIterator it(recentFiles);
    if(it.hasNext()) {
        if(QFile::exists(it.next()))
            it.remove();
    }
    for (int j = 0; j < MaxRecentFiles; ++j) {
        if (j < recentFiles.count()) {
            QString text = tr("&%1 %2")
                           .arg(j + 1)
                           .arg(strippedName(recentFiles[j]));
            recentFileActions[j]->setText(text);
            recentFileActions[j]->setData(recentFiles[j]);
            recentFileActions[j]->setVisible(true);
        } else {
            recentFileActions[j]->setVisible(false);
        }
    }
    separatorAction->setVisible(!recentFiles.isEmpty());
}

bool MainWindow::okToContinue()
{
    if(isWindowModified()) {
        int r = QMessageBox::warning(this, tr("SpreadSheet"),
                                              tr("the document has been modified.\n"
                                                 "Do you want to save your changes?"),
                                     QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (r == QMessageBox::Yes) {
            return save();
        } else if(r == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

static QString strippedName(const QString &fullFileName)
{
//    return QFileInfo(fullFileName).fileName();
}

void MainWindow::spreadsheetModified()
{
    setWindowModified(true);
    updateStatusBar();
}
