using System;
using System.Collections.Generic;
using System.Net;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Windows.Resources;
using Microsoft.Phone.Tasks;
using Microsoft.Devices;
using rhoruntime;
using ZXing;

namespace rho {

namespace BarcodeImpl
{      
    public class Barcode : BarcodeBase
    {
        public static int vibrateTimeMs = 100;

        private BarcodeReaderLib.OpticalReaderTask _barcodeScanTask = null;
        private IBarcodeReader           _barcodeReader   = new BarcodeReader();

        static EventWaitHandle _waitHandle      = new AutoResetEvent(false);
        static String          _recognizeResult = "";

        private IMethodResult _methodResult = null;
        private string        _effectiveRubyCallbackURL = null;
        private String        _prevScanResult = "";

        public Barcode()
        {
            if (BarcodeReaderLib.OpticalReaderTask.Instance == null)
                _barcodeScanTask = new BarcodeReaderLib.OpticalReaderTask();
            else
                _barcodeScanTask = BarcodeReaderLib.OpticalReaderTask.Instance;
            _barcodeScanTask.Completed += BarcodeScanTask_Completed;
        }

        public override void setNativeImpl(string strID, long native)
        {
            base.setNativeImpl(strID, native);
            IMethodResult pResult = new CMethodResultImpl();
            setProperty("scannerType", "camera", pResult);
        }

