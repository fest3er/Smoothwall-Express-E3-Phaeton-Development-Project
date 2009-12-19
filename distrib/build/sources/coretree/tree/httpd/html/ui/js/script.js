function MM_preloadImages() {
  var d=document; if(d.images){ if(!d.MM_p) d.MM_p=new Array();
    var i,j=d.MM_p.length,a=MM_preloadImages.arguments; for(i=0; i<a.length; i++)
    if (a[i].indexOf("#")!=0){ d.MM_p[j]=new Image; d.MM_p[j++].src=a[i];}}
}

function MM_swapImgRestore() {
  var i,x,a=document.MM_sr; for(i=0;a&&i<a.length&&(x=a[i])&&x.oSrc;i++) x.src=x.oSrc;
}

function MM_findObj(n, d) {
  var p,i,x;  if(!d) d=document; if((p=n.indexOf("?"))>0&&parent.frames.length) {
    d=parent.frames[n.substring(p+1)].document; n=n.substring(0,p);}
  if(!(x=d[n])&&d.all) x=d.all[n]; for (i=0;!x&&i<d.forms.length;i++) x=d.forms[i][n];
  for(i=0;!x&&d.layers&&i<d.layers.length;i++) x=MM_findObj(n,d.layers[i].document); return x;
}

function MM_swapImage() {
  var i,j=0,x,a=MM_swapImage.arguments; document.MM_sr=new Array; for(i=0;i<(a.length-2);i+=3)
   if ((x=MM_findObj(a[i]))!=null){document.MM_sr[j++]=x; if(!x.oSrc) x.oSrc=x.src; x.src=a[i+2];}
}

function displayHelp(url) {
	window.open("/cgi-bin/help.cgi?"+url,"disposableHelpWindow","resizable=yes,status=no,scrollbars=yes,width=400,height=500");
}

/* Validation functions and related options */

function _disable(field)
{
	if ( document.getElementById(field) ){
		document.getElementById(field).disabled = true;
	}
}

function _enable(field)
{
	if ( document.getElementById(field) ){
		document.getElementById(field).disabled = false;
	}
}

function _error(field)
{
	if ( document.getElementById(field) ){
		document.getElementById(field).style.backgroundColor = '#FFdddd';
	}
}

function _ok(field)
{
	if ( document.getElementById(field) ){
		document.getElementById(field).style.backgroundColor = 'white';
	}
}
	

function portlist( selectf, inputf, enabledon, allowblank )
{
	var selectval = document.getElementById(selectf).value;
	var inputval  = document.getElementById(inputf).value;

	if ( selectval == enabledon ){
		_enable(inputf);
		validport(inputf,allowblank);
	} else {
		_ok(inputf);
		_disable(inputf);
	}
}

function validport(field,allowblank)
{
	var inputval = document.getElementById(field).value;
	/* check it for validity */
	var errored = !/^[\d:]+$/.test(inputval);
	if(!errored) {
		errored = (inputval < 1 || inputval > 0xFFFF);
	}
	if( inputval == "" && allowblank == true){
		errored = 0;
	}

	if ( errored ){
		_error(field);
	} else {
		_ok(field);
	}

	return errored;
}

function validip(field,blank)
{
	var address = field;
	if ( document.getElementById(field) ){
		address = document.getElementById(field).value;
	}
	var valid = true;

	if ( address == "" && blank == 'true' ){
		valid = true;
	} else {
		var numbers = address.split( "." );

		if ( numbers.length != 4 ){
			valid = false;
		}
		
		for ( var number = 0 ; number < 4 ; number++ ){			
			if ( ! numbers[ number ] ){
				valid = false;
				break;
			}		
			
			for ( var character = 0 ; character < numbers[ number ].length ; character++ ){
				if ( 
					( numbers[ number ].charAt( character ) < '0' ) ||
					( numbers[ number ].charAt( character ) > '9' ) ){
					valid = false;
					break;
				}
			}
			
			if (( numbers[ number ] < 0 ) || ( numbers[ number ] > 255 )){
				valid = false;
			}
		}
	}

	if ( valid ){
		_ok(field);
	} else {
		_error(field);
	}

	return valid;
}

function validmask(field, blank)
{
	var mask = field;
	if ( document.getElementById(field) ){
		mask = document.getElementById(field).value;
	}
	var valid = true;
	
	if ( mask == "" && blank == 'true' ){
		valid = true;
	} else {
		// is it a valid ip ?
		if ( validip( field, blank ) ){
			valid = true;
		} else if ( mask > 0 && mask <= 32 ){
			valid = true;
		} else {
			valid = false;
		}
	}

	if ( valid ){
		_ok( field );
	} else {
		_error( field );
	}
	return valid;
}

