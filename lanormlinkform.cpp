#include <QtGui>
#include <QtSql>

#include "lanormlinkform.h"
#include "numprefix.h"
#include "fordelete.h"
#include "bolditemdelegate.h"

LaNormLinkForm::LaNormLinkForm(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    indexTemp = id;

    model = new QSqlTableModel;
    model->setEditStrategy(QSqlTableModel::OnRowChange);
    tableView = new QTableView;
    model->setTable("lanormlinktable");

    labelName = new QLabel(trUtf8("Name:"));
    editName = new LineEdit;
    editName->setReadOnly(onlyForRead);
    labelName->setBuddy(editName);

    QSqlQueryModel *fpModel = new QSqlQueryModel;
    fpModel->setQuery("SELECT laproductionfactorname FROM laproductionfactor");
    QCompleter *fpCompleter = new QCompleter(fpModel);
    fpCompleter->setCompletionMode(QCompleter::PopupCompletion);
    fpCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    editName->setCompleter(fpCompleter);

    saveButton = new QPushButton(trUtf8("Save"));
    connect(saveButton,SIGNAL(clicked()),this,SLOT(editRecord()));

    cancelButton = new QPushButton(trUtf8("Cancel"));
    cancelButton->setDefault(true);
    cancelButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(accept()));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(cancelButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(saveButton,QDialogButtonBox::ActionRole);

    QPushButton *addRecordTableB = new QPushButton(trUtf8("Add Record"));
    addRecordTableB->setIcon(QPixmap(":/add.png"));
    connect(addRecordTableB,SIGNAL(clicked()),this,SLOT(addRecordTable()));
    QPushButton *deleteRecordTableB = new QPushButton(trUtf8("Delete Record"));
    deleteRecordTableB->setIcon(QPixmap(":/delete.png"));
    connect(deleteRecordTableB,SIGNAL(clicked()),this,SLOT(deleteRecordTable()));
    QPushButton *editRecordTableB = new QPushButton(trUtf8("EditRecordTable"));
    editRecordTableB->setIcon(QPixmap(":/edit.png"));
    //connect(editRecordTableB,SIGNAL(clicked()),this,SLOT(editRecordTable()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addRecordTableB);
    buttonLayout->addWidget(deleteRecordTableB);
    buttonLayout->addWidget(editRecordTableB);
    buttonLayout->addStretch();

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT laproductionfactorname FROM laproductionfactor WHERE laproductionfactorid IN (SELECT lalanormlinkpfid FROM lanormlink WHERE lanormlinkid = :id)");
        query.bindValue(":id",indexTemp);
        query.exec();
        query.next();
        editName->setText(query.value(0).toString());
        model->setFilter(QString("lanormlinkid == '%1'").arg(indexTemp));
        model->select();
    }else{
        editName->clear();
        editName->setText(QObject::trUtf8("Name of Production Factor"));
        editName->selectAll();
    }
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelName,0,0);
    mainLayout->addWidget(editName,0,1);
    mainLayout->addLayout(buttonLayout,1,0,1,2);
    mainLayout->addWidget(tableView,2,0,1,2);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,3,1);
        editName->selectAll();
    }
    model->setHeaderData(2,Qt::Horizontal,trUtf8("Number"));
    model->setHeaderData(3,Qt::Horizontal,trUtf8("Description"));
    tableView->setModel(model);
    tableView->setColumnHidden(0,true);
    tableView->setColumnHidden(1,true);
    //connect(tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(editRecordTable()));
    //connect(tableView,SIGNAL(entered(QModelIndex)),this,SLOT(editRecordTable()));
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    //tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //tableView->setItemDelegate(new QStyledItemDelegate());
    tableView->setItemDelegateForColumn(2, new BoldItemDelegate(Qt::darkBlue));
    tableView->setAlternatingRowColors(true);
    tableView->resizeColumnsToContents();
    QHeaderView *head = tableView->horizontalHeader();
    head->setStretchLastSection(true);
    setLayout(mainLayout);

    setWindowTitle(trUtf8("Norm link"));
}

void LaNormLinkForm::editRecord()
{
    QSqlQuery queryPF;
    queryPF.prepare("SELECT lanormlinkid FROM lanormlink WHERE lanormlinkname = :name");
    queryPF.bindValue(":name",editName->text());
    queryPF.exec();
    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("UPDATE lanormlink SET lanormlinkpfid = :pfid WHERE  lanormlinkid = :id");
        query.bindValue(":name",queryPF.value(0).toString());
        query.bindValue(":id",indexTemp);
        query.exec();
        //indexTemp = editName->text();
    }else{
        QSqlQuery query;
        query.prepare("SELECT * FROM lanormlink WHERE lanormlinkname = :name");
        query.bindValue(":name",editName->text().simplified());
        query.exec();
        query.next();
        if(!query.isValid()){
            NumPrefix numPrefix;
            indexTemp = numPrefix.getPrefix("lanormlink");
            if(indexTemp == ""){
                close();
            }
            QSqlQuery query;
            query.prepare("INSERT INTO lanormlink (lanormlinkid, lanormlinkname) VALUES(:id, :name)");
            query.bindValue(":id",indexTemp);
            query.bindValue(":name",editName->text().simplified());
            query.exec();
            //ind = editName->text();
        }else{
            QString tempString = editName->text();
            tempString += QObject::trUtf8(" is availble!");
            QMessageBox::warning(this,QObject::trUtf8("Atention!!!"),tempString);
        }
    }
    emit accept();
    close();
}

void LaNormLinkForm::deleteRecord()
{
    ForDelete forDelete(indexTemp,"lanormlink",this);
    forDelete.exec();
    forDelete.deleteOnDefault();
    if(indexTemp != "OWN000000001"){
        QSqlQuery query;
        query.prepare("DELETE FROM lanormlink WHERE lanormlinkid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        query.next();
    }else{
        QMessageBox::warning(this,QObject::trUtf8("Attention"),QObject::trUtf8("You dont may delete default value!"));
    }
}

void LaNormLinkForm::done(int result)
{
    model->submitAll();
    QDialog::done(result);
}

void LaNormLinkForm::addRecordTable()
{
    int row = model->rowCount();
    model->insertRow(row);
    NumPrefix numPrefix;
    QString tablePref = numPrefix.getPrefix("laspecialtable");
    model->setData(model->index(row,0),tablePref);
    model->setData(model->index(row,1),indexTemp);
}

void LaNormLinkForm::deleteRecordTable()
{
    tableView->model()->removeRow(tableView->currentIndex().row());
    tableView->model()->submit();
    model->select();
}