        public override void getAutoEnter(IMethodResult oResult) { }
        public override void setAutoEnter(bool autoEnter, IMethodResult oResult) { }
        public override void getAutoTab(IMethodResult oResult) { }
        public override void setAutoTab(bool autoTab, IMethodResult oResult) { }
        public override void getHapticFeedback(IMethodResult oResult) { }
        public override void setHapticFeedback(bool hapticFeedback, IMethodResult oResult) { }
        public override void getLinearSecurityLevel(IMethodResult oResult) { }
        public override void setLinearSecurityLevel(string linearSecurityLevel, IMethodResult oResult) { }
        public override void getScanTimeout(IMethodResult oResult) { }
        public override void setScanTimeout(int scanTimeout, IMethodResult oResult) { }
        public override void getRasterMode(IMethodResult oResult) { }
        public override void setRasterMode(string rasterMode, IMethodResult oResult) { }
        public override void getRasterHeight(IMethodResult oResult) { }
        public override void setRasterHeight(int rasterHeight, IMethodResult oResult) { }
        public override void getAimType(IMethodResult oResult) { }
        public override void setAimType(string aimType, IMethodResult oResult) { }
        public override void getTimedAimDuration(IMethodResult oResult) { }
        public override void setTimedAimDuration(int timedAimDuration, IMethodResult oResult) { }
        public override void getSameSymbolTimeout(IMethodResult oResult) { }
        public override void setSameSymbolTimeout(int sameSymbolTimeout, IMethodResult oResult) { }
        public override void getDifferentSymbolTimeout(IMethodResult oResult) { }
        public override void setDifferentSymbolTimeout(int differentSymbolTimeout, IMethodResult oResult) { }
        public override void getAimMode(IMethodResult oResult) { }
        public override void setAimMode(string aimMode, IMethodResult oResult) { }
        public override void getPicklistMode(IMethodResult oResult) { }
        public override void setPicklistMode(string picklistMode, IMethodResult oResult) { }
        public override void getViewfinderMode(IMethodResult oResult) { }
        public override void setViewfinderMode(string viewfinderMode, IMethodResult oResult) { }
        public override void getViewfinderX(IMethodResult oResult) { }
        public override void setViewfinderX(int viewfinderX, IMethodResult oResult) { }
        public override void getViewfinderY(IMethodResult oResult) { }
        public override void setViewfinderY(int viewfinderY, IMethodResult oResult) { }
        public override void getViewfinderWidth(IMethodResult oResult) { }
        public override void setViewfinderWidth(int viewfinderWidth, IMethodResult oResult) { }
        public override void getViewfinderHeight(IMethodResult oResult) { }
        public override void setViewfinderHeight(int viewfinderHeight, IMethodResult oResult) { }
        public override void getViewfinderFeedback(IMethodResult oResult) { }
        public override void setViewfinderFeedback(string viewfinderFeedback, IMethodResult oResult) { }
        public override void getViewfinderFeedbackTime(IMethodResult oResult) { }
        public override void setViewfinderFeedbackTime(int viewfinderFeedbackTime, IMethodResult oResult) { }
        public override void getFocusMode(IMethodResult oResult) { }
        public override void setFocusMode(string focusMode, IMethodResult oResult) { }
        public override void getIlluminationMode(IMethodResult oResult) { }
        public override void setIlluminationMode(string illuminationMode, IMethodResult oResult) { }
        public override void getDpmMode(IMethodResult oResult) { }
        public override void setDpmMode(bool dpmMode, IMethodResult oResult) { }
        public override void getInverse1dMode(IMethodResult oResult) { }
        public override void setInverse1dMode(string inverse1dMode, IMethodResult oResult) { }
        public override void getPoorQuality1dMode(IMethodResult oResult) { }
        public override void setPoorQuality1dMode(bool poorQuality1dMode, IMethodResult oResult) { }
        public override void getBeamWidth(IMethodResult oResult) { }
        public override void setBeamWidth(string beamWidth, IMethodResult oResult) { }
        public override void getDbpMode(IMethodResult oResult) { }
        public override void setDbpMode(string dbpMode, IMethodResult oResult) { }
        public override void getKlasseEins(IMethodResult oResult) { }
        public override void setKlasseEins(bool klasseEins, IMethodResult oResult) { }
        public override void getAdaptiveScanning(IMethodResult oResult) { }
        public override void setAdaptiveScanning(bool adaptiveScanning, IMethodResult oResult) { }
        public override void getBidirectionalRedundancy(IMethodResult oResult) { }
        public override void setBidirectionalRedundancy(bool bidirectionalRedundancy, IMethodResult oResult) { }
        public override void getBarcodeDataFormat(IMethodResult oResult) { }
        public override void setBarcodeDataFormat(string barcodeDataFormat, IMethodResult oResult) { }
        public override void getDataBufferSize(IMethodResult oResult) { }
        public override void setDataBufferSize(int dataBufferSize, IMethodResult oResult) { }
        public override void getConnectionIdleTimeout(IMethodResult oResult) { }
        public override void setConnectionIdleTimeout(int connectionIdleTimeout, IMethodResult oResult) { }
        public override void getDisconnectBtOnDisable(IMethodResult oResult) { }
        public override void setDisconnectBtOnDisable(bool disconnectBtOnDisable, IMethodResult oResult) { }
        public override void getDisplayBtAddressBarcodeOnEnable(IMethodResult oResult) { }
        public override void setDisplayBtAddressBarcodeOnEnable(bool displayBtAddressBarcodeOnEnable, IMethodResult oResult) { }
        public override void getEnableTimeout(IMethodResult oResult) { }
        public override void setEnableTimeout(int enableTimeout, IMethodResult oResult) { }
        public override void getFriendlyName(IMethodResult oResult) { }
        public override void getLcdMode(IMethodResult oResult) { }
        public override void setLcdMode(bool lcdMode, IMethodResult oResult) { }
        public override void getLowBatteryScan(IMethodResult oResult) { }
        public override void setLowBatteryScan(bool lowBatteryScan, IMethodResult oResult) { }
        public override void getTriggerConnected(IMethodResult oResult) { }
        public override void setTriggerConnected(bool triggerConnected, IMethodResult oResult) { }
        public override void getDisableScannerDuringNavigate(IMethodResult oResult) { }
        public override void setDisableScannerDuringNavigate(bool disableScannerDuringNavigate, IMethodResult oResult) { }
        public override void getDecodeVolume(IMethodResult oResult) { }
        public override void setDecodeVolume(int decodeVolume, IMethodResult oResult) { }
        public override void getDecodeDuration(IMethodResult oResult) { }
        public override void setDecodeDuration(int decodeDuration, IMethodResult oResult) { }
        public override void getDecodeFrequency(IMethodResult oResult) { }
        public override void setDecodeFrequency(int decodeFrequency, IMethodResult oResult) { }
        public override void getInvalidDecodeFrequency(IMethodResult oResult) { }
        public override void setInvalidDecodeFrequency(int invalidDecodeFrequency, IMethodResult oResult) { }
        public override void getDecodeSound(IMethodResult oResult) { }
        public override void setDecodeSound(string decodeSound, IMethodResult oResult) { }
        public override void getInvalidDecodeSound(IMethodResult oResult) { }
        public override void setInvalidDecodeSound(string invalidDecodeSound, IMethodResult oResult) { }
        public override void getAllDecoders(IMethodResult oResult) { }
        public override void setAllDecoders(bool allDecoders, IMethodResult oResult) { }
        public override void getAztec(IMethodResult oResult) { }
        public override void setAztec(bool aztec, IMethodResult oResult) { }
        public override void getChinese2of5(IMethodResult oResult) { }
        public override void setChinese2of5(bool chinese2of5, IMethodResult oResult) { }
        public override void getCodabar(IMethodResult oResult) { }
        public override void setCodabar(bool codabar, IMethodResult oResult) { }
        public override void getCodabarClsiEditing(IMethodResult oResult) { }
        public override void setCodabarClsiEditing(bool codabarClsiEditing, IMethodResult oResult) { }
        public override void getCodabarMaxLength(IMethodResult oResult) { }
        public override void setCodabarMaxLength(int codabarMaxLength, IMethodResult oResult) { }
        public override void getCodabarMinLength(IMethodResult oResult) { }
        public override void setCodabarMinLength(int codabarMinLength, IMethodResult oResult) { }
        public override void getCodabarNotisEditing(IMethodResult oResult) { }
        public override void setCodabarNotisEditing(bool codabarNotisEditing, IMethodResult oResult) { }
        public override void getCodabarRedundancy(IMethodResult oResult) { }
        public override void setCodabarRedundancy(bool codabarRedundancy, IMethodResult oResult) { }
        public override void getCode11(IMethodResult oResult) { }
        public override void setCode11(bool code11, IMethodResult oResult) { }
        public override void getCode11checkDigitCount(IMethodResult oResult) { }
        public override void setCode11checkDigitCount(string code11checkDigitCount, IMethodResult oResult) { }
        public override void getCode11maxLength(IMethodResult oResult) { }
        public override void setCode11maxLength(int code11maxLength, IMethodResult oResult) { }
        public override void getCode11minLength(IMethodResult oResult) { }
        public override void setCode11minLength(int code11minLength, IMethodResult oResult) { }
        public override void getCode11redundancy(IMethodResult oResult) { }
        public override void setCode11redundancy(bool code11redundancy, IMethodResult oResult) { }
        public override void getCode11reportCheckDigit(IMethodResult oResult) { }
        public override void setCode11reportCheckDigit(bool code11reportCheckDigit, IMethodResult oResult) { }
        public override void getCode128(IMethodResult oResult) { }
        public override void setCode128(bool code128, IMethodResult oResult) { }
        public override void getCode128checkIsBtTable(IMethodResult oResult) { }
        public override void setCode128checkIsBtTable(bool code128checkIsBtTable, IMethodResult oResult) { }
        public override void getCode128ean128(IMethodResult oResult) { }
        public override void setCode128ean128(bool code128ean128, IMethodResult oResult) { }
        public override void getCode128isbt128(IMethodResult oResult) { }
        public override void setCode128isbt128(bool code128isbt128, IMethodResult oResult) { }
        public override void getCode128isbt128ConcatMode(IMethodResult oResult) { }
        public override void setCode128isbt128ConcatMode(string code128isbt128ConcatMode, IMethodResult oResult) { }
        public override void getCode128maxLength(IMethodResult oResult) { }
        public override void setCode128maxLength(int code128maxLength, IMethodResult oResult) { }
        public override void getCode128minLength(IMethodResult oResult) { }
        public override void setCode128minLength(int code128minLength, IMethodResult oResult) { }
        public override void getCode128other128(IMethodResult oResult) { }
        public override void setCode128other128(bool code128other128, IMethodResult oResult) { }
        public override void getCode128redundancy(IMethodResult oResult) { }
        public override void setCode128redundancy(bool code128redundancy, IMethodResult oResult) { }
        public override void getCode128securityLevel(IMethodResult oResult) { }
        public override void setCode128securityLevel(int code128securityLevel, IMethodResult oResult) { }
        public override void getCompositeAb(IMethodResult oResult) { }
        public override void setCompositeAb(bool compositeAb, IMethodResult oResult) { }
        public override void getCompositeAbUccLinkMode(IMethodResult oResult) { }
        public override void setCompositeAbUccLinkMode(string compositeAbUccLinkMode, IMethodResult oResult) { }
        public override void getCompositeAbUseUpcPreambleCheckDigitRules(IMethodResult oResult) { }
        public override void setCompositeAbUseUpcPreambleCheckDigitRules(bool compositeAbUseUpcPreambleCheckDigitRules, IMethodResult oResult) { }
        public override void getCompositeC(IMethodResult oResult) { }
        public override void setCompositeC(bool compositeC, IMethodResult oResult) { }
        public override void getCode39(IMethodResult oResult) { }
        public override void setCode39(bool code39, IMethodResult oResult) { }
        public override void getCode39code32Prefix(IMethodResult oResult) { }
        public override void setCode39code32Prefix(bool code39code32Prefix, IMethodResult oResult) { }
        public override void getCode39convertToCode32(IMethodResult oResult) { }
        public override void setCode39convertToCode32(bool code39convertToCode32, IMethodResult oResult) { }
        public override void getCode39fullAscii(IMethodResult oResult) { }
        public override void setCode39fullAscii(bool code39fullAscii, IMethodResult oResult) { }
        public override void getCode39maxLength(IMethodResult oResult) { }
        public override void setCode39maxLength(int code39maxLength, IMethodResult oResult) { }
        public override void getCode39minLength(IMethodResult oResult) { }
        public override void setCode39minLength(int code39minLength, IMethodResult oResult) { }
        public override void getCode39redundancy(IMethodResult oResult) { }
        public override void setCode39redundancy(bool code39redundancy, IMethodResult oResult) { }
        public override void getCode39reportCheckDigit(IMethodResult oResult) { }
        public override void setCode39reportCheckDigit(bool code39reportCheckDigit, IMethodResult oResult) { }
        public override void getCode39securityLevel(IMethodResult oResult) { }
        public override void setCode39securityLevel(int code39securityLevel, IMethodResult oResult) { }
        public override void getCode39verifyCheckDigit(IMethodResult oResult) { }
        public override void setCode39verifyCheckDigit(bool code39verifyCheckDigit, IMethodResult oResult) { }
        public override void getCode93(IMethodResult oResult) { }
        public override void setCode93(bool code93, IMethodResult oResult) { }
        public override void getCode93maxLength(IMethodResult oResult) { }
        public override void setCode93maxLength(int code93maxLength, IMethodResult oResult) { }
        public override void getCode93minLength(IMethodResult oResult) { }
        public override void setCode93minLength(int code93minLength, IMethodResult oResult) { }
        public override void getCode93redundancy(IMethodResult oResult) { }
        public override void setCode93redundancy(bool code93redundancy, IMethodResult oResult) { }
        public override void getD2of5(IMethodResult oResult) { }
        public override void setD2of5(bool d2of5, IMethodResult oResult) { }
        public override void getD2of5maxLength(IMethodResult oResult) { }
        public override void setD2of5maxLength(int d2of5maxLength, IMethodResult oResult) { }
        public override void getD2of5minLength(IMethodResult oResult) { }
        public override void setD2of5minLength(int d2of5minLength, IMethodResult oResult) { }
        public override void getD2of5redundancy(IMethodResult oResult) { }
        public override void setD2of5redundancy(bool d2of5redundancy, IMethodResult oResult) { }
        public override void getDatamatrix(IMethodResult oResult) { }
        public override void setDatamatrix(bool datamatrix, IMethodResult oResult) { }
        public override void getEan13(IMethodResult oResult) { }
        public override void setEan13(bool ean13, IMethodResult oResult) { }
        public override void getEan8(IMethodResult oResult) { }
        public override void setEan8(bool ean8, IMethodResult oResult) { }
        public override void getEan8convertToEan13(IMethodResult oResult) { }
        public override void setEan8convertToEan13(bool ean8convertToEan13, IMethodResult oResult) { }
        public override void getI2of5(IMethodResult oResult) { }
        public override void setI2of5(bool i2of5, IMethodResult oResult) { }
        public override void getI2of5convertToEan13(IMethodResult oResult) { }
        public override void setI2of5convertToEan13(bool i2of5convertToEan13, IMethodResult oResult) { }
        public override void getI2of5maxLength(IMethodResult oResult) { }
        public override void setI2of5maxLength(int i2of5maxLength, IMethodResult oResult) { }
        public override void getI2of5minLength(IMethodResult oResult) { }
        public override void setI2of5minLength(int i2of5minLength, IMethodResult oResult) { }
        public override void getI2of5redundancy(IMethodResult oResult) { }
        public override void setI2of5redundancy(bool i2of5redundancy, IMethodResult oResult) { }
        public override void getI2of5reportCheckDigit(IMethodResult oResult) { }
        public override void setI2of5reportCheckDigit(bool i2of5reportCheckDigit, IMethodResult oResult) { }
        public override void getI2of5verifyCheckDigit(IMethodResult oResult) { }
        public override void setI2of5verifyCheckDigit(string i2of5verifyCheckDigit, IMethodResult oResult) { }
        public override void getKorean3of5(IMethodResult oResult) { }
        public override void setKorean3of5(bool korean3of5, IMethodResult oResult) { }
        public override void getKorean3of5redundancy(IMethodResult oResult) { }
        public override void setKorean3of5redundancy(bool korean3of5redundancy, IMethodResult oResult) { }
        public override void getKorean3of5maxLength(IMethodResult oResult) { }
        public override void setKorean3of5maxLength(int korean3of5maxLength, IMethodResult oResult) { }
        public override void getKorean3of5minLength(IMethodResult oResult) { }
        public override void setKorean3of5minLength(int korean3of5minLength, IMethodResult oResult) { }
        public override void getMacroPdf(IMethodResult oResult) { }
        public override void setMacroPdf(bool macroPdf, IMethodResult oResult) { }
        public override void getMacroPdfBufferLabels(IMethodResult oResult) { }
        public override void setMacroPdfBufferLabels(bool macroPdfBufferLabels, IMethodResult oResult) { }
        public override void getMacroPdfConvertToPdf417(IMethodResult oResult) { }
        public override void setMacroPdfConvertToPdf417(bool macroPdfConvertToPdf417, IMethodResult oResult) { }
        public override void getMacroPdfExclusive(IMethodResult oResult) { }
        public override void setMacroPdfExclusive(bool macroPdfExclusive, IMethodResult oResult) { }
        public override void getMacroMicroPdf(IMethodResult oResult) { }
        public override void setMacroMicroPdf(bool macroMicroPdf, IMethodResult oResult) { }
        public override void getMacroMicroPdfBufferLabels(IMethodResult oResult) { }
        public override void setMacroMicroPdfBufferLabels(bool macroMicroPdfBufferLabels, IMethodResult oResult) { }
        public override void getMacroMicroPdfConvertToMicroPdf(IMethodResult oResult) { }
        public override void setMacroMicroPdfConvertToMicroPdf(bool macroMicroPdfConvertToMicroPdf, IMethodResult oResult) { }
        public override void getMacroMicroPdfExclusive(IMethodResult oResult) { }
        public override void setMacroMicroPdfExclusive(bool macroMicroPdfExclusive, IMethodResult oResult) { }
        public override void getMacroMicroPdfReportAppendInfo(IMethodResult oResult) { }
        public override void setMacroMicroPdfReportAppendInfo(bool macroMicroPdfReportAppendInfo, IMethodResult oResult) { }
        public override void getMatrix2of5(IMethodResult oResult) { }
        public override void setMatrix2of5(bool matrix2of5, IMethodResult oResult) { }
        public override void getMatrix2of5maxLength(IMethodResult oResult) { }
        public override void setMatrix2of5maxLength(int matrix2of5maxLength, IMethodResult oResult) { }
        public override void getMatrix2of5minLength(IMethodResult oResult) { }
        public override void setMatrix2of5minLength(int matrix2of5minLength, IMethodResult oResult) { }
        public override void getMatrix2of5reportCheckDigit(IMethodResult oResult) { }
        public override void setMatrix2of5reportCheckDigit(bool matrix2of5reportCheckDigit, IMethodResult oResult) { }
        public override void getMatrix2of5verifyCheckDigit(IMethodResult oResult) { }
        public override void setMatrix2of5verifyCheckDigit(bool matrix2of5verifyCheckDigit, IMethodResult oResult) { }
        public override void getMaxiCode(IMethodResult oResult) { }
        public override void setMaxiCode(bool maxiCode, IMethodResult oResult) { }
        public override void getMicroPdf(IMethodResult oResult) { }
        public override void setMicroPdf(bool microPdf, IMethodResult oResult) { }
        public override void getMicroQr(IMethodResult oResult) { }
        public override void setMicroQr(bool microQr, IMethodResult oResult) { }
        public override void getMsi(IMethodResult oResult) { }
        public override void setMsi(bool msi, IMethodResult oResult) { }
        public override void getMsiCheckDigits(IMethodResult oResult){}
        public override void setMsiCheckDigits(string msiCheckDigits, IMethodResult oResult) { }
        public override void getMsiCheckDigitScheme(IMethodResult oResult) { }
        public override void setMsiCheckDigitScheme(string msiCheckDigitScheme, IMethodResult oResult) { }
        public override void getMsiMaxLength(IMethodResult oResult) { }
        public override void setMsiMaxLength(int msiMaxLength, IMethodResult oResult) { }
        public override void getMsiMinLength(IMethodResult oResult) { }
        public override void setMsiMinLength(int msiMinLength, IMethodResult oResult) { }
        public override void getMsiRedundancy(IMethodResult oResult) { }
        public override void setMsiRedundancy(bool msiRedundancy, IMethodResult oResult) { }
        public override void getMsiReportCheckDigit(IMethodResult oResult) { }
        public override void setMsiReportCheckDigit(bool msiReportCheckDigit, IMethodResult oResult) { }
        public override void getPdf417(IMethodResult oResult) { }
        public override void setPdf417(bool pdf417, IMethodResult oResult) { }
        public override void getSignature(IMethodResult oResult) { }
        public override void setSignature(bool signature, IMethodResult oResult) { }
        public override void getSignatureImageHeight(IMethodResult oResult) { }
        public override void setSignatureImageHeight(int signatureImageHeight, IMethodResult oResult) { }
        public override void getSignatureImageWidth(IMethodResult oResult) { }
        public override void setSignatureImageWidth(int signatureImageWidth, IMethodResult oResult) { }
        public override void getSignatureImageQuality(IMethodResult oResult) { }
        public override void setSignatureImageQuality(int signatureImageQuality, IMethodResult oResult) { }
        public override void getAusPostal(IMethodResult oResult) { }
        public override void setAusPostal(bool ausPostal, IMethodResult oResult) { }
        public override void getCanPostal(IMethodResult oResult) { }
        public override void setCanPostal(bool canPostal, IMethodResult oResult) { }
        public override void getDutchPostal(IMethodResult oResult) { }
        public override void setDutchPostal(bool dutchPostal, IMethodResult oResult) { }
        public override void getJapPostal(IMethodResult oResult) { }
        public override void setJapPostal(bool japPostal, IMethodResult oResult) { }
        public override void getUkPostal(IMethodResult oResult) { }
        public override void setUkPostal(bool ukPostal, IMethodResult oResult) { }
        public override void getUkPostalReportCheckDigit(IMethodResult oResult) { }
        public override void setUkPostalReportCheckDigit(bool ukPostalReportCheckDigit, IMethodResult oResult) { }
        public override void getUs4state(IMethodResult oResult) { }
        public override void setUs4state(bool us4state, IMethodResult oResult) { }
        public override void getUs4stateFics(IMethodResult oResult) { }
        public override void setUs4stateFics(bool us4stateFics, IMethodResult oResult) { }
        public override void getUsPlanet(IMethodResult oResult) { }
        public override void setUsPlanet(bool usPlanet, IMethodResult oResult) { }
        public override void getUsPlanetReportCheckDigit(IMethodResult oResult) { }
        public override void setUsPlanetReportCheckDigit(bool usPlanetReportCheckDigit, IMethodResult oResult) { }
        public override void getUsPostNet(IMethodResult oResult) { }
        public override void setUsPostNet(bool usPostNet, IMethodResult oResult) { }
        public override void getUsPostNetReportCheckDigit(IMethodResult oResult) { }
        public override void setUsPostNetReportCheckDigit(bool usPostNetReportCheckDigit, IMethodResult oResult) { }
        public override void getQrCode(IMethodResult oResult) { }
        public override void setQrCode(bool qrCode, IMethodResult oResult) { }
        public override void getGs1dataBar(IMethodResult oResult) { }
        public override void setGs1dataBar(bool gs1dataBar, IMethodResult oResult) { }
        public override void getGs1dataBarExpanded(IMethodResult oResult) { }
        public override void setGs1dataBarExpanded(bool gs1dataBarExpanded, IMethodResult oResult) { }
        public override void getGs1dataBarLimited(IMethodResult oResult) { }
        public override void setGs1dataBarLimited(bool gs1dataBarLimited, IMethodResult oResult) { }
        public override void getTlc39(IMethodResult oResult) { }
        public override void setTlc39(bool tlc39, IMethodResult oResult) { }
        public override void getTrioptic39(IMethodResult oResult) { }
        public override void setTrioptic39(bool trioptic39, IMethodResult oResult) { }
        public override void getTrioptic39Redundancy(IMethodResult oResult) { }
        public override void setTrioptic39Redundancy(bool trioptic39Redundancy, IMethodResult oResult) { }
        public override void getUpcEanBookland(IMethodResult oResult) { }
        public override void setUpcEanBookland(bool upcEanBookland, IMethodResult oResult) { }
        public override void getUpcEanBooklandFormat(IMethodResult oResult) { }
        public override void setUpcEanBooklandFormat(string upcEanBooklandFormat, IMethodResult oResult) { }
        public override void getUpcEanConvertGs1dataBarToUpcEan(IMethodResult oResult) { }
        public override void setUpcEanConvertGs1dataBarToUpcEan(bool upcEanConvertGs1dataBarToUpcEan, IMethodResult oResult) { }
        public override void getUpcEanCoupon(IMethodResult oResult) { }
        public override void setUpcEanCoupon(bool upcEanCoupon, IMethodResult oResult) { }
        public override void getUpcEanLinearDecode(IMethodResult oResult) { }
        public override void setUpcEanLinearDecode(bool upcEanLinearDecode, IMethodResult oResult) { }
        public override void getUpcEanRandomWeightCheckDigit(IMethodResult oResult) { }
        public override void setUpcEanRandomWeightCheckDigit(bool upcEanRandomWeightCheckDigit, IMethodResult oResult) { }
        public override void getUpcEanRetryCount(IMethodResult oResult) { }
        public override void setUpcEanRetryCount(int upcEanRetryCount, IMethodResult oResult) { }
        public override void getUpcEanSecurityLevel(IMethodResult oResult) { }
        public override void setUpcEanSecurityLevel(int upcEanSecurityLevel, IMethodResult oResult) { }
        public override void getUpcEanSupplemental2(IMethodResult oResult) { }
        public override void setUpcEanSupplemental2(bool upcEanSupplemental2, IMethodResult oResult) { }
        public override void getUpcEanSupplemental5(IMethodResult oResult) { }
        public override void setUpcEanSupplemental5(bool upcEanSupplemental5, IMethodResult oResult) { }
        public override void getUpcEanSupplementalMode(IMethodResult oResult) { }
        public override void setUpcEanSupplementalMode(string upcEanSupplementalMode, IMethodResult oResult) { }
        public override void getUpca(IMethodResult oResult) { }
        public override void setUpca(bool upca, IMethodResult oResult) { }
        public override void getUpcaPreamble(IMethodResult oResult) { }
        public override void setUpcaPreamble(string upcaPreamble, IMethodResult oResult) { }
        public override void getUpcaReportCheckDigit(IMethodResult oResult) { }
        public override void setUpcaReportCheckDigit(bool upcaReportCheckDigit, IMethodResult oResult) { }
        public override void getUpce0(IMethodResult oResult) { }
        public override void setUpce0(bool upce0, IMethodResult oResult) { }
        public override void getUpce0convertToUpca(IMethodResult oResult) { }
        public override void setUpce0convertToUpca(bool upce0convertToUpca, IMethodResult oResult) { }
        public override void getUpce0preamble(IMethodResult oResult) { }
        public override void setUpce0preamble(string upce0preamble, IMethodResult oResult) { }
        public override void getUpce0reportCheckDigit(IMethodResult oResult) { }
        public override void setUpce0reportCheckDigit(bool upce0reportCheckDigit, IMethodResult oResult) { }
        public override void getUpce1(IMethodResult oResult) { }
        public override void setUpce1(bool upce1, IMethodResult oResult) { }
        public override void getUpce1convertToUpca(IMethodResult oResult) { }
        public override void setUpce1convertToUpca(bool upce1convertToUpca, IMethodResult oResult) { }
        public override void getUpce1preamble(IMethodResult oResult) { }
        public override void setUpce1preamble(string upce1preamble, IMethodResult oResult) { }
        public override void getUpce1reportCheckDigit(IMethodResult oResult) { }
        public override void setUpce1reportCheckDigit(bool upce1reportCheckDigit, IMethodResult oResult) { }
        public override void getWebcode(IMethodResult oResult) { }
        public override void setWebcode(bool webcode, IMethodResult oResult) { }
        public override void getWebcodeDecodeGtSubtype(IMethodResult oResult) { }
        public override void setWebcodeDecodeGtSubtype(bool webcodeDecodeGtSubtype, IMethodResult oResult) { }
        public override void getRsmModelNumber(IMethodResult oResult) { }
        public override void getRsmSerialNumber(IMethodResult oResult) { }
        public override void getRsmDateOfManufacture(IMethodResult oResult) { }
        public override void getRsmDateOfService(IMethodResult oResult) { }
        public override void getRsmBluetoothAddress(IMethodResult oResult) { }
        public override void getRsmFirmwareVersion(IMethodResult oResult) { }
        public override void getRsmDeviceClass(IMethodResult oResult) { }
        public override void getRsmBatteryStatus(IMethodResult oResult) { }
        public override void getRsmBatteryCapacity(IMethodResult oResult) { }
        public override void getRsmBatteryId(IMethodResult oResult) { }
        public override void getRsmBluetoothAuthentication(IMethodResult oResult) { }
        public override void setRsmBluetoothAuthentication(bool rsmBluetoothAuthentication, IMethodResult oResult) { }
        public override void getRsmBluetoothEncryption(IMethodResult oResult) { }
        public override void setRsmBluetoothEncryption(bool rsmBluetoothEncryption, IMethodResult oResult) { }
        public override void getRsmBluetoothPinCode(IMethodResult oResult) { }
        public override void setRsmBluetoothPinCode(string rsmBluetoothPinCode, IMethodResult oResult) { }
        public override void getRsmBluetoothPinCodeType(IMethodResult oResult) { }
        public override void setRsmBluetoothPinCodeType(string rsmBluetoothPinCodeType, IMethodResult oResult) { }
        public override void getRsmBluetoothReconnectionAttempts(IMethodResult oResult) { }
        public override void setRsmBluetoothReconnectionAttempts(int rsmBluetoothReconnectionAttempts, IMethodResult oResult) { }
        public override void getRsmBluetoothBeepOnReconnectAttempt(IMethodResult oResult) { }
        public override void setRsmBluetoothBeepOnReconnectAttempt(bool rsmBluetoothBeepOnReconnectAttempt, IMethodResult oResult) { }
        public override void getRsmBluetoothHidAutoReconnect(IMethodResult oResult) { }
        public override void setRsmBluetoothHidAutoReconnect(string rsmBluetoothHidAutoReconnect, IMethodResult oResult) { }
        public override void getRsmBluetoothFriendlyName(IMethodResult oResult) { }
        public override void setRsmBluetoothFriendlyName(string rsmBluetoothFriendlyName, IMethodResult oResult) { }
        public override void getRsmBluetoothInquiryMode(IMethodResult oResult) { }
        public override void setRsmBluetoothInquiryMode(string rsmBluetoothInquiryMode, IMethodResult oResult) { }
        public override void getRsmBluetoothAutoReconnect(IMethodResult oResult) { }
        public override void setRsmBluetoothAutoReconnect(string rsmBluetoothAutoReconnect, IMethodResult oResult) { }
        public override void getRsmForceSavePairingBarcode(IMethodResult oResult) { }
        public override void setRsmForceSavePairingBarcode(bool rsmForceSavePairingBarcode, IMethodResult oResult) { }
        public override void getRsmLowBatteryIndication(IMethodResult oResult) { }
        public override void setRsmLowBatteryIndication(bool rsmLowBatteryIndication, IMethodResult oResult) { }
        public override void getRsmLowBatteryIndicationCycle(IMethodResult oResult) { }
        public override void setRsmLowBatteryIndicationCycle(int rsmLowBatteryIndicationCycle, IMethodResult oResult) { }
        public override void getRsmScanLineWidth(IMethodResult oResult) { }
        public override void setRsmScanLineWidth(string rsmScanLineWidth, IMethodResult oResult) { }
        public override void getRsmGoodScansDelay(IMethodResult oResult) { }
        public override void setRsmGoodScansDelay(int rsmGoodScansDelay, IMethodResult oResult) { }
        public override void getRsmDecodeFeedback(IMethodResult oResult) { }
        public override void setRsmDecodeFeedback(bool rsmDecodeFeedback, IMethodResult oResult) { }
        public override void getRsmIgnoreCode128Usps(IMethodResult oResult) { }
        public override void setRsmIgnoreCode128Usps(bool rsmIgnoreCode128Usps, IMethodResult oResult) { }
        public override void getRsmScanTriggerWakeup(IMethodResult oResult) { }
        public override void setRsmScanTriggerWakeup(bool rsmScanTriggerWakeup, IMethodResult oResult) { }
        public override void getRsmMems(IMethodResult oResult) { }
        public override void setRsmMems(bool rsmMems, IMethodResult oResult) { }
        public override void getRsmProximityEnable(IMethodResult oResult) { }
        public override void setRsmProximityEnable(bool rsmProximityEnable, IMethodResult oResult) { }
        public override void getRsmProximityContinuous(IMethodResult oResult) { }
        public override void setRsmProximityContinuous(bool rsmProximityContinuous, IMethodResult oResult) { }
        public override void getRsmProximityDistance(IMethodResult oResult) { }
        public override void setRsmProximityDistance(string rsmProximityDistance, IMethodResult oResult) { }
        public override void getRsmPagingEnable(IMethodResult oResult) { }
        public override void setRsmPagingEnable(bool rsmPagingEnable, IMethodResult oResult) { }
        public override void getRsmPagingBeepSequence(IMethodResult oResult) { }
        public override void setRsmPagingBeepSequence(int rsmPagingBeepSequence, IMethodResult oResult) { }
        public override void registerBluetoothStatus(IMethodResult oResult) { }
        public override void getScannerType(IMethodResult oResult) { }
        public override void commandRemoteScanner(string command, IMethodResult oResult) { }

