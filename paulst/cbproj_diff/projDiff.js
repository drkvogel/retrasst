
function log(str) {
  var dbgObj = document.getElementById("dbg");
  if(dbgObj) {
    dbgObj.innerHTML = dbgObj.innerHTML + "<br />" +  str;
  }
}

function loadXmlFile(url) {  
  log("loadXmlFile(" + url + ")");
  var request = new XMLHttpRequest();
  request.open("GET", url, false);
  request.send(null);
  return request.responseXML;
}

function convertFilePathToUrl(filePath) {
  var re = /\\/g;
  var url = "file://" + filePath.replace(re, "/");
  return url;
}

var processor = new XSLTProcessor();

function doDiff( file1, file2, divID ) {
    var xml = loadXmlFile(file1);
    processor.setParameter(null, "other", file2);
    var html = processor.transformToFragment(xml, document);
    var div = document.getElementById(divID);
    removeAllChildren(div);
    var heading = document.createElement("h1");
    heading.appendChild( document.createTextNode( file1 + " v " + file2 ) );
    div.appendChild( heading );
    div.appendChild(html);
}

function diff(evt) {
    var file1 = document.getElementById("file1").value;
    var file2 = document.getElementById("file2").value;
    doDiff( file1, file2, "1v2" );
    doDiff( file2, file1, "2v1" );
}

function openWindowFor( controlID )
{
    var file = document.getElementById(controlID).value;
    window.open( file, '_blank', 'location=yes,height=570,width=520,scrollbars=yes,status=yes' );
}

function init() {
    processor.importStylesheet(loadXmlFile("xslt.xml"));
    var diffButton = document.getElementById("diff");
    diffButton.onclick = diff;
    var vf1Button = document.getElementById("vf1");
    vf1Button.onclick = function(event){ openWindowFor('file1'); };
    var vf2Button = document.getElementById("vf2");
    vf2Button.onclick = function(event){ openWindowFor('file2'); };
}
 
function removeAllChildren(documentElement) {
  while(documentElement.hasChildNodes()){
    documentElement.removeChild(documentElement.childNodes[0]);
  }
}

