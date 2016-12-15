#include <QTranslator>
#include <cstdlib>
#include "employee.h"
#include "mainwindow.h"
#include "update.h"
#include "registration.h"
#include "uploadbase.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QSystemSemaphore semaphore("EmployeeClient", 1);  // создаём семафор
    semaphore.acquire(); // Поднимаем семафор, запрещая другим экземплрам работать с разделяемой памятью

#ifndef Q_OS_WIN32
    // в linux/unix разделяемая память не особождается при аварийном завершении приложения,
    // поэтому необходимо избавиться от данного мусора
    QSharedMemory nix_fix_shared_memory("Employee");
    if(nix_fix_shared_memory.attach()){
        nix_fix_shared_memory.detach();
    }
#endif

    QSharedMemory sharedMemory("Employee");  // Создаём экземпляр разделяемой памяти
    bool is_running;            // переменную для проверки ууже запущенного приложения
    if (sharedMemory.attach()){ // пытаемся присоединить экземпляр разделяемой памяти
        // к уже существующему сегменту
        is_running = true;      // Если успешно, то определяем, что уже есть запущенный экземпляр
    }else{
        sharedMemory.create(1); // В противном случае выделяем 1 байти памяти
        is_running = false;     // И определяем, что других экземпляров не запущено
    }
    semaphore.release();        // Опускаем семафор

    // Если уже запущен один экземпляр приложения, то сообщаем ою этом пользователю
    // и завершаем работу текущего экземпляра приложения
    if(is_running){
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(QObject::trUtf8("Приложение уже запущено.\n"
                                       "Вы можете запустить только один экземпляр приложения."));
        msgBox.exec();
        return 1;
    }

    //    int fontId = QFontDatabase::addApplicationFont("./AO1.ttf");
    //    QSettings settings("AO_Batrakov_Inc.", "EmployeeClient");
    //    settings.setValue("Font", fontId);

    QFile file;
    file.setFileName("CE_SQLite.arh");
    file.remove();
    file.setFileName("CE_SQLite.dat");
    if(file.isOpen()){
        file.close();
    }
    file.setPermissions(QFile::ReadOwner | QFile::WriteOwner);
    file.remove();

    UpLoadBase upLoad;
    upLoad.exeVersion();

    Registration registrationForm;
    if(!registrationForm.exec()){
        return 0;
    }

    Update update;
    update.iniVersion();

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/client.png"));
    splash->show();

    MainWindow mW;
    mW.show();
    splash->finish(&mW);

    return a.exec();
}
