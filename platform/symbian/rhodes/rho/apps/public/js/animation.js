function startAnimation(index) {
  if (document.getElementById) {
    var img = document.getElementById('animation');
    if (img = document.getElementById('animation'))
      img.src = "/public/images/animation/aa-" + index + ".gif";
  }

  var nextIndex = index + 1;
  if (nextIndex >= 4)
    nextIndex = 0;
	setTimeout("startAnimation(" + nextIndex + ")", 300);	
}

