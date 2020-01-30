#include "user.h"

#include "QR-Code-generator/cpp/QrCode.hpp"
#include <QPainter>
#include <QBuffer>
#include <QByteArray>
#include <QPrinter>
#include <QPrintDialog>
#include <QPrinterInfo>
#include <QPrintPreviewDialog>

#include <QDebug>

namespace  {
    constexpr int size = 100;
}

User::User(QObject *parent) :
    QObject(parent)
{
}

QString User::getName() const
{
    return m_name;
}

void User::setName(const QString &userName)
{
    if (userName == m_name)
        return;

    m_name = userName;
    emit nameChanged();
}

void User::paintQR(QPainter &painter, const QSize sz, const QString &data, QColor fg)
{
     // NOTE: At this point you will use the API to get the encoding and format you want, instead of my hardcoded stuff:
     qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(data.toUtf8().constData(), qrcodegen::QrCode::Ecc::HIGH);
     const int s = qr.getSize()>0 ? qr.getSize():1;
     const double w=sz.width();
     const double h=sz.height();
     const double aspect=w/h;
     const double size=((aspect>1.0)?h:w);
     const double scale=size/(s+2);
     // NOTE: For performance reasons my implementation only draws the foreground parts in supplied color.
     // It expects background to be prepared already (in white or whatever is preferred).
     painter.setPen(Qt::NoPen);
     painter.setBrush(fg);
     for(int y=0; y<s; y++) {
         for(int x=0; x<s; x++) {
             const int color=qr.getModule(x, y);  // 0 for white, 1 for black
             if(0x0!=color) {
                 const double rx1=(x+1)*scale, ry1=(y+1)*scale;
                 QRectF r(rx1, ry1, scale, scale);
                 painter.drawRects(&r,1);
             }
         }
     }
}

void User::print() const
{
    QPrinter printer;
    QPrintDialog dialog(&printer);
    dialog.printer()->setPrinterName("Microsoft Print to PDF");
    dialog.setWindowTitle(tr("Print check-in recipt"));

    QPainter painter(&printer);
    painter.drawImage(0,0, getQRImage());
    painter.end();

}

QString User::generateQR() const
{
    QByteArray bArray;
    QBuffer buffer(&bArray);
    buffer.open(QIODevice::WriteOnly);
    getQRImage().save(&buffer, "PNG");
    QString image("data:image/png;base64,");
    image.append(QString::fromLatin1(bArray.toBase64().data()));
    return image;
}

QImage User::getQRImage() const
{
    QPixmap map(size, size);
    QPainter painter(&map);
    QByteArray bArray;
    QBuffer buffer(&bArray);

    paintQR(painter,QSize(size,size),m_name, QColor("white"));
    buffer.open(QIODevice::WriteOnly);
    return map.toImage();
}
