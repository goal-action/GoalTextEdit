#include "GoalTextEdit.h"

GoalTextEdit::GoalTextEdit(QWidget *parent)
    : QWidget(parent)
{
    m_pMainMenu         = new GTMainMenu(this);
    m_pStackedTextEdits = new GTStackedTextEdits(this);
    m_pSideBar          = new GTSideBar(this);
    m_pStatusBar        = new GTStatusBar(this);
    m_pProject          = nullptr;

    QSplitter* splitter = new QSplitter;
    splitter->addWidget(m_pStackedTextEdits);
    splitter->addWidget(m_pSideBar);
    splitter->setContentsMargins(5, 0, 5 ,0);
    QList<int> sizes = {width() / 100 * 75, width() / 100 * 25};
    splitter->setSizes(sizes);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(m_pMainMenu, 1, Qt::AlignTop);
    layout->addWidget(splitter, Qt::AlignTop);
    layout->addWidget(m_pStatusBar);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    setWindowTitle("GoalText Editor");

    connect(m_pMainMenu, SIGNAL(newProjectPressedSignal()), this, SLOT(newProjectSlot()));
    connect(m_pMainMenu, SIGNAL(openProjectPressedSignal()), this, SLOT(openProjectSlot()));
    connect(m_pMainMenu, SIGNAL(newFilePressedSignal()), this, SLOT(newFileSlot()));
    connect(m_pMainMenu, SIGNAL(closeProjectPressedSignal()), this, SLOT(closeProjectSlot()));
    connect(m_pMainMenu, SIGNAL(savefilePressedSignal()), this, SLOT(saveCurrentFileSlot()));
    connect(m_pMainMenu, SIGNAL(saveProjectPressedSignal()), this, SLOT(saveProjectSlot()));
    connect(m_pMainMenu, SIGNAL(deleteFilePressedSignal()), this, SLOT(deleteFileSlot()));
    connect(m_pMainMenu, SIGNAL(exitPressedSignal()), this, SLOT(exitSlot()));

    connect(m_pMainMenu, SIGNAL(runNowPressedSignal()), this, SLOT(runSlot()));

    connect(m_pMainMenu, SIGNAL(aboutPressedSignal()), this, SLOT(aboutSlot()));

    connect(m_pMainMenu, SIGNAL(leftSideBarPressedSignal()), this, SLOT(setLeftSideBarSlot()));
    connect(m_pMainMenu, SIGNAL(rightSideBarPressedSignal()), this, SLOT(setRightSideBarSlot()));
    connect(m_pMainMenu, SIGNAL(lightThemePressedSignal()), this, SLOT(setLightThemeSlot()));
    connect(m_pMainMenu, SIGNAL(darkThemePressedSignal()), this, SLOT(setDarkThemeSlot()));

    connect(this, SIGNAL(projectOpenedSignal(const QString&, QVector<GTFile*>*)), m_pStackedTextEdits, SLOT(openProjectSlot(const QString&, QVector<GTFile*>*)));
    connect(this, SIGNAL(projectOpenedSignal(const QString&, QVector<GTFile*>*)), m_pSideBar, SLOT(openProjectSlot(const QString&, QVector<GTFile*>*)));
    connect(this, SIGNAL(projectClosedSignal()), m_pStackedTextEdits, SLOT(closeProjectSlot()));
    connect(this, SIGNAL(projectClosedSignal()), m_pSideBar, SLOT(closeProjectSlot()));
    connect(this, SIGNAL(newFileSignal(GTFile*)), m_pSideBar, SLOT(newFileSlot(GTFile*)));
    connect(this, SIGNAL(newFileSignal(GTFile*)), m_pStackedTextEdits, SLOT(newFileSlot(GTFile*)));

    connect(m_pSideBar, SIGNAL(changedCurrentItemSignal(const QString&)), m_pStackedTextEdits, SLOT(setCurrentTextEditSlot(const QString&)));
    connect(m_pSideBar, SIGNAL(changedCurrentItemSignal(const QString&)), this, SLOT(newCurrentFileSlot(const QString&)));

    setLightThemeSlot();
    m_ntheme = LightTheme;
}


