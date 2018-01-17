#include "QZXing.h"

#include <zxing/common/GlobalHistogramBinarizer.h>
#include <zxing/Binarizer.h>
#include <zxing/BinaryBitmap.h>
#include <zxing/MultiFormatReader.h>
#include <zxing/DecodeHints.h>
#include "CameraImageWrapper.h"
#ifndef OS_SAILFISH
#include "imagehandler.h"
#endif
#include <QTime>
#include <QUrl>
#include <zxing/qrcode/ErrorCorrectionLevel.h>
#include <QColor>

using namespace zxing;

QZXing::QZXing(QObject *parent) : QObject(parent)
{
    isThreaded = true;

    decoder = new MultiFormatReader();
    setDecoder(DecoderFormat_QR_CODE |
               DecoderFormat_DATA_MATRIX |
               DecoderFormat_UPC_E |
               DecoderFormat_UPC_A |
               DecoderFormat_UPC_EAN_EXTENSION |
               DecoderFormat_RSS_14 |
               DecoderFormat_RSS_EXPANDED | //?
               DecoderFormat_PDF_417 | //?
               DecoderFormat_MAXICODE | //?
               DecoderFormat_EAN_8 |
               DecoderFormat_EAN_13 |
               DecoderFormat_CODE_128 |
               DecoderFormat_CODE_93 |
               DecoderFormat_CODE_39 |
               DecoderFormat_CODABAR |
               DecoderFormat_ITF |
               DecoderFormat_Aztec);
#ifndef OS_SAILFISH
    imageHandler = new ImageHandler();
#endif
}

QZXing::~QZXing()
{
#ifndef OS_SAILFISH
    if (imageHandler) delete imageHandler;
#endif
    if (decoder) delete decoder;
}

QZXing::QZXing(QZXing::DecoderFormat decodeHints, QObject *parent) : QObject(parent)
{
    decoder = new MultiFormatReader();
#ifndef OS_SAILFISH
    imageHandler = new ImageHandler();
#endif

    setDecoder(decodeHints);
}

QString QZXing::decoderFormatToString(int fmt)
{
    switch (fmt) {
      case 1:
          return "AZTEC";

      case 2:
          return "CODABAR";

      case 3:
          return "CODE_39";

      case 4:
          return "CODE_93";

      case 5:
          return "CODE_128";

      case 6:
          return "DATA_MATRIX";

      case 7:
          return "EAN_8";

      case 8:
          return "EAN_13";

      case 9:
          return "ITF";

      case 10:
          return "MAXICODE";

      case 11:
          return "PDF_417";

      case 12:
          return "QR_CODE";

      case 13:
          return "RSS_14";

      case 14:
          return "RSS_EXPANDED";

      case 15:
          return "UPC_A";

      case 16:
          return "UPC_E";

      case 17:
          return "UPC_EAN_EXTENSION";
    } // switch
    return QString();
}

QString QZXing::foundedFormat() const
{
    return foundedFmt;
}

QString QZXing::charSet() const
{
    return charSet_;
}

void QZXing::setDecoder(const uint &hint)
{
    unsigned int newHints = 0;

    if(hint & DecoderFormat_Aztec)
        newHints |= DecodeHints::AZTEC_HINT;

    if(hint & DecoderFormat_CODABAR)
        newHints |= DecodeHints::CODABAR_HINT;

    if(hint & DecoderFormat_CODE_39)
        newHints |= DecodeHints::CODE_39_HINT;

    if(hint & DecoderFormat_CODE_93)
        newHints |= DecodeHints::CODE_93_HINT;

    if(hint & DecoderFormat_CODE_128)
        newHints |= DecodeHints::CODE_128_HINT;

    if(hint & DecoderFormat_DATA_MATRIX)
        newHints |= DecodeHints::DATA_MATRIX_HINT;

    if(hint & DecoderFormat_EAN_8)
        newHints |= DecodeHints::EAN_8_HINT;

    if(hint & DecoderFormat_EAN_13)
        newHints |= DecodeHints::EAN_13_HINT;

    if(hint & DecoderFormat_ITF)
        newHints |= DecodeHints::ITF_HINT;

    if(hint & DecoderFormat_MAXICODE)
        newHints |= DecodeHints::MAXICODE_HINT;

    if(hint & DecoderFormat_PDF_417)
        newHints |= DecodeHints::PDF_417_HINT;

    if(hint & DecoderFormat_QR_CODE)
        newHints |= DecodeHints::QR_CODE_HINT;

    if(hint & DecoderFormat_RSS_14)
        newHints |= DecodeHints::RSS_14_HINT;

    if(hint & DecoderFormat_RSS_EXPANDED)
        newHints |= DecodeHints::RSS_EXPANDED_HINT;

    if(hint & DecoderFormat_UPC_A)
        newHints |= DecodeHints::UPC_A_HINT;

    if(hint & DecoderFormat_UPC_E)
        newHints |= DecodeHints::UPC_E_HINT;

    if(hint & DecoderFormat_UPC_EAN_EXTENSION)
        newHints |= DecodeHints::UPC_EAN_EXTENSION_HINT;

    enabledDecoders = newHints;

    emit enabledFormatsChanged();
}

