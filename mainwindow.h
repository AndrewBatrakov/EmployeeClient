#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QSqlRelationalTableModel>

class MainWindow : public QMainWindow
{
    Q_OBJECT   
public:
    MainWindow();
    ~MainWindow();

private slots:
    void createRightPanel();
    void writeSettings();
    void readSettings();
    void createActions();
    void createMenus();
    void createContextMenu();
    void aboutProgProcedure();
    void editRecordOfTable();
    void addRecordOfTable();
    void deleteRecordOfTable();
    void copyRecordOfTable();
    void programmSettings();
    void importExcelOstatki();

    void prefixProcedure();
    void getXMLProcedure();
    void sqlQuery();

    void viewOrganization();
    void viewSubdivision();
    void viewPost();
    void viewEmployee();
    void viewContract();
    void viewGrafikRabot();
    void viewSecurityProduction();
    void viewSafetyRequirements();
    void viewTestTask();
    void viewLegalActs();
    void viewSRP();
    void viewVenue();
    void viewTrainingSafety();
    void viewLaProductionFactor();
    void viewLaNormLink();
    void viewLaTerm();
    void viewLaClass();
    void viewLaSpecial();
    void viewEmpCert();
    void viewSiz();
    void viewHar();
    void viewPostSiz();
    void viewSizNaim();
    void viewPrichinaObuch();
    void viewVidAtestLul();
    void viewObosobl();
    void viewTypeDoc();
    void viewObuchLul();
    void viewObuchVis();
    void viewObuchPtm();
    void viewScore();
    void viewNomen();
    void viewKomissiya();

    //Охрана труда
    void viewStagirovka();

    void reportGrafikPromBez();
    void reportAllEmployeeSiz();
    void reportStatRazmerov();
    void reportSrokNoski();

    void ftpForm();
    void getPhotoProcedure();
    void putProcedure();
    void getProcedure();
    void vaccumProcedure();
    void importXmlTable();
    void smtpProcedure();
    void removeFiles();

    void viewTemplateTable(QString);
    void sortTable(int index);
    void updateRightPanel(QModelIndex);
    void searchProcedure();
    void splashScreen();
    void hideStatusBar();

private:
    QWidget *leftPanel;
    QWidget *rightPanel;

    QTableView *tableView;
    QLabel *tableLabel;
    QSqlRelationalTableModel *templateModel;
    QSqlQueryModel *queryModel;

    QPushButton *addRecordTable;
    QPushButton *editRecordTable;
    QPushButton *deleteRecordTable;

    QMenu *fileMenu;
    QAction *prefixAction;
    QMenu *importMenu;
    QAction *importExcelAction;
    QAction *getXMLAction;
    QAction *getPhotoAction;
    QAction *importXmlTableAction;
    QAction *exitAction;

    QMenu *referenceMenu;
    QAction *employeeAction;
    QAction *organizationAction;
    QAction *subdivisionAction;
    QAction *postAction;
    QAction *grafikRabot;
    QAction *prichinaObuchAction;
    QAction *vidAtestLulAction;
    QAction *typeDocAction;
    QAction *obosoblAction;
    QAction *komissiyaAction;
    QMenu *sizMenu;
    QAction *harAction;
    QAction *postSizAction;
    QAction *sizAction;
    QAction *postSizTableAction;
    QAction *sizNaimAction;
    QAction *nomenAction;
    QMenu *securityProductionMenu;
    QAction *securityProductionAction;
    QAction *safetyRequirements;
    QAction *testTask;
    QAction *legalActs;
    QAction *venueAction;
    QMenu *laboratoriyaRef;
    QAction *laProductionFactors;
    QAction *laNormLink;
    QAction *laTerm;
    QAction *laClass;
    QAction *laSpecial;

    QMenu *documentMenu;
    QMenu *trafficSecurityMenu;
    QAction *contractAction;
    QAction *internshipAction;
    QAction *trafficInfractionAction;
    QAction *accidentsAction;
    QAction *preventiveWorkAction;
    QAction *medicalExaminationAction;
    QMenu *securityProduction;
    QAction *srpAction;
    QAction *trainingSafetyAction;
    QAction *employeeCertificationAction;
    QMenu *laboratoriyaDoc;
    QAction *laProtocol;
    QMenu *obuchDoc;
    QAction *obuchLulAction;
    QAction *obuchVisAction;
    QAction *obuchPtmAction;
    QMenu *scoreMenu;
    QAction *scoreAction;
    QMenu *ohranaTrudaMenu;
    QAction *stagirovkaAction;

    QMenu *reportMenu;
    QAction *grafikPromBez;
    QAction *allEmployeeSiz;
    QMenu *reportStatistic;
    QAction *razmeriAction;
    QAction *srokNoskiAction;

    QMenu *serviceMenu;
    QAction *programmSettingsAction;
    QAction *ftpAction;
    QAction *searchAction;
    QAction *putAction;
    QAction *getAction;
    QAction *vaccumAction;
    QAction *smtpAction;
    QAction *sqlAction;

    QMenu *helpMenu;
    QAction *aboutQtAction;
    QAction *aboutProgAction;

    //Context Menu
    QAction *addAction;
    QAction *deleteAction;
    QAction *editAction;
    QAction *copyAction;

    QString filterTable;
    bool setFilter;

    QLabel *searchString;
    QFile fileExchange;

protected:
    void closeEvent(QCloseEvent *);
    void keyReleaseEvent(QKeyEvent *);
};

#endif // MAINWINDOW_H