//createProject - создает файл main.cpp и создает программу Hello world
void GoalTextEdit::createProject(const QString& projectPath){
    QFile mainCpp(projectPath + "/main.cpp");
    if(mainCpp.open(QIODevice::WriteOnly)){
        QFile mainCppBlank(":/mainBlank/mainBlank.cpp");
        if(mainCppBlank.open(QIODevice::ReadOnly)){
            mainCpp.write(mainCppBlank.readAll());
        }
        mainCppBlank.close();
    }
    mainCpp.close();
}


//Слоты для работы с файловой системой
void GoalTextEdit::newProjectSlot(){
    m_pStatusBar->setStatus("Creating a new project...");
    //Проверяем сохранены ли файлы
    if(m_pProject){
        int numberOfFiles = m_pProject->getProjectFiles()->size();
        QVector<QString> namesOfUnSavedFiles;
        for(int i = 0; i < numberOfFiles; i++){
            if(!m_pProject->getProjectFiles()->at(i)->isSaved()){
                namesOfUnSavedFiles.push_back(m_pProject->getProjectFiles()->at(i)->getFileName());
            }
        }
        if(namesOfUnSavedFiles.size() > 0){
            CUnSavedFilesDialog* unSavedFilesDialog = new CUnSavedFilesDialog(namesOfUnSavedFiles);
            unSavedFilesDialog->exec();
            if(unSavedFilesDialog->getStatus() == CUnSavedFilesDialog::SAVEALL){
                saveProjectSlot();
            }else if(unSavedFilesDialog->getStatus() == CUnSavedFilesDialog::CANCEL){
                 return;
            }
            delete unSavedFilesDialog;
        }
    }
    //=============================

    CNewProjectDialog* newProjectDialog = new CNewProjectDialog;
    if(newProjectDialog->exec() == QDialog::Accepted){
        QString projectPath = newProjectDialog->getProjectPath();
        if(!projectPath.isEmpty()){
            QDir dir(projectPath);
            if(dir.exists()){
                QMessageBox messageBox;
                messageBox.critical(0,"Error","Project is already exists!");
            }else{
                if(dir.mkdir(projectPath)){
                    if(m_pProject){
                        delete m_pProject;
                        m_pProject = nullptr;
                        emit projectClosedSignal();
                    }
                    createProject(projectPath);
                    m_pProject = new GTProject(projectPath, m_ntheme);
                    if(m_ntheme == LightTheme)
                        m_pProject->setLightTheme();
                    else
                        m_pProject->setDarkTheme();
                    for(int i = 0; i < m_pProject->getProjectFiles()->size(); i++){
                        connect(m_pProject->getProjectFiles()->at(i), SIGNAL(fileIsNotSavedSignal()), this, SLOT(currentFileIsNotSavedSlot()));
                    }
                    setWindowTitle(m_pProject->getCurrentFile()->getFilePath());

                    //Открываем пункты меню
                    m_pMainMenu->setProjectAndFileExistActionsAvailable();
                    m_pMainMenu->setProjectExistActionsAvailable();

                    //Сигнализируем об открытие проекта
                    emit projectOpenedSignal(m_pProject->getProjectName(), m_pProject->getProjectFiles());
                }else{
                    QMessageBox messageBox;
                    messageBox.critical(0,"Error","The project was not created!");
                }
            }
        }else{
            QMessageBox messageBox;
            messageBox.critical(0,"Error","No path!");
        }
    }
    delete newProjectDialog;
    m_pStatusBar->setStatus("");
}

