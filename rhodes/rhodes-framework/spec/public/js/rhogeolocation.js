function getGeoLocation() {
	$.get("/system/geolocation",function(data){
		  data = data.split(";");
		  $("geolatitude").text(data[1]);
		  $("geolongitude").text(data[2]);		
		  $("geolocation").text(data[0]);});
	setTimeout("getGeoLocation()",5000);	
}

$(document).ready(function(){getGeoLocation();});

