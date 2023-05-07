
#include "pgmspace.h"
// internet interface page html
static const char PROGMEM INDEX_HTML[] = R"rawliteral(

<!doctype html>
<html>
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width,initial-scale=1">
    <title>ESP32 OV2460</title>
    <style>
        body {
            font-family: Arial,Helvetica,sans-serif;
            background: #181818;
            color: #EFEFEF;
            font-size: 16px
        }
        h2 {
            font-size: 18px
        }
        section.main {
            display: flex
        }
        #menu,section.main {
            flex-direction: column
        }
        #menu {
            display: none;
            flex-wrap: nowrap;
            min-width: 340px;
            background: #363636;
            padding: 8px;
            border-radius: 4px;
            margin-top: -10px;
            margin-right: 10px;
        }
        #content {
            display: flex;
            flex-wrap: wrap;
            align-items: stretch
        }
        figure {
            padding: 0px;
            margin: 0;
            -webkit-margin-before: 0;
            margin-block-start: 0;
            -webkit-margin-after: 0;
            margin-block-end: 0;
            -webkit-margin-start: 0;
            margin-inline-start: 0;
            -webkit-margin-end: 0;
            margin-inline-end: 0
        }
        figure img {
            display: block;
            width: 100%;
            height: auto;
            border-radius: 4px;
            margin-top: 8px;
        }
        @media (min-width: 800px) and (orientation:landscape) {
            #content {
                display:flex;
                flex-wrap: nowrap;
                align-items: stretch
            }
            figure img {
                display: block;
                max-width: 100%;
                max-height: calc(100vh - 40px);
                width: auto;
                height: auto
            }
            figure {
                padding: 0 0 0 0px;
                margin: 0;
                -webkit-margin-before: 0;
                margin-block-start: 0;
                -webkit-margin-after: 0;
                margin-block-end: 0;
                -webkit-margin-start: 0;
                margin-inline-start: 0;
                -webkit-margin-end: 0;
                margin-inline-end: 0
            }
        }
        section#buttons {
            display: flex;
            flex-wrap: nowrap;
            justify-content: space-between
        }
        #nav-toggle {
            cursor: pointer;
            display: block
        }
        #nav-toggle-cb {
            outline: 0;
            opacity: 0;
            width: 0;
            height: 0
        }
        #nav-toggle-cb:checked+#menu {
            display: flex
        }
        .input-group {
            display: flex;
            flex-wrap: nowrap;
            line-height: 22px;
            margin: 5px 0
        }
        .input-group>label {
            display: inline-block;
            padding-right: 10px;
            min-width: 47%
        }
        .input-group input,.input-group select {
            flex-grow: 1
        }
        .range-max,.range-min {
            display: inline-block;
            padding: 0 5px
        }
        button {
            display: block;
            margin: 5px;
            padding: 0 12px;
            border: 0;
            line-height: 28px;
            cursor: pointer;
            color: #fff;
            background: #6CD9FFF9;
            border-radius: 5px;
            font-size: 16px;
            outline: 0
        }
        button:hover {
            background: rgba(187, 223, 236, 0.98)
        }
        button:active {
            background: #6CD9FFF9
        }
        button.disabled {
            cursor: default;
            background: #a0a0a0
        }
        input[type=range] {
            -webkit-appearance: none;
            width: 100%;
            height: 22px;
            background: #363636;
            cursor: pointer;
            margin: 0
        }
        input[type=range]:focus {
            outline: 0
        }
        input[type=range]::-webkit-slider-runnable-track {
            width: 100%;
            height: 2px;
            cursor: pointer;
            background: #EFEFEF;
            border-radius: 0;
            border: 0 solid #EFEFEF
        }
        input[type=range]::-webkit-slider-thumb {
            border: 1px solid rgba(0,0,30,0);
            height: 22px;
            width: 22px;
            border-radius: 50px;
            background: #6CD9FFF9;
            cursor: pointer;
            -webkit-appearance: none;
            margin-top: -11.5px
        }
        input[type=range]:focus::-webkit-slider-runnable-track {
            background: #EFEFEF
        }
        input[type=range]::-moz-range-track {
            width: 100%;
            height: 2px;
            cursor: pointer;
            background: #EFEFEF;
            border-radius: 0;
            border: 0 solid #EFEFEF
        }
        input[type=range]::-moz-range-thumb {
            border: 1px solid rgba(0,0,30,0);
            height: 22px;
            width: 22px;
            border-radius: 50px;
            background: #6CD9FFF9;
            cursor: pointer
        }
        input[type=range]::-ms-track {
            width: 100%;
            height: 2px;
            cursor: pointer;
            background: 0 0;
            border-color: transparent;
            color: transparent
        }
        input[type=range]::-ms-fill-lower {
            background: #EFEFEF;
            border: 0 solid #EFEFEF;
            border-radius: 0
        }
        input[type=range]::-ms-fill-upper {
            background: #EFEFEF;
            border: 0 solid #EFEFEF;
            border-radius: 0
        }
        input[type=range]::-ms-thumb {
            border: 1px solid rgba(0,0,30,0);
            height: 22px;
            width: 22px;
            border-radius: 50px;
            background: #6CD9FFF9;
            cursor: pointer;
            height: 2px
        }
        input[type=range]:focus::-ms-fill-lower {
            background: #EFEFEF
        }
        input[type=range]:focus::-ms-fill-upper {
            background: #363636
        }
        .switch {
            display: block;
            position: relative;
            line-height: 22px;
            font-size: 16px;
            height: 22px
        }
        .switch input {
            outline: 0;
            opacity: 0;
            width: 0;
            height: 0
        }
        .slider {
            width: 50px;
            height: 22px;
            border-radius: 22px;
            cursor: pointer;
            background-color: grey
        }
        .slider,.slider:before {
            display: inline-block;
            transition: .4s
        }
        .slider:before {
            position: relative;
            content: "";
            border-radius: 50%;
            height: 16px;
            width: 16px;
            left: 4px;
            top: 3px;
            background-color: #fff
        }
        input:checked+.slider {
            background-color: #6CD9FFF9
        }
        input:checked+.slider:before {
            -webkit-transform: translateX(26px);
            transform: translateX(26px)
        }
        select {
            border: 1px solid #363636;
            font-size: 14px;
            height: 22px;
            outline: 0;
            border-radius: 5px
        }
        .image-container {
            position: relative;
            min-width: 160px
        }
        .close {
            position: absolute;
            right: 5px;
            top: 5px;
            background: #6CD9FFF9;
            width: 16px;
            height: 16px;
            border-radius: 100px;
            color: #fff;
            text-align: center;
            line-height: 18px;
            cursor: pointer
        }
        .hidden {
            display: none
        }
    </style>
    <script src='https:\/\/fustyles.github.io/webduino/TensorFlow/Face-api/face-api.min.js'></script>
