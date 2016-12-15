#include "programmsettings.h"
#include <QtWidgets>

ProgrammSettings::ProgrammSettings(QWidget *parent) : QDialog(parent)
{
    nameFont = new QLabel(trUtf8("Font:"));
    editFont = new QComboBox;
    editFont->addItems(QStringList() << "8" << "9" << "10" << "11" << "12");
    nameFont->setBuddy(editFont);
    connect(editFont,SIGNAL(currentIndexChanged(QString)),this,SLOT(fontSet()));

    savePushButton = new QPushButton(trUtf8("Save"));
    connect(savePushButton,SIGNAL(clicked()),this,SLOT(editRecord()));
    savePushButton->setToolTip(trUtf8("Save And Close Button"));

    cancelPushButton = new QPushButton(trUtf8("Cancel"));
    cancelPushButton->setDefault(true);
    cancelPushButton->setStyleSheet("QPushButton:hover {color: red}");
    connect(cancelPushButton,SIGNAL(clicked()),this,SLOT(accept()));
    cancelPushButton->setToolTip(trUtf8("Cancel Button"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(cancelPushButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(savePushButton,QDialogButtonBox::ActionRole);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(nameFont,0,0);
    mainLayout->addWidget(editFont,0,1);
    //if(!onlyForRead){
        mainLayout->addWidget(buttonBox,1,1);
    //    editName->selectAll();
    //}

    setLayout(mainLayout);


//    setWindowTitle(trUtf8("Programm Settings"));
//    QFont font("Arial",12,QFont::Bold);
//    QApplication::setFont(font);
}

void ProgrammSettings::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    settings.setValue("NashaFirmaForm", saveGeometry());
    //settings.setValue("Font","hhh");
}

void ProgrammSettings::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    restoreGeometry(settings.value("NashaFirmaForm").toByteArray());
}

void ProgrammSettings::fontSet()
{
    int tt = editFont->currentText().toInt();
    QFont font("Arial",tt,QFont::Normal);
    QApplication::setFont(font);
}
