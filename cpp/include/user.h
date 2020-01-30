#pragma once

#include <QObject>
#include <QString>
#include <QImage>

class User : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)

public:
    explicit User(QObject *parent = nullptr);

    QString getName() const;
    void setName(const QString &userName);
    Q_INVOKABLE QString generateQR() const;
    Q_INVOKABLE void print() const;

signals:
    void nameChanged();

private:
    static void paintQR(QPainter &painter, const QSize sz, const QString &data, QColor fg);
    QImage getQRImage() const;

private:
    QString m_name;
};
