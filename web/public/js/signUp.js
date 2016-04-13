window.addEventListener("load", function() {
	document.getElementById("signUpForm").addEventListener("submit", function(ev) {
		var errmsg = document.getElementById("errmsg");
		errmsg.style.display = "none";
		if(document.getElementById("password").value != 
			document.getElementById("verifyPassword").value) {
			ev.preventDefault();
			errmsg.style.display = "inline";
		}
	});
});
