#include "filewidget.h"

FileWidget::FileWidget(QWidget *parent)
    : QWidget{parent}
{

    labelFileName = new QLabel(this);
    labelDate = new QLabel(this);
    labelSize = new QLabel(this);
    labelIcon = new QLabel(this);

    layout = new QVBoxLayout(this);

    layout->addWidget(labelIcon);
    layout->addWidget(labelFileName);
    layout->addWidget(labelSize);
    layout->addWidget(labelDate);


    labelIcon->hide();

    setLayout(layout);
}

FileWidget::FileWidget(const QFileInfo& fileInfo, QWidget *parent)
    : QWidget{parent}
{

    labelFileName = new QLabel(this);
    labelDate = new QLabel(this);
    labelSize = new QLabel(this);
    labelIcon = new QLabel(this);

    layout = new QVBoxLayout(this);

    layout->addWidget(labelIcon);
    layout->addWidget(labelFileName);
    layout->addWidget(labelSize);
    layout->addWidget(labelDate);

    setLayout(layout);
}

void FileWidget::setFileInfo(const QFileInfo& pFileInfo) {
    fileInfo = pFileInfo;
    labelFileName->setText(fileInfo.fileName());
    if(fileInfo.isFile()) {
        labelSize->setText(octet_to_string(fileInfo.size()));
        labelSize->show();
    }
    else {
        labelSize->hide();
    }
    labelDate->setText(fileInfo.lastModified().toString("MM/dd/yyyy HH:mm"));

    update();
}

QString FileWidget::octet_to_string(qint64 octet) const {
    int count = 0;
    const QString tab[5] { "octet", "Ko", "Mo", "Go", "To"};

    for(int i=0; i<4; i++) {
        if(octet >= 1000) {
            octet /= 1000;
            count++;
        }
        else {
            break;
        }
    }

    return QString::number(octet) + " " + tab[count];
}

void FileWidget::mouseReleaseEvent(QMouseEvent* event) {
    if(event->button() == Qt::LeftButton ) {
        emit released(fileInfo.absoluteFilePath());
    }
    QWidget::mouseReleaseEvent(event);
}