function validipormask(field, blank)
{
	var ipormask = document.getElementById(field).value;
	var valid = true;
	
	if ( ipormask == "" && blank == 'true' ){
		valid = true;
	} else {
		// is it an ip only ?
		if ( validip( field, blank ) ){
			valid = true;
		} else {
			/* split it into a number and a mask */
			var detail_finder = new RegExp( /^(.*)\/(.*)$/ );
			var matches = detail_finder.exec( ipormask );
			if ( !matches ){
				valid = false;
			} else {
				if ( !validip(matches[1]) ){
					valid = false;
				} else {
					valid = validmask(matches[2]);
				}
			}
		}
	}

	if ( valid ){
		_ok( field );
	} else {
		_error( field );
	}
	return valid;
}

function validipandmask(field, blank)
{
	var ipandmask = document.getElementById(field).value;
	var valid = true;
	
	if ( ipandmask == "" && blank == 'true' ){
		valid = true;
	} else {
		/* split it into a number and a mask */
		var detail_finder = new RegExp( /^(.*?)\/(.*?)$/ );
		var matches = detail_finder.exec( ipandmask );
		if ( !matches ){
			valid = false;
		} else {
			if ( !validip(matches[1]) ){
				valid = false;
			} else {
				valid = validmask(matches[2]);
			}
		}
	}

	if ( valid ){
		_ok( field );
	} else {
		_error( field );
	}
	return valid;
}

function validportrange( field, blank )
{	
	var port = document.getElementById(field).value;
	var valid = false;
	
	if ( validport(field, blank) ){
		valid = true;
	} else {
		var detail_finder = new RegExp( /^(.*):(.*)$/ );
		var matches = detail_finder.exec( port );
		if ( validport( matches[1] ) && validport( matches[2] ) ){
			if ( matches[1] < matches[2] ){
				valid = true;
			}
		}
	}

	if ( valid ){
		_ok( field );
	} else {
		_error( field );
	}
	return valid;
}


function validmac( field, blank )
{	
	var inputval = document.getElementById(field).value;
	var valid = false;

	var errored = !/^[0-9a-fA-F]{2}[\:\-][0-9a-fA-F]{2}[\:\-][0-9a-fA-F]{2}[\:\-][0-9a-fA-F]{2}[\:\-][0-9a-fA-F]{2}[\:\-][0-9a-fA-F]{2}$/.test(inputval);

	if(errored) {
		valid = true;
	}
	if( inputval == "" && blank == true){
		valid = true;
	}

	if ( valid ){
		_error(field);
	} else {
		_ok(field);
	}
}


function validcomment( field, blank )
{	
	var inputval = document.getElementById(field).value;
	var valid = false;

	var errored = !/^[\w\d\.\-,\(\)\@£\$!\%\^\&\*=\+_ ]*$/.test(inputval);

	if(errored) {
		valid = true;
	}
	if( inputval == "" && blank == true){
		valid = true;
	}

	if ( valid ){
		_error(field);
	} else {
		_ok(field);
	}
}


function validhostname( field, blank )
{	
	var inputval = document.getElementById(field).value;
	var valid = true;

	var parts = name.split(".");

    	for(var part in parts) {
		// if we only consist of a-z0-9 and - and each part has at least one non number
		if(!(/^[\w\d-]+$/.test(parts[part]) && /[^\d]+/.test(parts[part]))) { 
			valid = false;
		}
	}

	if ( valid ){
		_error(field);
	} else {
		_ok(field);
	}
	return valid;
}

function validnumber( field,lower, upper, allowempty )
{
	var value = document.getElementById(field).value;
	var valid = false;
	if( value == undefined || value == "" ) {
		valid = allowempty;
	} else {
		if ( value >= lower && value <= upper ){
			valid = false;
		} else {
			valid = true;
		}
	}
	if ( valid ){
		_error(field);
	} else {
		_ok(field);
	}
	return valid;
}

function validregex(field,regex,blank)
{
	var inputval = document.getElementById(field).value;
	var valid = false;

	if( inputval == undefined || inputval == ""){
		valid = !blank;
	} else {
		var re = new RegExp( regex );
		var ma = re.exec( inputval );

		if ( ma != null ){
			valid = false;
		} else {
			valid = true;
		}
	}
	
	if ( valid ){
		_error(field);
	} else {
		_ok(field);
	}
	return valid;
}

function validpassword(field, field2, regex,blank)
{
	var inputval = document.getElementById(field).value;
	var inputval2 = document.getElementById(field2).value;
	var valid = false;

	if( inputval == undefined || inputval == ""){
		valid = !blank;
	} else {
		var re = new RegExp( regex );
		var ma = re.exec( inputval );

		if ( ma != null ){
			valid = false;
		} else {
			valid = true;
		}
	}

	if ( inputval != inputval2 ){
		valid = true;
	}
	
	if ( valid ){
		_error(field);
		_error(field2);
	} else {
		_ok(field);
		_ok(field2);
	}
	return valid;
}






