import XHR from "xmlhttprequest";

(function() {
   let xhr = new XHR.XMLHttpRequest();
   xhr.open("GET", "http://127.0.0.1:9000/get", false);
   xhr.onreadystatuschange = function() {
       if(xhr.readyState == 4 && xhr.state == 200)
       {
           console.log(xhr.responseText);
       }
   };
   xhr.send();
})();
