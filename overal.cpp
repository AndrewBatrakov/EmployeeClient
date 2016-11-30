#include <QtWidgets>
#include <QtSql>

#include "overal.h"
#include "numprefix.h"
#include "fordelete.h"

OveralForm::OveralForm(QString id, QWidget *parent, bool onlyForRead) :
    QDialog(parent)
{
    exchangeFile.setFileName("Message.txt");
    if(!exchangeFile.isOpen()){
        exchangeFile.open(QIODevice::ReadWrite);
    }

    indexTemp = id;

    labelGrowth = new QLabel(trUtf8("Рост:"));
    editGrowth = new LineEdit;
    editGrowth->setReadOnly(onlyForRead);
    labelGrowth->setBuddy(editGrowth);

    QHBoxLayout *growthLayout = new QHBoxLayout;
    growthLayout->addWidget(labelGrowth);
    growthLayout->addWidget(editGrowth);

    labelHeaddress = new QLabel(trUtf8("Головной убор:"));
    editHeaddress = new LineEdit;
    editHeaddress->setReadOnly(onlyForRead);

    QHBoxLayout *headdressLayout = new QHBoxLayout;
    headdressLayout->addWidget(labelHeaddress);
    headdressLayout->addWidget(editHeaddress);

    labelWinChlothes = new QLabel(trUtf8("Зима:"));
    editWinClothes = new LineEdit;
    editWinClothes->setReadOnly(onlyForRead);

    QHBoxLayout *winClothesLayout = new QHBoxLayout;
    winClothesLayout->addWidget(labelWinChlothes);
    winClothesLayout->addWidget(editWinClothes);

    labelSumClothes = new QLabel(trUtf8("Лето:"));
    editSumClothes = new LineEdit;
    editSumClothes->setReadOnly(onlyForRead);

    QHBoxLayout *sumClothesLayout = new QHBoxLayout;
    sumClothesLayout->addWidget(labelSumClothes);
    sumClothesLayout->addWidget(editSumClothes);

    QVBoxLayout *clothesLayout = new QVBoxLayout;
    clothesLayout->addLayout(winClothesLayout);
    clothesLayout->addLayout(sumClothesLayout);

    QGroupBox *clothes = new QGroupBox(trUtf8("Одежда"));
    clothes->setLayout(clothesLayout);

    labelWinShoes = new QLabel(trUtf8("Зима:"));
    editWinShoes = new LineEdit;
    editWinShoes->setReadOnly(onlyForRead);

    QHBoxLayout *winShoesLayout = new QHBoxLayout;
    winShoesLayout->addWidget(labelWinShoes);
    winShoesLayout->addWidget(editWinShoes);

    labelSumShoes = new QLabel(trUtf8("Лето:"));
    editSumShoes = new LineEdit;
    editSumShoes->setReadOnly(onlyForRead);

    QHBoxLayout *sumShoesLayout = new QHBoxLayout;
    sumShoesLayout->addWidget(labelSumShoes);
    sumShoesLayout->addWidget(editSumShoes);

    QVBoxLayout *shoesLayout = new QVBoxLayout;
    shoesLayout->addLayout(winShoesLayout);
    shoesLayout->addLayout(sumShoesLayout);

    QGroupBox *shoes = new QGroupBox(trUtf8("Обувь"));
    shoes->setLayout(shoesLayout);

    labelRespirator = new QLabel(trUtf8("Респиратор:"));
    editRespirator = new LineEdit;
    editRespirator->setReadOnly(onlyForRead);

    labelGasmask = new QLabel(trUtf8("Противогаз:"));
    editGasmask = new LineEdit;
    editGasmask->setReadOnly(onlyForRead);

    labelMittens = new QLabel(tr("Mittens:"));
    editMittens = new LineEdit;
    editMittens->setReadOnly(onlyForRead);

    labelGloves = new QLabel(tr("Gloves:"));
    editGloves = new LineEdit;
    editGloves->setReadOnly(onlyForRead);


    savePushButton = new QPushButton(tr("Save"));
    connect(savePushButton,SIGNAL(clicked()),this,SLOT(editRecord()));
    savePushButton->setToolTip(tr("Save And Close Button"));

    cancelPushButton = new QPushButton(tr("Cancel"));
    cancelPushButton->setDefault(true);
    cancelPushButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelPushButton,SIGNAL(clicked()),this,SLOT(accept()));
    cancelPushButton->setToolTip(tr("Cancel Button"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(cancelPushButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(savePushButton,QDialogButtonBox::ActionRole);

    QSqlQuery query;
    query.prepare("SELECT * FROM overal WHERE employeeid = :id");
    query.bindValue(":id",indexTemp);
    query.exec();
    query.next();
    if(query.isValid()){
        editGrowth->setText(query.value(2).toString());
        editHeaddress->setText(query.value(3).toString());
        editWinClothes->setText(query.value(4).toString());
        editSumClothes->setText(query.value(5).toString());
        editWinShoes->setText(query.value(6).toString());
        editSumShoes->setText(query.value(7).toString());
        newRecord = false;
    }else{
        newRecord = true;
    }

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(growthLayout);
    leftLayout->addWidget(clothes);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addLayout(headdressLayout);
    rightLayout->addWidget(shoes);

    QHBoxLayout *allLayout = new QHBoxLayout;
    allLayout->addLayout(leftLayout);
    allLayout->addLayout(rightLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(allLayout);
    if(!onlyForRead){
        mainLayout->addWidget(buttonBox);
    }

    setLayout(mainLayout);

    setWindowTitle(trUtf8("Размеры"));
}

void OveralForm::editRecord()
{

    QTextStream stream(&exchangeFile);
    QString line;
    while(!stream.atEnd()){
        stream.readLine();
    }
    if(!newRecord){
        QSqlQuery query;
        query.prepare("UPDATE overal SET "
                      "employeeid = :employeeid, "
                      "growth = :growth, "
                      "headdress = :headdress, "
                      "winclothes = :winclothes, "
                      "sumclothes = :sumclothes, "
                      "winshoes = :winshoes, "
                      "sumshoes = :sumshoes "
                      "WHERE  overalid = :id");
        query.bindValue(":id",indexTemp);
        query.bindValue(":employeeid",indexTemp);
        query.bindValue(":growth",editGrowth->text());
        query.bindValue(":headdress",editHeaddress->text());
        query.bindValue(":winclothes",editWinClothes->text());
        query.bindValue(":sumclothes",editSumClothes->text());
        query.bindValue(":winshoes",editWinShoes->text());
        query.bindValue(":sumshoes",editSumShoes->text());
        query.exec();
        if(!query.isActive()){
            QMessageBox::warning(this,QObject::trUtf8("Ошибка базы данных! UPDATE overal."),query.lastError().text());
            return;
        }
        line += "UPDATE overal SET employeeid = '";
        line += indexTemp.toUtf8();
        line += "', growth = '";
        line += editGrowth->text().toUtf8();
        line += "', headdress = '";
        line += editHeaddress->text().toUtf8();
        line += "', winclothes = '";
        line += editWinClothes->text().toUtf8();
        line += "', sumclothes = '";
        line += editSumClothes->text().toUtf8();
        line += "', winshoes = '";
        line += editWinShoes->text().toUtf8();
        line += "', sumshoes = '";
        line += editSumShoes->text().toUtf8();
        line += "' WHERE  overalid = '";
        line += indexTemp.toUtf8();
        line += "'";
        line += "\r\n";
        stream<<line;
    }else{
        QSqlQuery query;
        query.prepare("INSERT INTO overal (overalid, "
                      "employeeid, "
                      "growth, "
                      "headdress, "
                      "winclothes, "
                      "sumclothes, "
                      "winshoes, "
                      "sumshoes) VALUES("
                      ":overalid, :employeeid,"
                      ":growth, :headdress, :winclothes, "
                      ":sumclothes, :winshoes, :sumshoes"
                      ")");
        query.bindValue(":overalid",indexTemp);
        query.bindValue(":employeeid",indexTemp);
        query.bindValue(":growth",editGrowth->text().toUtf8());
        query.bindValue(":headdress",editHeaddress->text().toUtf8());
        query.bindValue(":winclothes",editWinClothes->text().toUtf8());
        query.bindValue(":sumclothes",editSumClothes->text().toUtf8());
        query.bindValue(":winshoes",editWinShoes->text().toUtf8());
        query.bindValue(":sumshoes",editSumShoes->text().toUtf8());
        query.exec();
        if(!query.isActive()){
            QMessageBox::warning(this,QObject::trUtf8("Ошибка базы данных! INSERT INTO overal."),query.lastError().text());
            return;
        }
        line += "INSERT INTO overal (overalid, employeeid, growth, headdress, winclothes, "
                "sumclothes, winshoes, sumshoes) VALUES('";
        line += indexTemp.toUtf8();
        line += "', '";
        line += indexTemp.toUtf8();
        line += "', '";
        line += editGrowth->text().toUtf8();
        line += "', '";
        line += editHeaddress->text().toUtf8();
        line += "', '";
        line += editWinClothes->text().toUtf8();
        line += "', '";
        line += editSumClothes->text().toUtf8();
        line += "', '";
        line += editWinShoes->text().toUtf8();
        line += "', '";
        line += editSumShoes->text().toUtf8();
        line += "')";
        line += "\r\n";
        stream<<line;
    }
    emit accept();
    close();
}

void OveralForm::deleteRecord()
{
    ForDelete forDelete(indexTemp,"overal",this);
    forDelete.exec();
    int k = QMessageBox::warning(this,trUtf8("Внимание!!!"),trUtf8("Действительно удалить?"),
                                 QMessageBox::No|QMessageBox::Yes,QMessageBox::No);
    if(k == QMessageBox::Yes){
        forDelete.deleteOnDefault();

        QSqlQuery query;
        query.prepare("DELETE FROM overal WHERE postid = :id");
        query.bindValue(":id",indexTemp);
        query.exec();
        query.next();

    }

}

void OveralForm::done(int result)
{
    QDialog::done(result);
}