void GoalTextEdit::openProjectSlot(){
    m_pStatusBar->setStatus("Project is opening...");
    //Проверяем сохранены ли файлы
    if(m_pProject){
        int numberOfFiles = m_pProject->getProjectFiles()->size();
        QVector<QString> namesOfUnSavedFiles;
        for(int i = 0; i < numberOfFiles; i++){
            if(!m_pProject->getProjectFiles()->at(i)->isSaved()){
                namesOfUnSavedFiles.push_back(m_pProject->getProjectFiles()->at(i)->getFileName());
            }
        }
        if(namesOfUnSavedFiles.size() > 0){
            CUnSavedFilesDialog* unSavedFilesDialog = new CUnSavedFilesDialog(namesOfUnSavedFiles);
            unSavedFilesDialog->exec();
            if(unSavedFilesDialog->getStatus() == CUnSavedFilesDialog::SAVEALL){
                saveProjectSlot();
            }else if(unSavedFilesDialog->getStatus() == CUnSavedFilesDialog::CANCEL){
                 return;
            }
            delete unSavedFilesDialog;
        }
    }
    //=============================

    QString projectPath = QFileDialog::getExistingDirectory(0, "Open project");
    if(m_pProject){
        delete m_pProject;
        m_pProject = nullptr;
        emit projectClosedSignal();
    }
    m_pProject = new GTProject(projectPath, m_ntheme);
    if(m_ntheme == LightTheme)
        m_pProject->setLightTheme();
    else
        m_pProject->setDarkTheme();
    for(int i = 0; i < m_pProject->getProjectFiles()->size(); i++){
        connect(m_pProject->getProjectFiles()->at(i), SIGNAL(fileIsNotSavedSignal()), this, SLOT(currentFileIsNotSavedSlot()));
    }
    if(m_pProject->getCurrentFile()){
        setWindowTitle(m_pProject->getCurrentFile()->getFilePath());
    }else{
        setWindowTitle(m_pProject->getProjectPath());
    }
    //Открываем пункты меню
    if(m_pProject->countFiles() != 0)
        m_pMainMenu->setProjectAndFileExistActionsAvailable();
    m_pMainMenu->setProjectExistActionsAvailable();

    //Сигнализируем об открытие проекта
    emit projectOpenedSignal(m_pProject->getProjectName(), m_pProject->getProjectFiles());
    m_pStatusBar->setStatus("");
}

void GoalTextEdit::newFileSlot(){
    m_pStatusBar->setStatus("Creating a new file...");
    CNewFileDialog* newFileDialog = new CNewFileDialog;
    if(newFileDialog->exec() == QDialog::Accepted){
        QString nameOfNewFile = newFileDialog->getNameOfNewFile();
        if(nameOfNewFile == ".cpp" || nameOfNewFile == ".h"){
            QMessageBox messageBox;
            messageBox.critical(0,"Error","Invalid name of file!");
        }
        for(int i = 0; i < m_pProject->getProjectFiles()->size(); i++){
            if(m_pProject->getProjectFiles()->at(i)->getFileName() == nameOfNewFile){
                QMessageBox messageBox;
                messageBox.critical(0,"Error","The file is already exists!");
                return;
            }
        }
        m_pProject->addFile(new GTFile(m_pProject->getProjectPath() + '/' + nameOfNewFile, m_ntheme));
        int newFileIndex = m_pProject->getProjectFiles()->size() - 1;
        connect(m_pProject->getProjectFiles()->at(newFileIndex), SIGNAL(fileIsNotSavedSignal()), this, SLOT(currentFileIsNotSavedSlot()));
        emit newFileSignal(m_pProject->getProjectFiles()->at(newFileIndex));
        if(m_pProject->countFiles() == 1)//Если это первый файл в проекте,
            m_pMainMenu->setProjectAndFileExistActionsAvailable();//то открываем остальные пункты меню
    }
    delete newFileDialog;
    m_pStatusBar->setStatus("");
}

void GoalTextEdit::deleteFileSlot(){
    m_pStatusBar->setStatus("The file is being deleted...");
    m_pStackedTextEdits->removeCurrentTextEdit();
    m_pProject->deleteCurrentFile();
    m_pSideBar->deleteCurrentFile();
    if(!m_pProject->getCurrentFile())
        setWindowTitle(m_pProject->getProjectPath());
    if(m_pProject->countFiles() == 0)//Если файлов в проекте больше нет,
        m_pMainMenu->setProjectAndFileExistActionsUnAvailable();//то скрываем половину пунктов меню
    m_pStatusBar->setStatus("");
}

