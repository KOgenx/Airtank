const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0">
    <title>@@ehostename@@</title>
    <link rel="icon"
        href="data:image/svg+xml,<svg xmlns=%22http://www.w3.org/2000/svg%22 viewBox=%220 0 100 100%22><text y=%22.9em%22 font-size=%2290%22>🔥</text></svg>">

    <!-- CSS Styles -->
    <style>
        @import"https://fonts.googleapis.com/css2?family=Oxanium:wght@400;800&display=swap";*,*::before,*::after{box-sizing:border-box}body,h1,h2,h3,h4,p,figure,blockquote,dl,dd{margin:0}ul[role=list],ol[role=list]{list-style:none}html:focus-within{scroll-behavior:smooth}body{min-height:100vh;text-rendering:optimizeSpeed;line-height:1.5}a:not([class]){-webkit-text-decoration-skip:ink;text-decoration-skip-ink:auto}img,picture{max-width:100%;display:block}input,button,textarea,select{font:inherit}@media(prefers-reduced-motion: reduce){html:focus-within{scroll-behavior:auto}*,*::before,*::after{-webkit-animation-duration:.01ms !important;animation-duration:.01ms !important;-webkit-animation-iteration-count:1 !important;animation-iteration-count:1 !important;transition-duration:.01ms !important;scroll-behavior:auto !important}}:root{--hue: 127;--sat: 50%;--light: 40%;--light-offset: 15%;--bg-color: hsl(var(--hue), var(--sat), var(--light));--bg-color-dark: hsl(var(--hue), var(--sat), var(--light-offset));--bg: linear-gradient(0deg, var(--bg-color-dark) 0%, var(--bg-color) 100%)}html{background:var(--bg)}body{font-family:"Oxanium",sans-serif;font-weight:800;line-height:1.3;display:flex;justify-content:center;align-items:center;align-content:flex-start;color:#fff;background:var(--bg)}main{flex:0 0 80vw;padding:1rem 0;margin:3rem 0}.main__header h1{letter-spacing:-0.1rem;font-size:clamp(3rem, 0.3000rem + 12.0000vw, 7.5rem);text-transform:uppercase;line-height:1;margin-bottom:.1em}.main__header .updateDate{font-style:italic;font-weight:400;margin-right:1rem;font-size:clamp(1.3rem, 0.5800rem + 3.2000vw, 2.5rem);line-height:0}a.btn{display:block;font-size:clamp(0.8rem, 0.3800rem + 1.8667vw, 1.5rem);border-radius:.5em;font-weight:bold;border:none;text-align:center;color:#fff;background-color:var(--bg-color);padding:.5em;text-decoration:none;width:100%;line-height:1;transition:all 300ms ease-in}a.btn+.btn{margin:1em 0 0 0}.openModal{display:inline-block;float:right;font-size:clamp(0.8rem, 0.3800rem + 1.8667vw, 1.5rem);border-radius:.5em;font-weight:bold;border:none;text-align:center;color:var(--bg-color);background-color:#fff;padding:.5em;text-decoration:none;margin:1em .3em 0 0;cursor:pointer;line-height:1;transition:all 300ms ease-in}.openModal span{font-weight:normal}.openModal:hover,.openModal:focus{background-color:var(--bg-color);color:#fff}.pull-left{float:left}.btn-reload{display:inline-block;width:clamp(1.3rem, 0.5800rem + 3.2000vw, 2.5rem);padding:0;height:clamp(1.3rem, 0.5800rem + 3.2000vw, 2.5rem);background-color:transparent;background-size:contain;border:none;background-repeat:no-repeat;transform:translateY(11%);background-image:url("data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 24 24' width='36' height='36'%3E%3Cpath fill='none' d='M0 0h24v24H0z'/%3E%3Cpath d='M5.463 4.433A9.961 9.961 0 0 1 12 2c5.523 0 10 4.477 10 10 0 2.136-.67 4.116-1.81 5.74L17 12h3A8 8 0 0 0 6.46 6.228l-.997-1.795zm13.074 15.134A9.961 9.961 0 0 1 12 22C6.477 22 2 17.523 2 12c0-2.136.67-4.116 1.81-5.74L7 12H4a8 8 0 0 0 13.54 5.772l.997 1.795z' fill='rgba(255,255,255,1)'/%3E%3C/svg%3E")}.list__row{display:flex;flex-wrap:wrap;justify-content:space-between;align-items:flex-start;align-content:flex-start;font-size:clamp(1.3rem, 0.5800rem + 3.2000vw, 2.5rem);border-bottom:1px solid #fff;padding:.5em 0}.list__row:first-of-type{border-top:1px solid #fff}.list__data{font-weight:400;padding-left:1rem}.list__tag-wrapper{flex-basis:100%;text-align:right;font-size:1.5rem}.tag{-moz-columns:2;columns:2;font-size:1rem;padding:.4em .5em .3em .5em;border-radius:.5em;font-weight:400;color:#fff;border:1px solid #fff}.tag::after{content:"W";background:transparent url("data:image/svg+xml,%3Csvg version='1.1' id='emoji' xmlns='http://www.w3.org/2000/svg' x='0' y='0' viewBox='0 0 55 55' style='enable-background:new 0 0 55 55' xml:space='preserve'%3E%3Cstyle%3E.st0%7Bfill:none;stroke:%23fff;stroke-width:4.5;stroke-linecap:round;stroke-linejoin:round;stroke-miterlimit:10%7D%3C/style%3E%3Cg id='line'%3E%3Cpath class='st0' d='m17 15 6 4-6 4m21-8-6 4 6 4'/%3E%3Cpath class='st0' d='M46 39a23 23 0 1 0-35 2'/%3E%3Cpath class='st0' d='M39 40c2 0 5 2 5 4l-2 3c1 3-1 4-3 5l-3-1-2 2c-2 0-3-1-3-2l-3 2-2-2-3 2-2-2-3 2c-1 0-3-1-3-3l-3-2c0-1 1-3 3-3l-1-2c0-1 1-3 3-3h-1v-3c0-6 5-10 12-10s11 4 11 10v3z'/%3E%3C/g%3E%3C/svg%3E") center center no-repeat;background-size:contain;display:inline-block;color:transparent}.modal{position:fixed;inset:auto;background:#fff;z-index:9999;opacity:0;transition:opacity 400ms ease-in;pointer-events:none;color:#555;min-width:30vw;display:flex;flex-direction:column;flex-wrap:wrap;font-size:1rem}.modal::before{content:"";position:fixed;display:block;top:0;left:0;width:100vw;height:100vh;background-color:rgba(0,0,0,.8);z-index:-1}.modal header{display:flex;justify-content:space-between;background-color:var(--bg-color);color:#fff;padding:1rem;text-transform:uppercase}.modal a:not(.btn){color:#555;border-bottom:1px dotted #000;text-decoration:none}.modal a.close{color:#fff;border:none}.modal .list{background-color:#fff}.modal .list__row{font-size:1rem;border-color:#555;padding:1rem}.modal:target{opacity:1;pointer-events:all}form{display:flex;flex-direction:column;padding:1rem;background-color:#fff}form select,form input{margin-bottom:1rem;font-weight:400;border-radius:0;padding:.5rem;border:1px solid #000;background-color:#fff;color:#000;min-width:300px;height:40px}form em{font-weight:400;font-style:normal}form input[type=submit]{background-color:var(--bg-color);color:#fff;border:0;-webkit-appearance:none;-moz-appearance:none;appearance:none}@media screen and (min-width: 960px){main{flex:0 0 768px}}
    </style>
</head>

<body>
     <main>
        <header class="main__header">
            <time id="date" class="updateDate">UPDATED: 14:51:43</time><button class="btn-reload" onClick="window.location.reload();"></button>
            <h1>AIRTANK</h1>
            
        </header>

        <section class="list">
            <div class="list__row">
                <p class="list__def">
                    CO<small>2</small>
                </p>
                <p class="list__data">
                    @@C02@@ ppm
                </p>
                <p class="list__tag-wrapper">
                    <span class="tag">1000+ = </span>
                </p>
            </div>

            <div class="list__row">
                <p class="list__def">
                    PM<small>1</small>
                </p>
                <p class="list__data">
                    @@PM01@@ µg/m³
                </p>
            </div>

            <div class="list__row">
                <p class="list__def">
                    PM<small>2.5</small>
                </p>
                <p class="list__data">
                    @@PM02@@ µg/m³
                </p>
                <p class="list__tag-wrapper">
                    <span class="tag">20+ = </span>
                </p>
            </div>

            <div class="list__row">
                <p class="list__def">
                    PM<small>10</small>
                </p>
                <p class="list__data">
                    @@PM10@@ µg/m³
                </p>
                <p class="list__tag-wrapper">
                    <span class="tag">40+ = </span>
                </p>
            </div>

            <div class="list__row">
                <p class="list__def">
                    Temperature
                </p>
                <p class="list__data">
                    @@tmp@@ °C
                </p>
            </div>
            <div class="list__row">
                <p class="list__def">
                    Humidity
                </p>
                <p class="list__data">
                    @@hum@@ %
                </p>
            </div>
        </section>
        <a href="@@ledUrl@@" class="pull-left openModal">
            LED <span>@@ledState@@</span>
        </a>

        <a href="#sysInfo" class="openModal">Info</a>
        <a href="#sysSettings" class="openModal">Settings</a>
        <a href="#sysFunctions" class="openModal">Functions</a>
    </main>

    <!-- Sysinfo modal -->
    <section id="sysInfo" class="modal">

        <header>
            <p>System Info</p>
            <a href="#" title="Close" class="close">x</a>
        </header>
        <section class="list">
            <div class="list__row">
                <p class="list__def">
                    Restart reason
                </p>
                <p class="list__data">
                    @@espRestart@@
                </p>
            </div>
            <div class="list__row">
                <p class="list__def">
                    ESP chip ID
                </p>
                <p class="list__data">
                    @@espChipId@@
                </p>
            </div>
            <div class="list__row">
                <p class="list__def">
                    Core version
                </p>
                <p class="list__data">
                    @@espCoreV@@
                </p>
            </div>
            <div class="list__row">
                <p class="list__def">
                    CPU frequency
                </p>
                <p class="list__data">
                    @@espMhz@@ Mhz
                </p>
            </div>
            <div class="list__row">
                <p class="list__def">
                    Sketch size
                </p>
                <p class="list__data">
                    @@espSketch@@ KB
                </p>
            </div>
            <div class="list__row">
                <p class="list__def">
                    JSON address
                </p>
                <p class="list__data">
                    <a href="@@espJson@@">JSON</a>
                </p>
            </div>
            <div class="list__row">
                <p class="list__def">
                    Up Time
                </p>
                <p class="list__data">
                    @@espUptime@@
                </p>
            </div>
        </section>
    </section>

    <!-- SysSettings modal -->
    <section id="sysSettings" class="modal">
        <header>
            <p>System Settings</p>
            <a href="#" title="Close" class="close">x</a>
        </header>
        <form method="post" enctype="application/x-www-form-urlencoded" action="/settings">
            <label for="SwitchOFF">Sleepmode start hour <em>(0 - 23)</em></label>
            <input type="text" name="SwitchOFF" value="@@switchOff@@" inputmode="numeric" pattern="[0-9]*">

            <label for="SwitchON">Sleepmode stop hour <em>(0 - 23)</em></label>
            <input type="text" name="SwitchON" value="@@switchOn@@" inputmode="numeric" pattern="[0-9]*">

            <label for="overWrite">Disable Sleepmode</label>
            <select name="overWrite" id="overWrite">
                <option value="No" @@overwriteNo@@>No</option>
                <option value="Yes" @@overwriteYes@@>Yes</option>
            </select>

            <label for="tempOffset">Temperature offset <em>(0.9 = 90%)</em></label>
            <input type="text" name="tempOffset" value="@@tempOffset@@">

            <input type="submit" value="Save">
        </form>
    </section>

    <section id="sysFunctions" class="modal">
        <header>
            <p>Functions</p>
            <a href="#" title="Close" class="close">x</a>
        </header>
        <section class="list">
            <div class="list__row">
        <a href="/reboot" class="btn">Reboot</a>
        <a href="/displayoff" class="btn">Display Off</a>
        <a href="/displayon" class="btn">Display ON</a>
    </div>    
    </section>



    <script type="text/javascript">
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

        let today = new Date();
        let time = "UPDATED: " + today.getHours() + ":" + today.getMinutes() + ":" + today.getSeconds();
        document.getElementById("date").innerHTML = time;
    </script>
</body>

</html>
)=====";
