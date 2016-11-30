#include "programmsettings.h"
#include <QtWidgets>

ProgrammSettings::ProgrammSettings(QWidget *parent) : QDialog(parent)
{
    nameFont = new QLabel(tr("Font:"));
    editFont = new QComboBox;
    editFont->addItems(QStringList() << "8" << "9" << "10" << "11" << "12");
    nameFont->setBuddy(editFont);
    connect(editFont,SIGNAL(currentIndexChanged(QString)),this,SLOT(fontSet()));

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

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(nameFont,0,0);
    mainLayout->addWidget(editFont,0,1);
    //if(!onlyForRead){
        mainLayout->addWidget(buttonBox,1,1);
    //    editName->selectAll();
    //}

    setLayout(mainLayout);


//    setWindowTitle(tr("Programm Settings"));
//    QFont font("Arial",12,QFont::Bold);
//    QApplication::setFont(font);
}

void ProgrammSettings::writeSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "ProgrammSettings");
    settings.setValue("Geometry", saveGeometry());
    //settings.setValue("Font","hhh");
}

void ProgrammSettings::readSettings()
{
    QSettings settings("AO_Batrakov_Inc.", "ProgrammSettings");
    restoreGeometry(settings.value("Geometry").toByteArray());
}

void ProgrammSettings::fontSet()
{
    int tt = editFont->currentText().toInt();
    QFont font("Arial",tt,QFont::Normal);
    QApplication::setFont(font);
}
