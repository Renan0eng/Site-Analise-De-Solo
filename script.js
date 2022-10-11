
let temps = [29.32, 26.54]
var id = 0

function CamaraFria(){
    id = 0
    var Titulo = document.querySelector("#Titulo").innerHTML = "CAMARA FRIA" ;
    var Titulo = document.querySelector("#Temperaturas").innerHTML = `Temp: &#8451; ${temps[id]}`;
}
function TunelDeCongelamento(){
    id = 1
    var Titulo = document.querySelector("#DD").innerHTML = `<h1 id=\"Titulo\">TUNEL DE CONGELAMENTO</h1><div id=\"Temperatura\">Temperatura: &#8451; ${temps[id]}</div><h2>COMTROLER</h2><form method=\"get\"><input type=\"checkbox\" name=\"P4\" value=\"4\" id=\"pino4\">DISPLAY<br><br><button type=\"submit\">ENVIAR</button></form>`;
}
function Atl(){

}
