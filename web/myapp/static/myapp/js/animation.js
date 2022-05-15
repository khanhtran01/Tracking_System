let on_off_stt = 1

function opensidebar(){
    document.getElementById("leftside").style.width = "20%";
}
function closesidebar(){
    document.getElementById("leftside").style.width = "4%";

    document.getElementById("rightside").style.width = "96%";
    leftelement = document.getElementById("leftside");
    
}
function changestatus(){
    var left = document.getElementById("leftside");
    var right = document.getElementById("rightside");
    if (on_off_stt == 1){
        // left.className += " closesidebar";
        // right.className += " closesidebar";
        left.style.width = "4%";
        right.style.width= "96%";

        ele = left.getElementsByClassName("menutext");
        ele1 = left.getElementsByClassName("menutitle");
        ele2 = left.getElementsByClassName("rightsideselection")
        for (var i = 0 ; i < ele.length; i++){
            ele[i].style.display = "none";
        }
        for (var i = 0 ; i < ele1.length; i++){
            ele1[i].style.display = "none";
        }
        for (var i = 0 ; i < ele2.length; i++){
            ele2[i].style.display = "none";
        }
        document.getElementById("groupName").style.display = "none";
        on_off_stt = 0
        console.log("1")
    }
    else {
        console.log("0")
        // left.classList.remove("closesidebar");
        // right.classList.remove("closesidebar");
        ele = left.getElementsByClassName("menutext");
        ele2 = left.getElementsByClassName("rightsideselection")
        ele1 = left.getElementsByClassName("menutitle");
        for (var i = 0 ; i < ele.length; i++){
            ele[i].style.display = "block";
        }
        for (var i = 0 ; i < ele1.length; i++){
            ele1[i].style.display = "block";
        }
        for (var i = 0 ; i < ele2.length; i++){
            ele2[i].style.display = "block";
        }
        document.getElementById("groupName").style.display = "block";
        left.style.width = "20%";
        right.style.width= "80%";
        on_off_stt = 1
    }
}

function openModal(){
    var modalbg = document.getElementById("trafficLightModal");
    var modal = document.getElementById("Modal");
    modalbg.style.display = "flex";
    setTimeout(()=>{
        modal.classList.add("openModal");
    },100)
    
    // modal.classList.add("openModal");
}
function openMap(){
    var modalbg = document.getElementById("mapModal");
    var modal = document.getElementById("map");
    modalbg.style.display = "flex";
    setTimeout(()=>{
        modal.classList.add("openMap");
    },100)
}

function openSetModal(){
    var modalbg = document.getElementById("settrafficLightModal");
    var modal = document.getElementById("setModal");
    modalbg.style.display = "flex";
    setTimeout(()=>{
        modal.classList.add("openSetmodal");
    },100)
}

function closeMap(){
    var modalbg = document.getElementById("mapModal");
    var modal = document.getElementById("map");
    modal.classList.remove("openMap");
    setTimeout(()=>{
        modalbg.style.display = "none";
    },1000)
}
function closeSetModal(){
    var modalbg = document.getElementById("settrafficLightModal");
    var modal = document.getElementById("setModal");
    modal.classList.remove("openSetmodal");
    setTimeout(()=>{
        modalbg.style.display = "none";
    },1000)
}
function closeModal(){
    var modalbg = document.getElementById("trafficLightModal");
    var modal = document.getElementById("Modal");
    modal.classList.remove("openModal");
    setTimeout(()=>{
        modalbg.style.display = "none";
        // modal.style.display = "none";
    }, 1000)
}