function sendStrToServer() {
    var s = document.getElementById('calcArea').value;
    if ("WebSocket" in window) {
        // alert("WebSocket is supported by your Browser!");
        // Let us open a web socket
        var ws = new WebSocket("ws://149.28.39.220:2065");
        ws.onopen = function () {
            // Web Socket is connected, send data using send()
            ws.send(s);
            // alert("Message is sent...");
            // alert(s);
        };
        ws.onmessage = function (evt) {
            var received_msg = evt.data;
            // alert("Message is received...");
            document.getElementById('resultArea').value = received_msg;
            document.getElementById('resultArea').hidden = true;
        };
        ws.onclose = function () {
            // websocket is closed.
            // alert("Connection is closed...");
        };
        
    } else {
        // The browser doesn't support WebSocket
        alert("WebSocket NOT supported by your Browser!");
    }
}