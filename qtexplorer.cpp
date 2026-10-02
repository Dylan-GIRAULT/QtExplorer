#include "qtexplorer.h"
#include "ui_qtexplorer.h"
#include "filewidget.h"
#include <QFileInfo>
#include <QMessageBox>
#include <QToolButton>
#include <QDesktopServices>
#include <QInputDialog>

QtExplorer::QtExplorer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QtExplorer)
{
    ui->setupUi(this);
    ui->FileWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->FileWidget, &QWidget::customContextMenuRequested, this, &QtExplorer::showMainContextMenu);

    FOLDER_PALETTE.setColor(QPalette::Window, QColor(0xf6, 0xd6, 0x6c));
    FILE_PALETTE.setColor(QPalette::Window, QColor(0xff, 0xff, 0xff));

    vBoxLayout = new QVBoxLayout(ui->FileWidget);

    addButtonToVBoxLayout(DEFAULT_NUMBERLINE);

    setCentralWidget(ui->centralwidget);

    connect(ui->pathLineEdit, &QLineEdit::editingFinished, this, &QtExplorer::onLineEditChanged);
    connect(ui->filterLineEdit, &QLineEdit::textEdited, this, &QtExplorer::onFilterTyping);

    connect(ui->backButton, &QToolButton::released, this, &QtExplorer::onBackward);
    connect(ui->frontButton, &QToolButton::released, this, &QtExplorer::onForward);
    connect(ui->resetButton, &QToolButton::released, this, &QtExplorer::onReset);

    ui->pathLineEdit->setText(DEFAULT_PATH);
    path = DEFAULT_PATH;
    updateVisualButton();
    updateFilesLayer();

}

void QtExplorer::onFilterTyping(const QString& text) {
    filter = text;
    updateFilesLayer();
}

void QtExplorer::onButtonClicked(const QString& newPath) {
    QString oldPath = path;
    if(updateFilesLayer(newPath)) {
        backward.push(oldPath);
        forward.clear();
        updateVisualButton();
    }

}

void QtExplorer::onLineEditChanged() {
    const QString text = ui->pathLineEdit->text();

    QString old_path = path;
    if(updateFilesLayer(text)) {
        backward.push(old_path);
        forward.clear();
        updateVisualButton();
    }

}

bool QtExplorer::updateFilesLayer(const QString& p_newPath) {
    QString newPath = p_newPath;

    if(newPath == "") {
        newPath = path;
    }
    else {
        clearFilter();
    }

    QFileInfo directory(newPath);

    if(!directory.exists()) {
        QMessageBox::warning(this, "Warning", "The Path does not exist : " + newPath);
        ui->pathLineEdit->setText(path);
        return false;
    }
    else if(directory.isFile()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(directory.absoluteFilePath()));
        ui->pathLineEdit->setText(path);
        return false;
    }

    QStringList filterNames;
    if(filter != ""){
        filterNames.append("*" + filter + "*");
    }
    else {
        filterNames.append("*");
    }

    QDir dir(directory.absoluteFilePath());

    QStringList list = dir.entryList(filterNames, QDir::AllEntries | QDir::NoDot, QDir::Time | QDir::DirsFirst);

    if(list.size() > numberLine) {
        addButtonToVBoxLayout(list.size() + 5 - numberLine);
    }

    for(int i=0; i<list.size() || i<numberUsedLine; i++) {
        FileWidget* fileWidget = qobject_cast<FileWidget*>(vBoxLayout->itemAt(i)->widget());

        if(i < list.size()) {
            QFileInfo fileInfo(dir.absoluteFilePath(list[i]));

            if(fileInfo.isFile()) {
                fileWidget->setPalette(FILE_PALETTE);
            }
            else if(fileInfo.isDir()) {
                fileWidget->setPalette(FOLDER_PALETTE);
            }
            fileWidget->setFileInfo(fileInfo);
            fileWidget->show();
        }
        else {
            fileWidget->hide();
        }
    }

    numberUsedLine = list.size();

    path = newPath;
    ui->pathLineEdit->setText(path);
    return true;

}

void QtExplorer::addButtonToVBoxLayout(unsigned int value) {

    for(int i=0; i<value; i++) {
        FileWidget* fileWidget = new FileWidget();
        vBoxLayout->addWidget(fileWidget);
        fileWidget->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Maximum);
        fileWidget->setStyleSheet(DEFAULT_STYLE_BUTTON);

        fileWidget->setAutoFillBackground(true);
        fileWidget->setContextMenuPolicy(Qt::CustomContextMenu);

        connect(fileWidget, &FileWidget::customContextMenuRequested, this, &QtExplorer::showContextMenu);
        connect(fileWidget, &FileWidget::released, this, &QtExplorer::onButtonClicked);
        fileWidget->hide();
    }

    numberLine += value;

}