        /* --------------------------------------------------------------------------------------------------------- */

        bool _isEnable = false;

        public override void enable(IReadOnlyDictionary<string, string> propertyMap, IMethodResult oResult)
        {
            _isEnable = true;
        }
                
        public override void start(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void stop(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void disable(IMethodResult oResult)
        {
            _isEnable = false;
        }        

        public override void barcode_recognize(string imageFilePath, IMethodResult oResult)
        {
            _recognizeResult = "";
            _methodResult    = oResult;

            StreamResourceInfo info = Application.GetResourceStream(new Uri(imageFilePath, UriKind.Relative));
            BitmapSource bmSrc = null;

            dispatchInvoke(() =>
            {
                bmSrc = new BitmapImage();
                bmSrc.SetSource(info.Stream);

                WriteableBitmap writableBitmap = new WriteableBitmap(bmSrc);
                writableBitmap.Invalidate();

                _barcodeReader.TryHarder = true;
                _barcodeReader.ResultFound += BarcodeRecognizeTask_Completed;
                _barcodeReader.Decode(writableBitmap);

                _waitHandle.Set();
            });

            _waitHandle.WaitOne();

            oResult.set(_recognizeResult);
        }

        void BarcodeRecognizeTask_Completed(Result obj)
        {
            _recognizeResult = obj.Text;
        }

        public override void getSupportedProperties(IMethodResult oResult)
        {
            List<String> propsList = new List<String>();
            oResult.set(propsList);
        }

        public override void take(IReadOnlyDictionary<string, string> propertyMap, IMethodResult oResult)
        {
            _methodResult = oResult;

            dispatchInvoke(() =>
            {
                _barcodeScanTask.Show();
            });
        }

        public override void take_barcode(string rubyCallbackURL, IReadOnlyDictionary<string, string> propertyMap, IMethodResult oResult)
        {
            take(null, oResult);
        }

        static Mutex _m = new Mutex();

        private void BarcodeScanTask_Completed(object sender, BarcodeReaderLib.OpticalReaderResult readerResult)
        {
            System.Diagnostics.Debug.WriteLine("BarcodeScanTask_Completed 1");

             if (readerResult.TaskResult == TaskResult.None || _methodResult == null)
                return;

            if (readerResult.TaskResult == TaskResult.Cancel)
            {
                new Thread(() =>
                {
                    _m.WaitOne();

                    System.Diagnostics.Debug.WriteLine("BarcodeScanTask_Completed cancel");

                    Dictionary<string, string> result = new Dictionary<string, string>();
                    result.Add("status", "cancel");
                    result.Add("barcode", "");
                    _methodResult.set(result);
                    _methodResult = null;

                    _m.ReleaseMutex();
                }).Start();
                
                return;
            }

            VibrateController.Default.Start(TimeSpan.FromMilliseconds(Barcode.vibrateTimeMs));

            System.Diagnostics.Debug.WriteLine("BarcodeScanTask_Completed 2");

            _prevScanResult = readerResult.Text;

            CRhoRuntime.getInstance().logEvent(String.Format("Barcode: {0} = {1}",
                readerResult.Format,
                readerResult.Text)
            );

            new Thread(() =>
            {
                _m.WaitOne();

                System.Diagnostics.Debug.WriteLine("BarcodeScanTask_Completed send data");

                Dictionary<string, string> result = new Dictionary<string, string>();
                result.Add("status", "ok");
                result.Add("barcode", readerResult.Text);
                _methodResult.set(result);

                _m.ReleaseMutex();
            }).Start();
        }
    }

    public class BarcodeSingleton : BarcodeSingletonBase
    {
        public BarcodeSingleton()
        {
        }

        public override void enumerate(IMethodResult oResult)
        {
             List<String> scannerEnum = new List<String>();
             scannerEnum.Add("CameraScanner1");
             oResult.set(scannerEnum);
        }
    }

    public class BarcodeFactory : BarcodeFactoryBase
    {
    }
}

}
