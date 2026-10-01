#ifndef FILEWIDGET_H
#define FILEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QFileInfo>
#include <QMouseEvent>
#include <QVBoxLayout>

class FileWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FileWidget(QWidget *parent = nullptr);
    explicit FileWidget(const QFileInfo& fileInfo, QWidget *parent = nullptr);
    ~FileWidget() override = default;

    QString getFileName() const {return labelFileName->text();}
    QString getFileDate() const {return labelDate->text();}
    QString getFileSize() const {return labelSize->text();}
    QString getFileIcon() const {return labelIcon->text();}
    QString getAbsoluteFilePath() const {return fileInfo.absoluteFilePath();}

    void setFileInfo(const QFileInfo& pFileInfo);

protected:
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    QLabel* labelFileName;
    QLabel* labelDate;
    QLabel* labelSize;
    QLabel* labelIcon;

    QVBoxLayout* layout;

    QFileInfo fileInfo;

    QString octet_to_string(qint64 octet) const;

signals:
    void released(const QString &filePath);
};

#endif // FILEWIDGET_H
