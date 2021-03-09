#ifndef GOTOCELLDIALOG_H
#define GOTOCELLDIALOG_H
#include <QDialog>
class QDialogButtonBox;
class QLabel;
class QLineEdit;
class GoToCellDialog : public QDialog
{
    Q_OBJECT
public:
    GoToCellDialog(QWidget *parent = nullptr);
private slots:
    void on_lineEdit_textChanged();
private:
    QDialogButtonBox *buttonBox = nullptr;
    QLabel *label;
public:
    QLineEdit *lineEdit;

};

#endif // GOTOCELLDIALOG_H
