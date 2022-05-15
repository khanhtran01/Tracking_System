function tabsto(index){
    var listboard = document.getElementsByClassName("board");
    
        curr = listboard[index];
        for (var i = 0; i < listboard.length; i++){
            listboard[i].classList.remove("activate");
        }
        curr.classList.add("activate");
    
}