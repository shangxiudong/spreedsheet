#include <QtGui>
#include "myspreadsheet.h"
#include "cell.h"
#include <QFile>
#include <QMessageBox>
#include <QApplication>

Spreadsheet::Spreadsheet(QWidget *parent) :
    QTableWidget(parent)
{
    autoRecalc = true;
    setItemPrototype(new Cell); //设置填充的单元格类型
    setSelectionMode(ContiguousSelection); //可连续选择
    connect(this, static_cast<void (QTableWidget::*)(QTableWidgetItem *)>(&QTableWidget::itemChanged), //使用强转，识别不同的构造函数
            this, &Spreadsheet::somethingChanged);
    clear();

}

QString Spreadsheet::currentLocation() const
{
    return QChar('A' + currentColumn()) + QString::number(currentRow() + 1);
}

QString Spreadsheet::currentFormula() const
{
    return formula(currentRow(), currentColumn());
}

QTableWidgetSelectionRange Spreadsheet::selectedRange() const
{

}

void Spreadsheet::clear()
{
    setRowCount(0);
    setColumnCount(0);
    setRowCount(RowCount);
    setColumnCount(ColumnCount);

    //设置列标题
    for(int i = 0; i < ColumnCount; ++i) {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(QString(char('A' + i)));
        setHorizontalHeaderItem(i, item);
    }
    setCurrentCell(0, 0);

}

bool Spreadsheet::readFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("spreadsheet"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_3);
    quint32 magic;
    in >> magic;
    if(magic != magicNumber) {
        QMessageBox::warning(this, tr("Spreadsheet"),
                             tr("the file is not a spreadsheet file."));
        return false;
    }
    clear();

    quint16 row;
    quint16 column;
    QString str;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    while (!in.atEnd()) {
        in >> row >> column >> str;
        setFormula(row, column, str);
    }
    QApplication::restoreOverrideCursor();
    return   true;
}

bool Spreadsheet::writeFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("Spreadsheet"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_3);
    out << quint32(magicNumber);  //自己的编码方式保存文件
    QApplication::setOverrideCursor(Qt::WaitCursor);
    for(int row=0; row<RowCount; ++row) {
        for(int column=0; column < ColumnCount; ++column) {
            QString str = formula(row, column);
            if(!str.isEmpty())
                out << quint16(row) << quint16(column) << str;
        }
    }
    QApplication::restoreOverrideCursor();
    return true;
}

void Spreadsheet::sort(const SpreadsheetCompare &compare)
{

}

void Spreadsheet::cut()
{

}

void Spreadsheet::copy()
{

}

void Spreadsheet::paste()
{

}

void Spreadsheet::del()
{

}

void Spreadsheet::selectCurrentRow()
{

}

void Spreadsheet::selectCurrentColumn()
{

}

void Spreadsheet::recalculate()
{

}

void Spreadsheet::setAutoRecalculate(bool recalc)
{

}

void Spreadsheet::findNext(const QString &str, Qt::CaseSensitivity cs)
{

}

void Spreadsheet::findPrevious(const QString &str, Qt::CaseSensitivity cs)
{

}

void Spreadsheet::somethingChanged()
{
    if(autoRecalc)
        recalculate();
    emit modified();
}

Cell *Spreadsheet::cell(int row, int column) const
{
    return static_cast<Cell *>(item(row, column));
}

QString Spreadsheet::text(int row, int column) const
{
    Cell *c = cell(row, column);
    if(c) {
        return c->formula();
    } else {
        return "";
    }
}

QString Spreadsheet::formula(int row, int column) const
{
 Cell *c = cell(row, column);
 if(c) {
     return c->formula();
 } else {
     return "";
 }
}

void Spreadsheet::setFormula(int row, int column, const QString &formula)
{
    Cell *c = cell(row, column);
    if(!c) {
        c = new Cell;
        setItem(row, column, c);
    }
    c->setFormula(formula);
}
