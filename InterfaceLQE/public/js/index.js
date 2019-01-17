
var socket = io('http://localhost:8080');

let btn_amarelo = document.querySelector('.amarelo');
let btn_verde = document.querySelector('.verde');
let btn_vermelho = document.querySelector('.vermelho');

btn_amarelo.addEventListener("click", ()=>{envia('amarelo')});
btn_verde.addEventListener("click", ()=>{envia('verde')});
btn_vermelho.addEventListener("click", ()=>{envia('vermelho')});

let ctx = document.getElementById("mychart");
let label = ["Red", "Blue", "Yellow", "Green", "Purple", "Orange", "Nelson"];
var myChart = new Chart(ctx, {
    type: 'line',
    data: {
        labels: label,
        datasets: [{
            label: '# of Votes',
            data: [12, 19, 3, 5, 2, 3],
            backgroundColor: [
                'rgba(255, 99, 132, 0.2)',
                'rgba(54, 162, 235, 0.2)',
                'rgba(255, 206, 86, 0.2)',
                'rgba(75, 192, 192, 0.2)',
                'rgba(153, 102, 255, 0.2)',
                'rgba(255, 159, 64, 0.2)'
            ],
            borderColor: [
                'rgba(255,99,132,1)',
                'rgba(54, 162, 235, 1)',
                'rgba(255, 206, 86, 1)',
                'rgba(75, 192, 192, 1)',
                'rgba(153, 102, 255, 1)',
                'rgba(255, 159, 64, 1)'
            ],
            borderWidth: 1
        }]
    },
    options: {
        scales: {
            yAxes: [{
                ticks: {
                    beginAtZero:true
                }
            }]
        }
    }
});
function envia(texto){
	socket.emit('teste', texto);

	
}
