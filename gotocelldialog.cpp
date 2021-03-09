#include "gotocelldialog.h"
#include <QLineEdit>
#include <QLabel>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRegExp>
#include <QPushButton>
#include <QRegExpValidator>

GoToCellDialog::GoToCellDialog(QWidget *parent):
    buttonBox(new QDialogButtonBox()),
    label(new QLabel("&Cell Location")),
    lineEdit(new QLineEdit())
{
    QVBoxLayout *mainlayout = new QVBoxLayout;
    buttonBox->addButton(QDialogButtonBox::Ok);
    buttonBox->addButton(QDialogButtonBox::Cancel);
    QHBoxLayout *h1 = new QHBoxLayout;
    h1->addWidget(label);
    h1->addWidget(lineEdit);
    QHBoxLayout *h2 = new QHBoxLayout;
    h2->addStretch();
    h2->addWidget(buttonBox);
    mainlayout->addLayout(h1);
    mainlayout->addLayout(h2);
    setLayout(mainlayout);

    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    QRegExp regExp("[A-Za-z][1-9][0-9]{0,2}");
    lineEdit->setValidator(new QRegExpValidator(regExp, this));

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void GoToCellDialog::on_lineEdit_textChanged()
{
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
                lineEdit->hasAcceptableInput());
}
