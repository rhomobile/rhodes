#include "WebUrlRequestInterceptor.h"

#include <QDebug>
WebUrlRequestInterceptor::WebUrlRequestInterceptor(QObject *p):QWebEngineUrlRequestInterceptor(p)
{

}
void WebUrlRequestInterceptor::interceptRequest(QWebEngineUrlRequestInfo &info) {
    QString rsrct = "";
    switch(info.resourceType()){
    case 0:rsrct="ResourceTypeMainFrame = 0, // top level page"; break;
    case 1:rsrct="ResourceTypeSubFrame, // frame or iframe";break;
    case 2:rsrct="ResourceTypeStylesheet, // a CSS stylesheet";break;
    case 3:rsrct="ResourceTypeScript, // an external script";break;
    case 4:rsrct="ResourceTypeImage, // an image (jpg/gif/png/etc)";break;
    case 5:rsrct="ResourceTypeFontResource, // a font";break;
    case 6:rsrct="ResourceTypeSubResource, // an other subresource.";break;
    case 7:rsrct="ResourceTypeObject, // an object (or embed) tag for a plugin,";break;
    case 8:rsrct="ResourceTypeMedia, // a media resource.";break;
    case 9:rsrct="ResourceTypeWorker, // the main resource of a dedicated worker.";break;
    case 10:rsrct="ResourceTypeSharedWorker, // the main resource of a shared worker.";break;
    case 11:rsrct="ResourceTypePrefetch, // an explicitly requested prefetch";break;
    case 12:rsrct="ResourceTypeFavicon, // a favicon"; info.block(true); return; break;
    case 13:rsrct="ResourceTypeXhr, // a XMLHttpRequest";break;
    case 14:rsrct="ResourceTypePing, // a ping request for <a ping>";break;
    case 15:rsrct="ResourceTypeServiceWorker, // the main resource of a service worker.";break;
    case 16:rsrct="ResourceTypeUnknown";break;

    default : rsrct="Unknown";break;
    }

  qDebug()<<"\t"<<Q_FUNC_INFO<<":\n\t\t" << "WebUrlRequestInterceptor::interceptRequest    " <<info.requestMethod()
         <<"\r\n  "<<info.requestUrl()<<"   "<<rsrct      <<"\r\n";

}