void QtExplorer::showContextMenu(const QPoint &pos) {
    FileWidget* fileWidget = (FileWidget*)sender();

    QString filePath = fileWidget->getAbsoluteFilePath();

    QMenu menu(this);

    QAction* openAction = menu.addAction("&Open");
    QAction* renameAction = menu.addAction("&Rename");
    menu.addSeparator();
    QAction* deleteAction = menu.addAction("&Delete");


    QAction* selectedAction = menu.exec(
        fileWidget->mapToGlobal(pos)
    );


    QFileInfo fileInfo(filePath);

    if (selectedAction == openAction) {
        updateFilesLayer();
    }
    else if (selectedAction == renameAction) {
        // Renommer
        bool ok = false;

        while(!ok) {
            QString newName = QInputDialog::getText(this, tr("QtExplorer"), tr("Write a new name : "), QLineEdit::Normal, fileInfo.fileName(), &ok);
            if(!ok) {
                break;
            }
            if(fileInfo.fileName() != newName) {
                ok = QFile::rename(filePath, fileInfo.dir().absoluteFilePath(newName));
                if(!ok) {
                    QMessageBox::warning(this, "Warning", "Can't use this name (already use)");
                    continue;
                }
            }
        }
        updateFilesLayer();

    }
    else if (selectedAction == deleteAction) {
        QMessageBox::StandardButton ret = QMessageBox::warning(
            this,
            tr("QtExplorer"),
            "Are you sure to delete \"" + fileInfo.fileName() + "\" ?",
            QMessageBox::Yes | QMessageBox::No
        );
        if(ret == QMessageBox::Yes) {
            bool ok = false;

            if(fileInfo.isFile()) {
                ok = QFile::remove(filePath);
            }
            else if(fileInfo.isDir()) {
                ok = QDir(filePath).removeRecursively();
            }

            if(!ok) {
                QMessageBox::warning(this, "Warning", "Can't delete the file");
            }
        }
        updateFilesLayer();
    }

}

void QtExplorer::clearFilter() {
    filter = "";
    ui->filterLineEdit->setText(filter);
}

void QtExplorer::onBackward() {
    QString old_path = path;
    while(!backward.empty()) {
        if(updateFilesLayer(backward.pop())) {
            forward.push(old_path);
            break;
        }
    }

    updateVisualButton();
}

void QtExplorer::onForward() {
    QString old_path = path;
    while(!forward.empty()) {
        if(updateFilesLayer(forward.pop())) {
            backward.push(old_path);
            break;
        }
    }

    updateVisualButton();
}

void QtExplorer::onReset() {
    updateFilesLayer(path);
}

void QtExplorer::updateVisualButton() {
    if(forward.empty()) {
        ui->frontButton->setEnabled(false);
    }
    else {
        ui->frontButton->setEnabled(true);
    }

    if(backward.empty()) {
        ui->backButton->setEnabled(false);
    }
    else {
        ui->backButton->setEnabled(true);
    }
}

void QtExplorer::showMainContextMenu(const QPoint& pos) {
    QMenu menu(this);

    QMenu* menuCreate = menu.addMenu("Create");
    QAction* createFileAction = menuCreate->addAction("&File");
    QAction* createFolderAction = menuCreate->addAction("&Folder");

    QAction* openAction = menu.addAction("&Open in Another file explorer");


    QAction* selectedAction = menu.exec(mapToGlobal(pos));

    if(selectedAction == openAction) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    }
    else if(selectedAction == createFileAction) {
        bool ok = false;

        while(!ok) {
            QString name = QInputDialog::getText(this, tr("QtExplorer"), tr("Write the name of the file : "), QLineEdit::Normal, "file.txt", &ok);
            if(!ok) {
                QMessageBox::warning(this, "Warning", "Name is invalid");
                continue;
            }

            QFile file(QDir(path).absoluteFilePath(name));
            ok = file.open(QIODevice::ReadWrite);

            if(!ok) {
                QMessageBox::warning(this, "Warning", "Can't use this name (already use)");
                continue;
            }
        }
        updateFilesLayer();
    }
    else if(selectedAction == createFolderAction) {
        bool ok = false;

        while(!ok) {
            QString name = QInputDialog::getText(this, tr("QtExplorer"), tr("Write the name of the folder : "), QLineEdit::Normal, "folder", &ok);
            if(!ok) {
                QMessageBox::warning(this, "Warning", "Name is invalid");
                continue;
            }

            ok = QDir().mkdir(QDir(path).absoluteFilePath(name));

            if(!ok) {
                QMessageBox::warning(this, "Warning", "Can't use this name (already use)");
                continue;
            }
        }
        updateFilesLayer();
    }

}

QtExplorer::~QtExplorer()
{
    delete ui;

    for(int i=0; i<numberLine; i++) {
        FileWidget* fileWidget = qobject_cast<FileWidget*>(vBoxLayout->itemAt(i)->widget());
        delete fileWidget;
    }
    delete vBoxLayout;
}
