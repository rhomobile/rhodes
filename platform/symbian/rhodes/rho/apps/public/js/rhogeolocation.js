function getGeoLocation() {
	$.get("/system/geolocation",function(data){
		data = data.split(";");
		if (data.length == 3) {
			$("geolocation").text(data[0]);
			$("geolatitude").text(data[1]);
			$("geolongitude").text(data[2]);
		}
	});
	setTimeout("getGeoLocation()",5000);	
}

$(document).ready(function(){getGeoLocation();});

