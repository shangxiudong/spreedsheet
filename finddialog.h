#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QCheckBox;
class QPushButton;

class FindDialog final : public QDialog
{
    Q_OBJECT
public:
    FindDialog(QWidget *parent = nullptr);
    virtual ~FindDialog() = default;

signals:
    void findPrevious(const QString &text, Qt::CaseSensitivity cs);
    void findNext(const QString &text, Qt::CaseSensitivity cs);

private slots:
    void on_findButton_clicked();
    void on_closeButton_clicked();
    void enable_findButton(const QString &text);

private:
    void initUI();
private:
    QLabel *label;
    QLineEdit *lineEdit;
    QCheckBox *case_checkBox;
    QCheckBox *backSearch_checkBox;
    QPushButton *findButton;
    QPushButton *closeButton;

};

#endif // FINDDIALOG_H
