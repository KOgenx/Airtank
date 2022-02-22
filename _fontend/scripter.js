//Set background color depending on CO2
function setBG(hue, sat, li, offset) {
   let root = document.documentElement;
   root.style.setProperty('--hue', hue);
   root.style.setProperty('--sat', sat);
   root.style.setProperty('--light', li);
   root.style.setProperty('--light-offset', offset);
}

setBG('20', '100%', '60%', '30%');
if (@@C02@@ < 1000) { setBG('127', '50%', '40%', '15%'); };
if (@@C02@@ > 1400) { setBG('360', '70%', '40%', '15%'); };

// Get time 
let today = new Date();
let time = "UPDATED: " + today.getHours() + ":" + today.getMinutes() + ":" + today.getSeconds();
document.getElementById("date").innerHTML = time;
