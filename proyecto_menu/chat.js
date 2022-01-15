// conenction
//io();
let socket = io();
//var socket = io.connect("http://192.168.1.231:3000/");

// DOM elements
let message = document.getElementById('message');
let username = document.getElementById('username');
let btn = document.getElementById('send');
let output = document.getElementById('output');
let actions = document.getElementById('actions');

// btn.addEventListener('click', function() {
//   socket.emit('chat:message', {
//     message: message.value,
//     username: username.value
//   });
// });

// message.addEventListener('keypress', function () {
//   socket.emit('chat:typing', username.value);
// });

/*socket.on('chat:message', function(data) {
  actions.innerHTML = '';
  output.innerHTML += `<p>
    <strong>${data.username}</strong>: ${data.message}
  </p>`
});*/

/*socket.on('chat:typing', function(data) {
  actions.innerHTML =  `<p><em>${data} is typing a message...</em></p>`
});*/


socket.on('event:message', function(data) {
  //const obj = JSON.stringify(data);
  espgestos2(data.action);
  console.log(data.action);
});


var tops = 0;
var lefts = 0;
var cuadrado  = document.getElementById("cuadro");
//var cuadrado  = document.getElementById("menu1");
//var barrera  = document.getElementById("barrera");
var barreraTop = 0;
var barreraLeft= 0;
var barreraBottom= 0;
var barreraRight= 0;
window.onload = function(){
  document.onkeydown = teclas;
  //var barrpos = barrera.getBoundingClientRect();
 // barreraTop = barrpos.top;
 // barreraLeft = barrpos.left;
 // barreraBottom = barrpos.bottom;
 // barreraRight = barrpos.right;

}


function teclas(event){
  var codigo = event.keyCode;
  var cuapos = cuadrado.getBoundingClientRect()
  var cuatop = cuapos.top;
  var cualeft = cuapos.left;
  var cuabottom = cuapos.bottom;
  var cuaright = cuapos.right;

  if (codigo==40){
    //tops++;
    tops = tops + 40;
    cuadrado.style.top = tops+"px";
  }
  if (codigo==38){
    //tops--;
    tops = tops - 40;
    cuadrado.style.top = tops+"px";
  }
  if (codigo==39){
    //lefts++;
    lefts = lefts + 40;
    cuadrado.style.left = lefts+"px";
  }
  if (codigo==37){
    //lefts--;
    lefts = lefts - 40;
    cuadrado.style.left = lefts+"px";
  }

  if (cuaright==barreraLeft && cuabottom>=barreraTop && cuatop<=barreraBottom){
    lefts = lefts -2;
  }
  if (cuabottom==barreraTop && cuaright>=barreraLeft && cualeft<=barreraRight){
    tops = tops-2;
  }
  if (cualeft==barreraRight && cuabottom>=barreraTop && cuatop<=barreraBottom){
    lefts = lefts +2;
  }
  if (cuatop==barreraBottom && cuaright>=barreraLeft && cualeft<=barreraRight){
    tops = tops+2;
  }
}




function espgestos(accion){
  //var codigo = event.keyCode;
  var codigo = accion;
  var cuapos = cuadrado.getBoundingClientRect()
  var cuatop = cuapos.top;
  var cualeft = cuapos.left;
  var cuabottom = cuapos.bottom;
  var cuaright = cuapos.right;

  if (codigo=="DOWN"){
    //tops++;
    tops = tops + 160;
    cuadrado.style.top = tops+"px";
  }
  if (codigo=="UP"){
    //tops--;
    tops = tops - 160;
    cuadrado.style.top = tops+"px";
  }
  if (codigo=="RIGHT"){
    //lefts++;
    lefts = lefts + 160;
    cuadrado.style.left = lefts+"px";
  }
  if (codigo=="LEFT"){
    //lefts--;
    lefts = lefts - 160;
    cuadrado.style.left = lefts+"px";
  }

  if (cuaright==barreraLeft && cuabottom>=barreraTop && cuatop<=barreraBottom){
    lefts = lefts -2;
  }
  if (cuabottom==barreraTop && cuaright>=barreraLeft && cualeft<=barreraRight){
    tops = tops-2;
  }
  if (cualeft==barreraRight && cuabottom>=barreraTop && cuatop<=barreraBottom){
    lefts = lefts +2;
  }
  if (cuatop==barreraBottom && cuaright>=barreraLeft && cualeft<=barreraRight){
    tops = tops+2;
  }
}