QString QZXing::decodeImage(QImage &image, int maxWidth, int maxHeight, bool smoothTransformation)
{
    QTime t;
    t.start();
    Ref<Result> res;
    emit decodingStarted();

    if(image.isNull())
    {
        emit decodingFinished(false);
        processingTime = t.elapsed();
        return "";
    }

    CameraImageWrapper *ciw = NULL;
    try {
        if ((maxWidth > 0) || (maxHeight > 0))
            ciw = CameraImageWrapper::Factory(image, maxWidth, maxHeight, smoothTransformation);
        else
            ciw = new CameraImageWrapper(image);

        Ref<LuminanceSource> imageRef(ciw);
        GlobalHistogramBinarizer *binz = new GlobalHistogramBinarizer(imageRef);

        Ref<Binarizer> bz(binz);
        BinaryBitmap *bb = new BinaryBitmap(bz);

        Ref<BinaryBitmap> ref(bb);

        res = decoder->decode(ref, DecodeHints((int)enabledDecoders));

        QString string = QString(res->getText()->getText().c_str());
        if (!string.isEmpty()) {
            int fmt = res->getBarcodeFormat().value;
            foundedFmt = decoderFormatToString(fmt);
            charSet_ = QString::fromStdString(res->getCharSet());
            if (!charSet_.isEmpty()) {
                QTextCodec *codec = QTextCodec::codecForName(res->getCharSet().c_str());
                if (codec)
                    string = codec->toUnicode(res->getText()->getText().c_str());
            }
            emit tagFound(string);
            emit tagFoundAdvanced(string, foundedFmt, charSet_);
        }
        processingTime = t.elapsed();
        emit decodingFinished(true);
        return string;
    }
    catch(zxing::Exception &e)
    {
        emit error(QString(e.what()));
        emit decodingFinished(false);
        processingTime = t.elapsed();
        return "";
    }
}

QString QZXing::decodeImageFromFile(const QString& imageFilePath, int maxWidth, int maxHeight, bool smoothTransformation)
{
    // used to have a check if this image exists
    // but was removed because if the image file path doesn't point to a valid image
    // then the QImage::isNull will return true and the decoding will fail eitherway.
    QUrl imageUrl(imageFilePath);
    QImage tmpImage = QImage(imageUrl.toLocalFile());
    return decodeImage(tmpImage, maxWidth, maxHeight, smoothTransformation);
}
#ifndef OS_SAILFISH
QString QZXing::decodeImageQML(QObject *item)
{
    return decodeSubImageQML(item);
}



QString QZXing::decodeSubImageQML(QObject *item,
                                  const double offsetX, const double offsetY,
                                  const double width, const double height)
{
    if(item  == NULL)
    {
        processingTime = 0;
        emit decodingFinished(false);
        return "";
    }

    QImage img = imageHandler->extractQImage(item, offsetX, offsetY, width, height);

    return decodeImage(img);
}

QString QZXing::decodeImageQML(const QUrl &imageUrl)
{
    return decodeSubImageQML(imageUrl);
}

QString QZXing::decodeSubImageQML(const QUrl &imageUrl,
                                  const double offsetX, const double offsetY,
                                  const double width, const double height)
{
    QString imagePath = imageUrl.path();
    imagePath = imagePath.trimmed();
    QFile file(imagePath);
    if (!file.exists()) {
        qDebug() << "[decodeSubImageQML()] The file" << file.fileName() << "does not exist.";
        emit decodingFinished(false);
        return "";
    }
    QImage img(imageUrl.path());
    if(!(offsetX == 0 && offsetY == 0 && width == 0 && height == 0)) {
        img = img.copy(offsetX, offsetY, width, height);
    }
    return decodeImage(img);
}
#endif
int QZXing::getProcessTimeOfLastDecoding()
{
    return processingTime;
}

uint QZXing::getEnabledFormats() const
{
    return enabledDecoders;
}
