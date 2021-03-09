#include "finddialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

FindDialog::FindDialog(QWidget *parent)
    : QDialog(parent)
    , label(new QLabel(tr("Find &what:")))
    , lineEdit(new QLineEdit())
    , case_checkBox(new QCheckBox(tr("Match &case")))
    , backSearch_checkBox(new QCheckBox(tr("Search &back")))
    , findButton(new QPushButton(tr("&Find")))
    , closeButton(new QPushButton(tr("Close")))
{
    label->setBuddy(lineEdit);
    findButton->setDefault(true);
    findButton->setEnabled(false);
    initUI();

    connect(lineEdit, &QLineEdit::textChanged, this, &FindDialog::enable_findButton);
    connect(findButton, &QPushButton::clicked, this, &FindDialog::on_findButton_clicked);
    connect(closeButton, &QPushButton::clicked, this, &FindDialog::close);

}

void FindDialog::on_findButton_clicked()
{
    QString text = lineEdit->text();
    Qt::CaseSensitivity cs = case_checkBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
    if (backSearch_checkBox->isChecked())
        emit findPrevious(text, cs);
    else
        emit findNext(text, cs);
}

void FindDialog::on_closeButton_clicked()
{

}

void FindDialog::enable_findButton(const QString &text)
{
    if(!text.isEmpty())
        findButton->setEnabled(true);
    else
        findButton->setEnabled(false);
}

void FindDialog::initUI()
{
    QHBoxLayout *main_layout = new QHBoxLayout;
    setLayout(main_layout);
    QHBoxLayout *line_layout = new QHBoxLayout;
    line_layout->addWidget(label);
    line_layout->addWidget(lineEdit);
    QVBoxLayout *left_layout = new QVBoxLayout;
    left_layout->addLayout(line_layout);
    left_layout->addWidget(case_checkBox);
    left_layout->addWidget(backSearch_checkBox);
    left_layout->addStretch();

    QVBoxLayout *right_layout = new QVBoxLayout;
    right_layout->addWidget(findButton);
    right_layout->addWidget(closeButton);
    right_layout->addStretch();
    main_layout->addLayout(left_layout);
    main_layout->addLayout(right_layout);
}