</head>
<body>
ESP32-CAM IP：<input type="text" id="ip" size="20" value="192.168.">&nbsp;&nbsp;<input type="button" value="Set" onclick="start();">
<figure>
    <div id="stream-container" class="image-container hidden">
        <div class="close" id="close-stream">×</div>
        <img id="stream" src="" style="display:none" >
        <canvas id="canvas" width="0" height="0"></canvas>
    </div>
</figure>
<section class="main">
    <div id="logo">
        <label for="nav-toggle-cb" id="nav-toggle">&#9776;&nbsp;&nbsp;Toggle OV2640 settings</label>
    </div>
    <div id="content">
        <div id="sidebar">
            <input type="checkbox" id="nav-toggle-cb" checked="checked">
            <nav id="menu">
                <section id="buttons">
                    <button id="restart">Restart board</button>
                    <button id="stop-still">Stop</button>
                    <button id="get-still">Get Still</button>
                    <button id="toggle-stream" style="display:none">Start Stream</button>
                </section>
                <div class="input-group" id="uart-group">
                    <label for="uart">Smile Detector</label>
                    <div class="switch">
                        <input id="uart" type="checkbox" class="default-action" checked="checked">
                        <label class="slider" for="uart"></label>
                    </div>
                </div>
                <div class="input-group" id="sobel-group">
                    <label for="sobel">Motion Detector</label>
                    <div class="switch">
                        <input id="sobel" type="checkbox" class="default-action" checked="checked">
                        <label class="slider" for="sobel"></label>
                    </div>
                </div>
                <div class="input-group" id="probability-group">
                    <label for="probability">probability</label>
                    <div class="range-min">0</div>
                    <input type="range" id="probability" min="0" max="1" value="0" step="0.1" class="default-action">
                    <div class="range-max">1</div>
                </div>
                <div class="input-group" id="flash-group">
                    <label for="flash">Flash</label>
                    <div class="range-min">0</div>
                    <input type="range" id="flash" min="0" max="255" value="0" class="default-action">
                    <div class="range-max">255</div>
                </div>
                <div class="input-group" id="framesize-group">
                    <label for="framesize">Resolution</label>
                    <select id="framesize" class="default-action">
                        <option value="10">UXGA(1600x1200)</option>
                        <option value="9">SXGA(1280x1024)</option>
                        <option value="8">XGA(1024x768)</option>
                        <option value="7">SVGA(800x600)</option>
                        <option value="6">VGA(640x480)</option>
                        <option value="5" selected="selected">CIF(400x296)</option>
                        <option value="4">QVGA(320x240)</option>
                        <option value="3">HQVGA(240x176)</option>
                        <option value="0">QQVGA(160x120)</option>
                    </select>
                </div>
                <div class="input-group" id="quality-group">
                    <label for="quality">Quality</label>
                    <div class="range-min">10</div>
                    <input type="range" id="quality" min="10" max="63" value="10" class="default-action">
                    <div class="range-max">63</div>
                </div>
                <div class="input-group" id="brightness-group">
                    <label for="brightness">Brightness</label>
                    <div class="range-min">-2</div>
                    <input type="range" id="brightness" min="-2" max="2" value="0" class="default-action">
                    <div class="range-max">2</div>
                </div>
                <div class="input-group" id="contrast-group">
                    <label for="contrast">Contrast</label>
                    <div class="range-min">-2</div>
                    <input type="range" id="contrast" min="-2" max="2" value="0" class="default-action">
                    <div class="range-max">2</div>
                </div>
                <div class="input-group" id="saturation-group">
                    <label for="saturation">Saturation</label>
                    <div class="range-min">-2</div>
                    <input type="range" id="saturation" min="-2" max="2" value="0" class="default-action">
                    <div class="range-max">2</div>
                </div>
                <div class="input-group" id="special_effect-group">
                    <label for="special_effect">Special Effect</label>
                    <select id="special_effect" class="default-action">
                        <option value="0" selected="selected">No Effect</option>
                        <option value="1">Negative</option>
                        <option value="2">Grayscale</option>
                        <option value="3">Red Tint</option>
                        <option value="4">Green Tint</option>
                        <option value="5">Blue Tint</option>
                        <option value="6">Sepia</option>
                    </select>
                </div>
                <!--div class="input-group" id="hmirror-group">
                    <label for="hmirror">H-Mirror</label>
                    <div class="switch">
                        <input id="hmirror" type="checkbox" class="default-action" checked="checked">
                        <label class="slider" for="hmirror"></label>
                    </div>
                </div-->
                <!--div class="input-group" id="vflip-group">
                    <label for="vflip">V-Flip</label>
                    <div class="switch">
                        <input id="vflip" type="checkbox" class="default-action" checked="checked">
                        <label class="slider" for="vflip"></label>
                    </div>
                </div-->
                <div class="input-group" id="servo-group">
                    <label for="servo">Servo</label>
                    <div class="range-min">0</div>
                    <input type="range" id="servo" min="0" max="180" value="90" class="default-action">
                    <div class="range-max">180</div>
                    <select id="pinServo" width="30"><option value="2" selected>IO2</option><option value="12">IO12</option><option value="13">IO13</option><option value="14">IO14</option><option value="15">IO15</option></select>
                </div>
                <div class="input-group" id="relay-group">
                    <label for="relay">Relay</label>
                    <div class="switch">
                        <input id="relay" type="checkbox" class="default-action" checked="checked">
                        <label class="slider" for="relay"></label>
                    </div>
                    <select id="pinRelay" width="30"><option value="2">IO2</option><option value="12">IO12</option><option value="13" selected>IO13</option><option value="14">IO14</option><option value="15">IO15</option></select>
                </div>
            </nav>
        </div>
    </div>
