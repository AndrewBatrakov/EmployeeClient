#include "smtp.h"

Smtp::Smtp(QString host, QString username, QString pass)
{
    //smtpsocet = "";
}

void Smtp::disconnected()
{
    //QMessageBox::warning(this,tr("Disconnected!"),smtpsocet->errorString(),);
}

void Smtp::connected()
{
    output.append("connected");
}

void Smtp::ReadLiner()
{
    if(isConneccted){

    }
}

void Smtp::PutSendLine()
{

}

Smtp::~Smtp()
{

}