const imagen1 = document.getElementById("canvas-1");
const imagen2 = document.getElementById("canvas-2");
const imagen3 = document.getElementById("canvas-3");
const imagen4 = document.getElementById("canvas-4");
const imagen5 = document.getElementById("canvas-5");
const imagen6 = document.getElementById("canvas-6");

//imagen1.style.background='blue';
var puntero=1;

if(puntero===1){
    imagen1.style.backgroundColor='blue';
}


function espgestos2(action){

  if (action==="FAR" || action ==="NEAR"){
    //if (action==="FAR" ){
    alert("Estas seguro de seleccionar");
  }

  if ((action==="5355684513-1")|| (action == "5565564513-1")){
    alert("Menu confirmado");
  }


  if(puntero===1){
        imagen1.style.background='blue';
        switch(action){
            case "RIGHT":
                imagen1.style.background='red';
                imagen2.style.background='blue';
                console.log("Derecha")
                puntero=2;
                console.log(puntero)
            break;

            case "DOWN":
                imagen1.style.background='red';
                imagen4.style.background='blue';
                console.log("Abajo")
                puntero=4;
                console.log(puntero)
            break;
        } 
    }else{
        if(puntero===2){
            switch(action){
                case "LEFT":
                    imagen2.style.background='red';
                    imagen1.style.background='blue';
                    console.log("Izquierda")
                    puntero=1;
                    console.log(puntero)
                break;
    
                case "RIGHT":
                    imagen2.style.background='red';
                    imagen3.style.background='blue';
                    console.log("Derecha")
                    puntero=3;
                    console.log(puntero)
                break;
    
                case "DOWN":
                    imagen2.style.background='red';
                    imagen5.style.background='blue';
                    console.log("Abajo")
                    puntero=5;
                    console.log(puntero)
                break;
            } 
        }else{
            if(puntero===3){
                switch(action){
                    case "LEFT":
                        imagen3.style.background='red';
                        imagen2.style.background='blue';
                        console.log("Izquierda")
                        puntero=2;
                        console.log(puntero)
                    break;
        
                    case "DOWN":
                        imagen3.style.background='red';
                        imagen6.style.background='blue';
                        console.log("Abajo")
                        puntero=6;
                        console.log(puntero)
                    break;
                } 
            }else{
                if(puntero===4){
                    switch(action){
                        case "UP":
                            imagen4.style.background='red';
                            imagen1.style.background='blue';
                            console.log("Arriba")
                            puntero=1;
                            console.log(puntero)
                        break;
            
                        case "RIGHT":
                            imagen4.style.background='red';
                            imagen5.style.background='blue';
                            console.log("Derecha")
                            puntero=5;
                            console.log(puntero)
                        break;
                    } 
                }else{
                    if(puntero===5){
                        switch(action){
                            case "LEFT":
                                imagen5.style.background='red';
                                imagen4.style.background='blue';
                                console.log("Izquierda")
                                puntero=4;
                                console.log(puntero)
                            break;
                            case "UP":
                                imagen5.style.background='red';
                                imagen2.style.background='blue';
                                console.log("Arriba")
                                puntero=2;
                                console.log(puntero)
                            break;
                
                            case "RIGHT":
                                imagen5.style.background='red';
                                imagen6.style.background='blue';
                                console.log("Derecha")
                                puntero=6;
                                console.log(puntero)
                            break;
                        } 
                    }else{
                        if(puntero===6){
                            switch(action){
                                case "RIGHT":
                                    imagen6.style.background='red';
                                    imagen5.style.background='blue';
                                    console.log("Izquierda")
                                    puntero=5;
                                    console.log(puntero)
                                break;
                                case "UP":
                                    imagen6.style.background='red';
                                    imagen3.style.background='blue';
                                    console.log("Arriba")
                                    puntero=3;
                                    console.log(puntero)
                                break;
                            } 
                        }
                    }
                }
            }
        }
    
    }
}