</section>
Result：<input type="checkbox" id="chkResult" checked>
<div id="message" style="color:red">Please wait for loading model.<div>

    <script>
        //法蘭斯影像辨識
        const aiView = document.getElementById('stream')
        const aiStill = document.getElementById('get-still')
        const canvas = document.getElementById('canvas')
        var context = canvas.getContext("2d");
        const message = document.getElementById('message');
        const uart = document.getElementById('uart');
        const sobel = document.getElementById('sobel');
        const chkResult = document.getElementById('chkResult');
        const probability = document.getElementById('probability')
        var res = "";
        //Model: https://github.com/fustyles/webduino/tree/master/TensorFlow/Face-api
        const modelPath = 'https://fustyles.github.io/webduino/TensorFlow/Face-api/';
        let currentStream;
        let displaySize = { width:320, height: 240 }
        let faceDetection;

        Promise.all([
            faceapi.nets.tinyFaceDetector.load(modelPath),
            faceapi.nets.faceLandmark68TinyNet.load(modelPath),
            faceapi.nets.faceRecognitionNet.load(modelPath),
            faceapi.nets.faceExpressionNet.load(modelPath),
            faceapi.nets.ageGenderNet.load(modelPath)
        ]).then(function(){
            message.innerHTML = "";
            aiStill.click();
        })

        async function DetectImage() {
            canvas.setAttribute("width", aiView.width);
            canvas.setAttribute("height", aiView.height);
            context.drawImage(aiView, 0, 0, aiView.width, aiView.height);
            if (!chkResult.checked) message.innerHTML = "";

            const detections = await faceapi.detectAllFaces(canvas, new faceapi.TinyFaceDetectorOptions()).withFaceLandmarks(true).withFaceExpressions().withAgeAndGender()
            const resizedDetections = faceapi.resizeResults(detections, displaySize)
            faceapi.draw.drawDetections(canvas, resizedDetections)
            faceapi.draw.drawFaceLandmarks(canvas, resizedDetections)
            faceapi.draw.drawFaceExpressions(canvas, resizedDetections)
            resizedDetections.forEach(result => {
                const { detection,expressions,gender,genderProbability,age } = result
                //message.innerHTML = JSON.stringify(result);
                res = "";
                var i=0;
                var maxEmotion="neutral";
                var maxProbability=expressions.neutral;
                if (expressions.happy>maxProbability) {
                    maxProbability=expressions.happy;
                    maxEmotion="happy";
                }
                if (expressions.sad>maxProbability) {
                    maxProbability=expressions.sad;
                    maxEmotion="sad";
                }
                if (expressions.angry>maxProbability) {
                    maxProbability=expressions.angry;
                    maxEmotion="angry";
                }
                if (expressions.fearful>maxProbability) {
                    maxProbability=expressions.fearful;
                    maxEmotion="fearful";
                }
                if (expressions.disgusted>maxProbability) {
                    maxProbability=expressions.disgusted;
                    maxEmotion="disgusted";
                }
                if (expressions.surprised>maxProbability) {
                    maxProbability=expressions.surprised;
                    maxEmotion="surprised";
                }
                if (uart.checked) {
                    //當可能性最大的表情是happy時
                    if (maxEmotion=="happy"&&maxProbability>=Number(probability.value)) {
                        var query = document.location.origin+'?uart='+maxEmotion;
                        fetch(query)
                            .then(response => {
                                console.log(`request to ${query} finished, status: ${response.status}`)
                            })
                    }
                }

                res+= i+",age,"+Math.round(age)+",gender,"+gender+",genderProbability,"+Math.round(genderProbability)+",emotion,"+maxEmotion+",neutral,"+Math.round(expressions.neutral)+",happy,"+Math.round(expressions.happy)+",sad,"+Math.round(expressions.sad)+",angry,"+Math.round(expressions.angry)+",fearful,"+Math.round(expressions.fearful)+",disgusted,"+Math.round(expressions.disgusted)+",surprised,"+Math.round(expressions.surprised)+",boxX,"+Math.round(detection._box._x)+",boxY,"+Math.round(detection._box._y)+",boxWidth,"+Math.round(detection._box._width)+",boxHeight,"+Math.round(detection._box._height)+"<br>";
                i++;
                new faceapi.draw.DrawTextField(
                    [
                        `${faceapi.round(age, 0)} years`,
                        `${gender} (${faceapi.round(genderProbability)})`
                    ],
                    result.detection.box.bottomRight
                ).draw(canvas)
            })

            if (chkResult.checked) message.innerHTML = res;
            aiStill.click();
        }

        aiView.onload = function (event) {
            try {
                document.createEvent("TouchEvent");
                setTimeout(function(){DetectImage();},250);
            } catch(e) {
                setTimeout(function(){DetectImage();},150);
            }
        }

        //官方式函式
        function start() {
            var baseHost = 'http://'+document.getElementById("ip").value;  //var baseHost = document.location.origin

            const hide = el => {
                el.classList.add('hidden')
            }

            const show = el => {
                el.classList.remove('hidden')
            }

            const disable = el => {
                el.classList.add('disabled')
                el.disabled = true
            }

            const enable = el => {
                el.classList.remove('disabled')
                el.disabled = false
            }

            const updateValue = (el, value, updateRemote) => {
                updateRemote = updateRemote == null ? true : updateRemote
                let initialValue
                if(!el) return;
                if (el.type === 'checkbox') {
                    initialValue = el.checked
                    value = !!value
                    el.checked = value
                } else {
                    initialValue = el.value
                    el.value = value
                }

                if (updateRemote && initialValue !== value) {
                    updateConfig(el);
                }
            }

            function updateConfig (el) {
                let value
                switch (el.type) {
                    case 'checkbox':
                        value = el.checked ? 1 : 0
                        break
                    case 'range':
                    case 'select-one':
                        value = el.value
                        break
                    case 'button':
                    case 'submit':
                        value = '1'
                        break
                    default:
                        return
                }

                if (el.id =="flash") {  //新增flash自訂指令
                    var query = baseHost+"?flash=" + String(value);
                } else if (el.id =="servo") {  //新增servo自訂指令
                    var query = baseHost+"?servo=" + pinServo.value + ";" + String(value);
                } else if (el.id =="relay") {  //新增繼電器自訂指令
                    var query = baseHost+"?relay=" + pinRelay.value + ";" + Number(relay.checked);
                } else if (el.id =="uart") {  //新增uart自訂指令
                    return;
                } else if (el.id =="probability") {  //新增probability自訂指令
                    return;
                } else {
                    var query = `${baseHost}/?${el.id}=${value}`
                }

                fetch(query)
                    .then(response => {
                        console.log(`request to ${query} finished, status: ${response.status}`)
                    })
            }

            document
                .querySelectorAll('.close')
                .forEach(el => {
                    el.onclick = () => {
                        hide(el.parentNode)
                    }
                })

            const view = document.getElementById('stream')
            const viewContainer = document.getElementById('stream-container')
            const stillButton = document.getElementById('get-still')
            //const enrollButton = document.getElementById('face_enroll')
            const closeButton = document.getElementById('close-stream')
            const stopButton = document.getElementById('stop-still')            //新增stopButton變數
            const restartButton = document.getElementById('restart')            //新增restart變數
            const flash = document.getElementById('flash')                      //新增flash變數
            const servo = document.getElementById('servo')                      //新增servo變數
            const pinServo = document.getElementById('pinServo');               //新增servo pin變數
            const relay = document.getElementById('relay')                      //新增relay變數
            const pinRelay = document.getElementById('pinRelay');               //新增relay pin變數
            const uart = document.getElementById('uart')                        //新增uart變數


            stopButton.onclick = function (event) {
                window.stop();
                message.innerHTML = "";
            }

            // Attach actions to buttons
            stillButton.onclick = () => {
                view.src = `${baseHost}/?getstill=${Date.now()}`
                show(viewContainer);
            }

            closeButton.onclick = () => {
                hide(viewContainer)
            }

            //新增重啟電源按鈕點選事件 (自訂指令格式：http://192.168.xxx.xxx/?cmd=P1;P2;P3;P4;P5;P6;P7;P8;P9)
            restartButton.onclick = () => {
                fetch(baseHost+"/?restart");
            }

            // Attach default on change action
            document
                .querySelectorAll('.default-action')
                .forEach(el => {
                    el.onchange = () => updateConfig(el)
                })

            framesize.onchange = () => {
                updateConfig(framesize)
            }

            // read initial values
            fetch(`${baseHost}/?status`)
                .then(function (response) {
                    return response.json()
                })
                .then(function (state) {
                    document
                        .querySelectorAll('.default-action')
                        .forEach(el => {
                            if (el.id=="flash") {  //新增flash設定預設值0
                                flash.value=0;
                                var query = baseHost+"?flash=0";
                                fetch(query)
                                    .then(response => {
                                        console.log(`request to ${query} finished, status: ${response.status}`)
                                    })
                            } else if (el.id=="servo") {  //新增servo設定預設值90度
                                servo.value=90;
                                /*
                                var query = baseHost+"?servo=" + pinServo.value + ";90";
                                fetch(query)
                                  .then(response => {
                                    console.log(`request to ${query} finished, status: ${response.status}`)
                                  })
                                */
                            } else if (el.id=="relay") {  //新增relay設定預設值0
                                relay.checked = false;
                                /*
                                var query = baseHost+"?relay=" + pinRelay.value + ";0";
                                fetch(query)
                                  .then(response => {
                                    console.log(`request to ${query} finished, status: ${response.status}`)
                                  })
                                */
                            } else if (el.id=="uart") {  //新增uart設定預設值0
                                uart.checked = false;
                            } else if (el.id=="probability") {  //新增probability設定預設值0
                                probability.value = 0;
                            } else {
                                updateValue(el, state[el.id], false)
                            }
                        })
                })
        }

        //  網址/?192.168.1.38  可自動帶入?後參數IP值
        var href=location.href;
        if (href.indexOf("?")!=-1) {
            ip.value = location.search.split("?")[1].replace(/http:\/\//g,"");
            start();
        }
        else if (href.indexOf("http")!=-1) {
            ip.value = location.host;
            start();
        }

    </script>
</body>
</html>


)rawliteral";