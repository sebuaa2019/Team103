// <!-- 作者：周尚纯 -->
// <!-- 编写时间：2019.5.23 -->
// <!-- 发布时间：2019.5.28 -->
var socket = new WebSocket("ws://127.0.0.1:8000");

socket.onopen = function () {
    /* 与服务器端连接成功后，自动执行 */

    // var newTag = document.createElement('div');
    // newTag.innerHTML = "【连接成功】";
    // document.getElementById('content').appendChild(newTag);
    console.log("connect succeed!");
};

socket.onmessage = function (event) {
    /* 服务器端向客户端发送数据时，自动执行 */
    var response = event.data;
    // var newTag = document.createElement('div');
    // newTag.innerHTML = response;
    // document.getElementById('content').appendChild(newTag);
    console.log(response);
};

socket.onclose = function (event) {
    /* 服务器端主动断开连接时，自动执行 */
    // var newTag = document.createElement('div');
    // newTag.innerHTML = "【关闭连接】";
    // document.getElementById('content').appendChild(newTag);
    console.log("on close!");
};

function sendMsg() {
    // var txt = document.getElementById('txt');
    socket.send("start");
    // txt.value = ""; 
}
function closeConn() {
    socket.close();
    // var newTag = document.createElement('div');
    // newTag.innerHTML = "【关闭连接】";
    // document.getElementById('content').appendChild(newTag);
    console.log("close connection!");
}
function cleanScreen() {
    var f = document.getElementById("content");
    var childs = f.childNodes;
    for (var i = childs.length - 1; i >= 0; i--) {
        //alert(childs[i].nodeName); 
        f.removeChild(childs[i]);
    }
}
