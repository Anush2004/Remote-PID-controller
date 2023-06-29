var interval = 900000;

function reset() {
    localStorage.endTime = +new Date + interval;
}

function onTimesUp() {
    clearInterval(timerInterval);
}

if (!localStorage.endTime) {
    reset();
}

function blinker() {
    var d = document.getElementById("timer");
    d.style.color = (d.style.color == 'red' ? 'white' : 'red');
    setTimeout('blinker()', 900);
}

function profile() {

    localStorage.removeItem("endTime");
    window.location.href = "./../profile";
}

setInterval(function () {
    var remaining = localStorage.endTime - new Date;
    if (remaining >= 0) {
        minutes = Math.floor(remaining / 60000)
        seconds = Math.floor((remaining / 1000) % 60)
        if (seconds < 10) {
            seconds = `0${seconds}`;
        }
        if (minutes < 10) {
            document.getElementById("timer").style.color = `#ffa500`;
            minutes = `0${minutes}`;
        }
        else {
            document.getElementById('timer').style.color = "green";
        }
        if (minutes == "00" && seconds < 60) {
            document.getElementById('timer').style.color = "red";
            document.getElementById('timer').style.fontSize = "40px";
        }
        if (minutes == "00" && seconds < 15) {
            blinker();
        }
        //colorchange(minutes,seconds)
        document.getElementById("timer").innerHTML = `${minutes}:${seconds}`
    }
    else {
        localStorage.clear();
        window.location.href = "./../profile";
    }
}, 1000);


function changeColor() {
    const body = document.body
    const rbgValue = document.getElementById('rgbValue');
    const red = document.getElementById('rangeRed')
    const green = document.getElementById('rangeGreen')
    const blue = document.getElementById('rangeBlue')

    let r = red.value;
    let g = green.value;
    let b = blue.value;
    //   let bkgdClr = `kp=${r} ki=${g} kd=${b}`
    kp.innerHTML = `kp = ${r}`;
    ki.innerHTML = `ki = ${g}`;
    kd.innerHTML = `kd = ${b}`;
}
