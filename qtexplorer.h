#ifndef QTEXPLORER_H
#define QTEXPLORER_H

#include <QMainWindow>

#include <QTableView>
#include <QPlainTextEdit>
#include <QLine>
#include <QSplitter>
#include <QVBoxLayout>
#include <QDir>
#include <QStack>

QT_BEGIN_NAMESPACE
namespace Ui {
class QtExplorer;
}
QT_END_NAMESPACE

class QtExplorer : public QMainWindow
{
    Q_OBJECT

public:
    explicit QtExplorer(QWidget *parent = nullptr);
    ~QtExplorer() override;

private:
    bool updateFilesLayer(const QString& newPath = "");
    void addButtonToVBoxLayout(unsigned int value);
    void clearFilter();
    void updateVisualButton();

    Ui::QtExplorer *ui = nullptr;

    QVBoxLayout* vBoxLayout = nullptr;
    const QString DEFAULT_STYLE_BUTTON = "text-align:left;";
    const QString STYLE_BUTTON_FOLDER = DEFAULT_STYLE_BUTTON + "background-color: #F6D66C";
    const QString STYLE_BUTTON_FILE = DEFAULT_STYLE_BUTTON + "background-color: #FFFFFF";

    QPalette FOLDER_PALETTE;
    QPalette FILE_PALETTE;

    const QString DEFAULT_PATH = QDir::homePath();
    QString path = "";

    QString filter = "";

    unsigned int numberLine = 0;
    unsigned int numberUsedLine = 0;
    const unsigned int DEFAULT_NUMBERLINE = 30;

    QStack<QString> forward;
    QStack<QString> backward;

    const char* PROPERTY_FILE_PATH = "filePath";


private slots:
    void onLineEditChanged();
    void onFilterTyping(const QString& text);
    void onButtonClicked(const QString& newPath);
    void showContextMenu(const QPoint &pos);
    void showMainContextMenu(const QPoint &pos);
    void onBackward();
    void onForward();
    void onReset();


};
#endif // QTEXPLORER_H