void GoalTextEdit::closeProjectSlot(){
    m_pStatusBar->setStatus("The project is being closed...");
    //Проверяем сохранены ли файлы
    if(m_pProject){
        int numberOfFiles = m_pProject->getProjectFiles()->size();
        QVector<QString> namesOfUnSavedFiles;
        for(int i = 0; i < numberOfFiles; i++){
            if(!m_pProject->getProjectFiles()->at(i)->isSaved()){
                namesOfUnSavedFiles.push_back(m_pProject->getProjectFiles()->at(i)->getFileName());
            }
        }
        if(namesOfUnSavedFiles.size() > 0){
            CUnSavedFilesDialog* unSavedFilesDialog = new CUnSavedFilesDialog(namesOfUnSavedFiles);
            unSavedFilesDialog->exec();
            if(unSavedFilesDialog->getStatus() == CUnSavedFilesDialog::SAVEALL){
                saveProjectSlot();
            }else if(unSavedFilesDialog->getStatus() == CUnSavedFilesDialog::CANCEL){
                 return;
            }
            delete unSavedFilesDialog;
        }
    }
    //=============================

    if(m_pProject){
        delete m_pProject;
        m_pProject = nullptr;
        setWindowTitle("GoalText Editor");
        emit projectClosedSignal();
    }
    //скрываем все пункты меню
    m_pMainMenu->setProjectAndFileExistActionsUnAvailable();
    m_pMainMenu->setProjectExistActionsUnAvailable();
    m_pStatusBar->setStatus("");
}

void GoalTextEdit::saveCurrentFileSlot(){
    m_pStatusBar->setStatus("The project is being closed...");
    m_pProject->getCurrentFile()->saveFile();
    setWindowTitle(m_pProject->getCurrentFile()->getFilePath());
    m_pStatusBar->setStatus("");
}

void GoalTextEdit::saveProjectSlot(){
    m_pStatusBar->setStatus("The project is being saved...");
    m_pProject->saveProject();
    setWindowTitle(m_pProject->getCurrentFile()->getFilePath());
    m_pStatusBar->setStatus("");
}

void GoalTextEdit::newCurrentFileSlot(const QString& fileName){
    for(int i = 0; i < m_pProject->getProjectFiles()->size(); i++){
        if(m_pProject->getProjectFiles()->at(i)->getFileName() == fileName){
            m_pProject->setCurrentFile(m_pProject->getProjectFiles()->at(i));
            if(m_pProject->getProjectFiles()->at(i)->isSaved())
                setWindowTitle(m_pProject->getProjectPath()+ '/' + fileName);
            else{
                setWindowTitle(m_pProject->getProjectPath()+ '/' + fileName + '*');
            }
            break;
        }
    }
}

void GoalTextEdit::currentFileIsNotSavedSlot(){
    setWindowTitle(m_pProject->getCurrentFile()->getFilePath() + "*");
}


//Слот для работы с компиляцией
void GoalTextEdit::runSlot(){
    m_pStatusBar->setStatus("The project is running");
    //Проверяем сохранены ли файлы
    if(m_pProject){
        int numberOfFiles = m_pProject->getProjectFiles()->size();
        QVector<QString> namesOfUnSavedFiles;
        for(int i = 0; i < numberOfFiles; i++){
            if(!m_pProject->getProjectFiles()->at(i)->isSaved()){
                namesOfUnSavedFiles.push_back(m_pProject->getProjectFiles()->at(i)->getFileName());
            }
        }
        if(namesOfUnSavedFiles.size() > 0){
            CUnSavedFilesDialog* unSavedFilesDialog = new CUnSavedFilesDialog(namesOfUnSavedFiles);
            unSavedFilesDialog->exec();
            if(unSavedFilesDialog->getStatus() == CUnSavedFilesDialog::SAVEALL){
                saveProjectSlot();
            }else if(unSavedFilesDialog->getStatus() == CUnSavedFilesDialog::CANCEL){
                 return;
            }
            delete unSavedFilesDialog;
        }
    }
    //=============================


    QString debugDirPath = m_pProject->getProjectPath() + "/debug";
    QDir debugDir(debugDirPath);
    if(!debugDir.exists())
        debugDir.mkdir(debugDirPath);

    QProcess proc;
    QStringList args;
    args << "-static";
    int numberOfFiles = m_pProject->getProjectFiles()->size();
    for(int i = 0; i < numberOfFiles; i++){
        args << m_pProject->getProjectFiles()->at(i)->getFilePath();
    }
    args << "-o";
    args << debugDirPath + "/" + m_pProject->getProjectName() + ".exe";
    proc.start("g++", args);
    proc.waitForReadyRead();
    QByteArray compiledErrors = proc.readAllStandardError();;
    if(!compiledErrors.isEmpty()){
        CCompileErrorsDialog* compileErrorsDialog = new CCompileErrorsDialog(compiledErrors);
        compileErrorsDialog->exec();
        proc.waitForFinished(-1);
        delete compileErrorsDialog;
    }
    else{
        proc.waitForFinished(-1);
        system(QString(debugDirPath+ "/" + m_pProject->getProjectName() + ".exe").toStdString().c_str());
    }
    m_pStatusBar->setStatus("");
}


