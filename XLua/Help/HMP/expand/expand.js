// globals
var closed = "expand/images/pluscold.gif";			//image used for collapsed item in callExpand()
var closedHot = "expand/images/plushot.gif";			//hot image used for collapsed item in callExpand()
var expand = "expand/images/minuscold.gif";			//image used for expanded item in callExpand()
var expandHot = "expand/images/minushot.gif";		//hot image used for expanded item in callExpand()

function callExpand(iIndex) {
     if (document.all("ExpandDiv" + iIndex).style.display == "block")
	      doCollapse(iIndex);
     else doExpand(iIndex);
}

// Returns expand image to hot. 
function expandGoesHot(iIndex){
   if (document.all("ExpandDiv" + iIndex).style.display == "block") document.all("ExpandImage" + iIndex).src = expandHot;
   else document.all("ExpandImage" + iIndex).src = closedHot;
}

// Returns expand image to cold.
function expandGoesCold(iIndex){
 if (document.all("ExpandDiv" + iIndex).style.display == "block") document.all("ExpandImage" + iIndex).src = expand;
  else document.all("ExpandImage" + iIndex).src = closed;  
}


//  Expands expandable block & changes image
function doExpand(iIndex) {
	document.all("ExpandImage" + iIndex).src = expand;
	document.all("ExpandDiv" + iIndex).style.display = "block";	
}


//  Collapses expandable block & changes image	
function doCollapse(iIndex) {
    document.all("ExpandDiv" + iIndex).style.display = "none";
    document.all("ExpandImage" + iIndex).src = closed;
}
