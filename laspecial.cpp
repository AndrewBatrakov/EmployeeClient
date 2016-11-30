//#include <QtSql>
#include "laspecial.h"
#include "fordelete.h"
#include "numprefix.h"
#include "intitemdelegate.h"
//#include "latermform.moc"

LaSpecialForm::LaSpecialForm(QString id,QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    indexTemp = id;
    model = new QSqlTableModel;
    model->setEditStrategy(QSqlTableModel::OnRowChange);
    tableView = new QTableView;

    model->setTable("laspecialtable");

    labelName = new QLabel(tr("Name:"));
    editName = new LineEdit;
    editName->setReadOnly(onlyForRead);
    //QRegExp regExp("[\\x4010-\\x044f 0-9 - ,. ]{150}");
    //editName->setValidator(new QRegExpValidator(regExp,this));
    labelName->setBuddy(editName);

    saveButton = new QPushButton(tr("Save"));
    connect(saveButton,SIGNAL(clicked()),this,SLOT(editRecord()));

    cancelButton = new QPushButton(tr("Cancel"));
    cancelButton->setDefault(true);
    cancelButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(accept()));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(cancelButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(saveButton,QDialogButtonBox::ActionRole);

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT laspecialname FROM laspecial WHERE laspecialid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        query.next();
        editName->setText(query.value(0).toString());
        model->setFilter(QString("laspecialid == '%1'").arg(indexTemp));
        model->select();
    }else{
        editName->clear();
        editName->setText(QObject::tr("Name of Special Table"));
        editName->selectAll();
    }

    QPushButton *addRecordTableB = new QPushButton(tr("Add Record"));
    addRecordTableB->setIcon(QPixmap(":/add.png"));
    connect(addRecordTableB,SIGNAL(clicked()),this,SLOT(addRecordTable()));
    QPushButton *deleteRecordTableB = new QPushButton(tr("Delete Record"));
    deleteRecordTableB->setIcon(QPixmap(":/delete.png"));
    connect(deleteRecordTableB,SIGNAL(clicked()),this,SLOT(deleteRecordTable()));
    QPushButton *editRecordTableB = new QPushButton(tr("EditRecordTable"));
    editRecordTableB->setIcon(QPixmap(":/edit.png"));
    connect(editRecordTableB,SIGNAL(clicked()),this,SLOT(editRecordTable()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addRecordTableB);
    buttonLayout->addWidget(deleteRecordTableB);
    buttonLayout->addWidget(editRecordTableB);
    buttonLayout->addStretch();

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelName,0,0);
    mainLayout->addWidget(editName,0,1);
    mainLayout->addLayout(buttonLayout,1,0,1,2);
    mainLayout->addWidget(tableView,2,0,1,2);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,3,1);
        editName->selectAll();
    }
    model->setHeaderData(2,Qt::Horizontal,tr("Type Of Work\nwork space"));
    model->setHeaderData(3,Qt::Horizontal,tr("Sound Level"));
    tableView->setModel(model);
    tableView->setColumnHidden(0,true);
    tableView->setColumnHidden(1,true);
    connect(tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(editRecordTable()));
    connect(tableView,SIGNAL(entered(QModelIndex)),this,SLOT(editRecordTable()));
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    //tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableView->setItemDelegate(new QStyledItemDelegate());
    tableView->setItemDelegateForColumn(3, new IntItemDelegate());
    tableView->setAlternatingRowColors(true);
    tableView->resizeColumnsToContents();
    QHeaderView *head = tableView->horizontalHeader();
    head->setStretchLastSection(true);
    setLayout(mainLayout);

    setWindowTitle(tr("Special Tables"));
}

void LaSpecialForm::editRecord()
{
    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("UPDATE laspecial SET laspecialname = :name WHERE  laspecialid = :id");
        query.bindValue(":name",editName->text());
        query.bindValue(":id",indexTemp);
        query.exec();
    }else{
        QSqlQuery query;
        query.prepare("SELECT * FROM laspecial WHERE laspecialname = :name");
        query.bindValue(":name",editName->text().simplified());
        query.exec();
        query.next();
        if(!query.isValid()){
            NumPrefix numPrefix;
            indexTemp = numPrefix.getPrefix("laspecial");
            QSqlQuery query;
            query.prepare("INSERT INTO laspecial (laspecialid, laspecialname) VALUES(:id, :name)");
            query.bindValue(":id",indexTemp);
            query.bindValue(":name",editName->text().simplified());
            query.exec();
        }else{
            QString tempString = editName->text();
            tempString += QObject::tr(" is availble!");
            QMessageBox::warning(this,QObject::tr("Atention!!!"),tempString);
        }
    }
    emit accept();
    close();
}

void LaSpecialForm::deleteRecord()
{
    ForDelete forDelete(indexTemp,"special",this);
    forDelete.exec();
    forDelete.deleteOnDefault();
    if(indexTemp != "OWN000000001"){
        QSqlQuery query;
        query.prepare("DELETE FROM laspecial WHERE laspecialid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        query.next();
    }else{
        QMessageBox::warning(this,QObject::tr("Attention"),QObject::tr("You dont may delete default value!"));
    }
}

void LaSpecialForm::done(int result)
{
    model->submitAll();
    QDialog::done(result);
}

void LaSpecialForm::addRecordTable()
{
    int row = model->rowCount();
    model->insertRow(row);
    NumPrefix numPrefix;
    QString tablePref = numPrefix.getPrefix("laspecialtable");
    model->setData(model->index(row,0),tablePref);
    model->setData(model->index(row,1),indexTemp);
    //tableView->itemDelegateForColumn()
}

void LaSpecialForm::editRecordTable()
{

}

void LaSpecialForm::deleteRecordTable()
{
    tableView->model()->removeRow(tableView->currentIndex().row());
    tableView->model()->submit();
    model->select();
}
