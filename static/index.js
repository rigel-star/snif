let list = document.getElementById("pack-list-ul");

const fetcher = new Worker("./static/_http_pack_fetch.js");
fetcher.addEventListener("message", (msg) => {
   let newPackItem = document.createElement("li");
   newPackItem.classList.add("pack-list-ul-li");
   newPackItem.innerHTML = msg.data;
   list.appendChild(newPackItem);
});