//Слот для работы с AboutDialog
void GoalTextEdit::aboutSlot(){
    CAboutGoalTextEditorDialog* aboutGoalTextEditorDialog = new CAboutGoalTextEditorDialog;
    aboutGoalTextEditorDialog->exec();
    delete aboutGoalTextEditorDialog;
}


//Слоты для работы с внешним видом
void GoalTextEdit::setLeftSideBarSlot(){
    delete layout();
    QSplitter* splitter = new QSplitter;
    splitter->addWidget(m_pSideBar);
    splitter->addWidget(m_pStackedTextEdits);
    splitter->setContentsMargins(5, 0, 5 ,0);
    QList<int> sizes = {width() / 100 * 25, width() / 100 * 75};
    splitter->setSizes(sizes);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(m_pMainMenu, 1, Qt::AlignTop);
    layout->addWidget(splitter, Qt::AlignTop);
    layout->addWidget(m_pStatusBar);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
}

void GoalTextEdit::setRightSideBarSlot(){
    delete layout();
    QSplitter* splitter = new QSplitter;
    splitter->addWidget(m_pStackedTextEdits);
    splitter->addWidget(m_pSideBar);
    splitter->setContentsMargins(5, 0, 5 ,0);
    QList<int> sizes = {width() / 100 * 75, width() / 100 * 25};
    splitter->setSizes(sizes);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(m_pMainMenu, 1, Qt::AlignTop);
    layout->addWidget(splitter, Qt::AlignTop);
    layout->addWidget(m_pStatusBar);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
}

void GoalTextEdit::setLightThemeSlot(){
    if(m_pProject)
        m_pProject->setLightTheme();
    m_pMainMenu->setLightTheme();
    m_pStackedTextEdits->setLightTheme();
    m_pStatusBar->setLightTheme();
    m_pSideBar->setLightTheme();
    QFile file(":/styles/goalTextEdit/lightTheme/textEdit.css");
    file.open(QIODevice::ReadOnly);
    setStyleSheet(file.readAll());
    file.close();
    if(m_pProject)
        m_pProject->setLightHighlighter();

    m_ntheme = LightTheme;
}

void GoalTextEdit::setDarkThemeSlot(){
    if(m_pProject)
        m_pProject->setDarkTheme();
    m_pMainMenu->setDarkTheme();
    m_pStackedTextEdits->setDarkTheme();
    m_pStatusBar->setDarkTheme();
    m_pSideBar->setDarkTheme();
    QFile file(":/styles/goalTextEdit/darkTheme/textEdit.css");
    file.open(QIODevice::ReadOnly);
    setStyleSheet(file.readAll());
    file.close();
    if(m_pProject)
        m_pProject->setDarkHighlighter();

    m_ntheme = DarkTheme;
}

//Слот для выхода из программы
void GoalTextEdit::exitSlot(){
    m_pStatusBar->setStatus("Exiting...");
    //Проверяем сохранены ли файлы
    if(m_pProject){
        int numberOfFiles = m_pProject->getProjectFiles()->size();
        QVector<QString> namesOfUnSavedFiles;
        for(int i = 0; i < numberOfFiles; i++){
            if(!m_pProject->getProjectFiles()->at(i)->isSaved()){
                namesOfUnSavedFiles.push_back(m_pProject->getProjectFiles()->at(i)->getFileName());
            }
        }
        if(namesOfUnSavedFiles.size() > 0){
            CUnSavedFilesDialog* unSavedFilesDialog = new CUnSavedFilesDialog(namesOfUnSavedFiles);
            unSavedFilesDialog->exec();
            if(unSavedFilesDialog->getStatus() == CUnSavedFilesDialog::SAVEALL){
                saveProjectSlot();
            }else if(unSavedFilesDialog->getStatus() == CUnSavedFilesDialog::CANCEL){
                 return;
            }
            delete unSavedFilesDialog;
        }
    }
    //=============================
    qApp->quit();
}


//Деструктор
GoalTextEdit::~GoalTextEdit()
{

}

