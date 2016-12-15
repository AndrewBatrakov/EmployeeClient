#include <QtSql>

#include "postform.h"
#include "lineedit.h"
#include "numprefix.h"
#include "fordelete.h"

PostForm::PostForm(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    exchangeFile.setFileName("Message.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }

    indexTemp = id;

    labelPost = new QLabel(trUtf8("Наименование:"));
    editPost = new LineEdit;
    editPost->setReadOnly(onlyForRead);
    QRegExp regExp("[\\x0410-\\x044f 0-9 \" -]{150}");
    editPost->setValidator(new QRegExpValidator(regExp,this));
    labelPost->setBuddy(editPost);

    labelItr = new QLabel(trUtf8("ИТР:"));
    editItr = new QCheckBox;
    labelItr->setBuddy(editItr);

    savePushButton = new QPushButton(trUtf8("Записать"));
    connect(savePushButton,SIGNAL(clicked()),this,SLOT(editRecord()));
    savePushButton->setToolTip(trUtf8("Кнопка \"Записать и закрыть\""));

    cancelPushButton = new QPushButton(trUtf8("Отмена"));
    cancelPushButton->setDefault(true);
    cancelPushButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelPushButton,SIGNAL(clicked()),this,SLOT(accept()));
    cancelPushButton->setToolTip(trUtf8("Кнопка отмены"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(cancelPushButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(savePushButton,QDialogButtonBox::ActionRole);

    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("SELECT postname, itr FROM post WHERE postid = ?");
        query.addBindValue(indexTemp);
        query.exec();
        while(query.next()){
            editPost->setText(query.value(0).toString());
            if(query.value(1).toBool()){
                editItr->setChecked(true);
            }else{
                editItr->setChecked(false);
            }
        }
    }else{
        editPost->clear();
    }

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(labelPost,0,0);
    mainLayout->addWidget(editPost,0,1);
    mainLayout->addWidget(labelItr,1,0);
    mainLayout->addWidget(editItr,1,1);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox,2,1);
        editPost->selectAll();
    }

    setLayout(mainLayout);

    setWindowTitle(trUtf8("Должность"));
    readSettings();
}

void PostForm::editRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    int checkState;
    if(editItr->isChecked()){
        checkState = 1;
    }else{
        checkState = 0;
    }
    if(indexTemp != ""){
        QSqlQuery query;
        query.prepare("UPDATE post SET postname = :name, postuppername = :postuppername, itr = :itr WHERE  postid = :id");
        query.bindValue(":name",editPost->text());
        query.bindValue(":postuppername",editPost->text().toUpper());
        query.bindValue(":itr",checkState);
        query.bindValue(":id",indexTemp);
        query.exec();
        line += "UPDATE post SET postname = '";
        line += editPost->text().toUtf8();
        line += "', postuppername = '";
        line += editPost->text().toUpper().toUtf8();
        line += "', itr = '";
        line += QString::number(checkState);
        line +=  "' WHERE  postid = '";
        line += indexTemp;
        line += "'";
        line += "\r\n";
        stream<<line;
    }else{
        QSqlQuery query;
        query.prepare("SELECT * FROM post WHERE postname = :postname");
        query.bindValue(":postname",editPost->text().simplified());
        query.exec();
        query.next();
        if(!query.isValid()){
            NumPrefix numPrefix;
            indexTemp = numPrefix.getPrefix("post");
            QSqlQuery query;
            query.prepare("INSERT INTO post (postid, postname, postuppername, itr) VALUES("
                          ":idpost, :namepost, :postuppername, :itr)");
            query.bindValue(":idpost",indexTemp);
            query.bindValue(":namepost",editPost->text().simplified());
            query.bindValue(":postuppername",editPost->text().toUpper().toUtf8());
            query.bindValue(":itr",checkState);
            query.exec();
            line += "INSERT INTO post (postid, postname, postuppername, itr) VALUES('";
            line += indexTemp.toUtf8();
            line += "', '";
            line += editPost->text().toUtf8();
            line += "', '";
            line += editPost->text().toUpper().toUtf8();
            line += "', '";
            line += QString::number(checkState);
            line += "')";
            line += "\r\n";
            stream<<line;
        }else{
            QString tempString = editPost->text();
            tempString += QObject::trUtf8(" существует!");
            QMessageBox::warning(this,QObject::trUtf8("Внимание!!!"),tempString);
        }
    }
    emit accept();
    close();
}

void PostForm::deleteRecord()
{
    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }

    ForDelete forDelete(indexTemp,"post",this);

    if(forDelete.result() == QDialog::Accepted){
        forDelete.exec();
        int k = QMessageBox::warning(this,trUtf8("Внимание!!!"),trUtf8("Действительно удалить?"),
                                     QMessageBox::No|QMessageBox::Yes,QMessageBox::No);
        if(k == QMessageBox::Yes){
            forDelete.deleteOnDefault();

            QSqlQuery query;
            query.prepare("DELETE FROM post WHERE postid = :id");
            query.bindValue(":id",indexTemp);
            query.exec();
            query.next();
            line += "DELETE FROM post WHERE postid = '";
            line += indexTemp;
            line += "'";
            line += "\r\n";
            stream<<line;
        }
    }
}

void PostForm::done(int result)
{
    writeSettings();
    QDialog::done(result);
}

void PostForm::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    restoreGeometry(settings.value("PostForm").toByteArray());
}

void PostForm::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("PostForm", saveGeometry());
}
