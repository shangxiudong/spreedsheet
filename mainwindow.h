#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Spreadsheet;
class QAction;
class QMenu;
class QLabel;
class FindDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;
private slots:
    void newFile();
    bool save();
    void open();
    bool saveAs();
    void find();
    void goToCell();

    void openRecentFile();
    void spreadsheetModified();
    void updateStatusBar();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createContextMenu();
    void createStatusBar();

    void setCurrentFile(const QString &fileName);

    bool saveFile(const QString &fileName);
    bool loadFile(const QString &fileName);

    //利用Qt的Qsetting读写一些数据
    void readSettings();
    void writeSettings();
    //更新RecentFileAction
    void updateRecentFileActions();
    bool okToContinue();
    QString strippedName(const QString &fullFilename);

private:
    Spreadsheet *spreadsheet;
    FindDialog *findDialog;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *slectSubMenu;
    QMenu *toolsMenu;
    QMenu *optionsMenu;
    QMenu *helpMenu;
    QMenu *selectSubMenu;

    QString curFile;

    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *exitAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *deleteAction;
    QAction *selectRowAction;
    QAction *selectColumnAction;
    QAction *selectAllAction;
    QAction *findAction;
    QAction *goToCellAction;
    QAction *recalculateAction;
    QAction *sortAction;
    QAction *showGridAction;
    QAction *autoRecalcAction;
    QAction *aboutAction;
    QAction *aboutQtAction;

    enum { MaxRecentFiles = 5 };
    QAction *recentFileActions[MaxRecentFiles];
    QAction *separatorAction;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;

    //状态栏图标
    QLabel *locationLabel;
    QLabel *formulaLabel;

    QStringList recentFiles;
};

#endif // MAINWINDOW_H
