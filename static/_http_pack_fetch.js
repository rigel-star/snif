(function() {
   while(true)
   {
       let xhr = new XMLHttpRequest();
       xhr.open("GET", "http://127.0.0.1:9000/get", false);
       xhr.onreadystatechange = () => {
           if(xhr.readyState == XMLHttpRequest.DONE && xhr.status == 200)
           {
               self.postMessage(xhr.responseText);
           }
       };
       xhr.send();
   }
})();
