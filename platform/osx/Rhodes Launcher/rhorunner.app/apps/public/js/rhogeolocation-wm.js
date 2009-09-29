var xmlHttpReq = null;

function setTag(tag,value) {
	if (document.getElementsByTagName) {
		var el=document.getElementsByTagName(tag);
		for (i=0;i<el.length;i++) {
			el[i].innerText = value;
  		}
	}
}

function doHttpReadyStateChange() {
	if (xmlHttpReq.readyState == 4) {
		if (xmlHttpReq.status == 200) {
      		var data = xmlHttpReq.responseText;
      		if (data != null) {
				data = data.split(";");
				setTag("geolatitude",data[1]);
				setTag("geolongitude",data[2]);
				setTag("geolocation",data[0]);
				//alert("setTimeout");
			    setTimeout(getGeoLocation,5000);
			}
		}
	}
}

function ajaxCall(url)
{
        //Construct an XMLHTTP Object to handle our HTTP Request
        if (xmlHttpReq != null){
			xmlHttpReq = null;
		}

        if (xmlHttpReq == null){
        	try {
            	xmlHttpReq = new ActiveXObject("Msxml2.XMLHTTP");
        	} catch (err) {
            	alert("Can't make the call using Msxml2.XMLHTTP");
            	try {
                	xmlHttpReq = new ActiveXObject("Microsoft.XMLHTTP");
				} catch (err2) {
            		alert("Can't make the call - xmlHttpReq is not available");
            		return;
            	}
        	}
		}
		//alert("about to open xmlHttpReq");
   		xmlHttpReq.open("GET", url, true);
        xmlHttpReq.onreadystatechange = doHttpReadyStateChange;
		//alert("about to send Req");
   		xmlHttpReq.send();
}

function getGeoLocation() {
	ajaxCall("/system/geolocation");
}

getGeoLocation();
