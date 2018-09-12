#include "../../../shared/generated/cpp/SignatureBase.h"
#include "common/RhodesApp.h"
#include "signatureqmlmodel.h"
namespace rho {

using namespace apiGenerator;
using namespace common;

class CSignatureImpl: public CSignatureBase
{
public:
	CSignatureImpl (const rho::String& strID) {}
};

class CSignatureSingleton: public CSignatureSingletonBase
{
public:
	/**************************************************************************/
    CSignatureSingleton()
	{
        SignatureQMLModel::getInstance();
	}

	/**************************************************************************/
	~CSignatureSingleton()
	{

	}



	/**************************************************************************/
    virtual void takeFullScreen( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
	{       
        QString fileName;
        if (propertyMap.containsKey("signature_uri")){
            fileName = QString::fromStdString(propertyMap.get("signature_uri"));
        }else if (propertyMap.containsKey("imageUri")){
            fileName = QString::fromStdString(propertyMap.get("imageUri"));
        }else if (propertyMap.containsKey("fileName")){
            fileName = QString::fromStdString(propertyMap.get("fileName"));
        }else{
            fileName = "signature_" + QDate::currentDate().toString("yyyyMMdd") +
                    QTime::currentTime().toString("hhmmss") + ".jpg";
        }
        SignatureQMLModel::getInstance()->setOResult(fileName, oResult);
        QtMainWindow::getLastInstance()->openQMLDocument("SignaturePage.qml");
	}


	virtual void show (const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
	{

	}

	/**************************************************************************/
    virtual void capture (rho::apiGenerator::CMethodResult& oResult)
	{

	}

	/**************************************************************************/
	virtual void hide(rho::apiGenerator::CMethodResult& oResult)
	{

	}

	/**************************************************************************/
    virtual void clear(rho::apiGenerator::CMethodResult& oResult)
	{

	}

	virtual void getCompressionFormat(rho::apiGenerator::CMethodResult& oResult) {
        oResult.set(COMPRESSION_FORMAT_PNG);
	}

    virtual void setCompressionFormat( const rho::String& compressionFormat, rho::apiGenerator::CMethodResult& oResult) {

	}

    virtual void getOutputFormat(rho::apiGenerator::CMethodResult& oResult) {
        //oResult.set(outputFormat);
	}

    virtual void setOutputFormat( const rho::String& outputFormat, rho::apiGenerator::CMethodResult& oResult) {
        /*if((outputFormat.compare(OUTPUT_FORMAT_IMAGE) == 0) || (outputFormat.compare(OUTPUT_FORMAT_DATAURI) == 0) ) {
			this->outputFormat = outputFormat;			
			if(outputFormat.compare(OUTPUT_FORMAT_IMAGE) == 0)
				bFormatImage=true;
			else if(outputFormat.compare(OUTPUT_FORMAT_DATAURI) == 0)
				bFormatImage=false;
			
		} else {
			oResult.setError("Invalid outputFormat");
        }*/
	}

    virtual void getFileName(rho::apiGenerator::CMethodResult& oResult) {
        oResult.set(SignatureQMLModel::getInstance()->getFileName().toStdString());
	}

    virtual void setFileName( const rho::String& fileName, rho::apiGenerator::CMethodResult& oResult) {       
        if (fileName.length() != 0) {
            SignatureQMLModel::getInstance()->setFileName(QString::fromStdString(fileName));
        }
	}

    virtual void getBorder(rho::apiGenerator::CMethodResult& oResult) {
        //oResult.set(border);
	}

    virtual void setBorder( bool border, rho::apiGenerator::CMethodResult& oResult) {
        //this->border = border;
	}

    virtual void getPenColor(rho::apiGenerator::CMethodResult& oResult) {
        //oResult.set(this->penColour);
	}

    virtual void setPenColor( const rho::String& penColor, rho::apiGenerator::CMethodResult& oResult) {

	}

    virtual void getPenWidth(rho::apiGenerator::CMethodResult& oResult) {
        //oResult.set(this->penWidth);
	}

    virtual void setPenWidth( int penWidth, rho::apiGenerator::CMethodResult& oResult) {
		// Do we need to actually check this to make sure it isn't a stupid value?
        /*this->penWidth = penWidth;

		if (pSignature) {
			pSignature->SetLineWidth(this->penWidth);
        }*/
	}

    virtual void getBgColor(rho::apiGenerator::CMethodResult& oResult) {
        //oResult.set(this->bgColour);
	}

    virtual void setBgColor( const rho::String& bgColor, rho::apiGenerator::CMethodResult& oResult) {
        /*this->bgColour = bgColor;

		// MultiByteToWideChar
		int requiredSize = mbstowcs(NULL, bgColor.c_str(), 0);
		TCHAR* wBgColor = (TCHAR *)malloc( (requiredSize + 1) * sizeof( TCHAR ));
		int size = mbstowcs( wBgColor, bgColor.c_str(), requiredSize + 1); // C4996

		if (pSignature)
			pSignature->SetBgColor(wBgColor);
		// Release memory to stop leak
        free(wBgColor);	*/
	}

    virtual void getLeft(rho::apiGenerator::CMethodResult& oResult) {
        //oResult.set( this->left );
	}

    virtual void setLeft( int left, rho::apiGenerator::CMethodResult& oResult) {
        /*if (left >= 0) {
			this->left = left;
			if (pSignature) {
				pSignature->SetXpos(this->left);
			}
        }*/
	}

    virtual void getTop(rho::apiGenerator::CMethodResult& oResult) {
        //oResult.set( this->top );
	}

    virtual void setTop( int top, rho::apiGenerator::CMethodResult& oResult) {
        /*if (top >= 0) {
			this->top = top;
			if (pSignature) {
				pSignature->SetYpos(this->top);
			}
        }*/
	}

    virtual void getWidth(rho::apiGenerator::CMethodResult& oResult) {
        //oResult.set(width);
	}

    virtual void setWidth( int width, rho::apiGenerator::CMethodResult& oResult) {
        /*if (width >= 0) {
			this->width = width;
			if (pSignature) {
				pSignature->SetWidth(this->width);
			}
        }*/
	}

    virtual void getHeight(rho::apiGenerator::CMethodResult& oResult) {
        //oResult.set(this->height);
	}

    virtual void setHeight( int height, rho::apiGenerator::CMethodResult& oResult) {
        /*if (height >= 0) {
			this->height = height;
			if (pSignature) {
				pSignature->SetHeight(this->height);
			}
        }*/
	}
    
    virtual void setVectorCallback(rho::apiGenerator::CMethodResult& oResult)
	{

	}

	// Called by Capture method when finished.  This will retrieve all the data as a vector.
	void sendVectors(rho::String &vectors)
	{

	}
};

class CSignatureFactory: public CSignatureFactoryBase
{
    ~CSignatureFactory(){}
	virtual ISignatureSingleton* createModuleSingleton();
};

extern "C" void Init_Signature_extension()
{
    CSignatureFactory::setInstance( new CSignatureFactory() );
    Init_Signature_API();

    RHODESAPP().getExtManager().requireRubyFile("RhoSignatureApi");

}

ISignatureSingleton* CSignatureFactory::createModuleSingleton()
{
    return new CSignatureSingleton();
}

}
