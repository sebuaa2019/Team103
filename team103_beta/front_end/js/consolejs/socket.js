// <!-- 作者：周尚纯 & 陈麒先 -->
// <!-- 编写时间：2019.5.23 -->
// <!-- 发布时间：2019.5.28 -->
// <!-- 最终更新时间：2019.6.9 -->
let socket = new WebSocket("ws://192.168.43.229:8000");
// let socket = new WebSocket("ws://127.0.0.1:8000");
// begin Chris.Chen //
let cnt = 1; // 任务ID计数器
// 功能：解析服务器发送的机器人状态数据并显示与任务列表
function addNewLine(data) {

    let tbody = document.getElementsByTagName('tbody')[0];
    let row = document.createElement('tr');
    let curNode = document.createElement('td');
    let taskStatus = document.createElement('td');
    let startTime = document.createElement('td');
    let taskID = document.createElement('td');

    let statusLabel = document.createElement('label');

    // curNode.innerHTML = data;
    // 设置任务节点与任务节点执行状态
    statusLabel.classList.add('badge');
    if (data == '1') {
        curNode.innerHTML = '跟随状态';
        statusLabel.classList.add('badge-gradient-success');
        statusLabel.innerHTML = '跟随成功';
    }
    else if (data == '2') {
        curNode.innerHTML = '问询状态';
        statusLabel.classList.add('badge-gradient-warning');
        statusLabel.innerHTML = '等待指令';
    }
    else if (data == '3') {
        curNode.innerHTML = '前往目标';
        statusLabel.classList.add('badge-gradient-info');
        statusLabel.innerHTML = '正在前往';
    }
    else if (data == '4') {
        curNode.innerHTML = '抓取目标';
        statusLabel.classList.add('badge-gradient-primary');
        statusLabel.innerHTML = '开始抓取';
    }
    else if (data == '5') {
        curNode.innerHTML = '返回起点';
        statusLabel.classList.add('badge-gradient-success');
        statusLabel.innerHTML = '抓取成功';
    }
    else if (data == '6') {
        curNode.innerHTML = '物品递给';
        statusLabel.classList.add('badge-gradient-success');
        statusLabel.innerHTML = '递给成功';
    }
    else {
        curNode.innerHTML = '未知状态';
        statusLabel.classList.add('badge-gradient-danger');
        statusLabel.innerHTML = '状态出错';
    }
    taskStatus.appendChild(statusLabel);


    // 设置时间
    let myDate = new Date();
    let y = myDate.getFullYear();    // 获取完整的年份(4位,1970-????)
    let m = myDate.getMonth() + 1;       // 获取当前月份(0-11,0代表1月)
    let d = myDate.getDate();        // 获取当前日(1-31)
    let hrs = myDate.getHours();       // 获取当前小时数(0-23)
    let min = myDate.getMinutes();     // 获取当前分钟数(0-59)
    let sec = myDate.getSeconds();     // 获取当前秒数(0-59)

    startTime.innerHTML = y + '-' + m + '-' + d + ' ' + hrs + ':' + min +':' + sec;

    // 设置任务号
    taskID.innerHTML = 'KL-' + cnt;
    cnt++;

    row.appendChild(curNode);
    row.appendChild(taskStatus);
    row.appendChild(startTime);
    row.appendChild(taskID);
    console.log(tbody);
    tbody.appendChild(row);
}
// end Chris.Chen //

socket.onopen = function () {
    /* 与服务器端连接成功后，自动执行 */

    let conSuccess = document.getElementsByClassName('connection-success')[0];
    let conFail = document.getElementsByClassName('connection-fail')[0];
    let conWait = document.getElementsByClassName('connection-wait')[0];
    console.log(conSuccess);
    conSuccess.style.display = "block";
    conFail.style.display = "none";
    conWait.style.display = "none";
    console.log("connect succeed!");
};

socket.onmessage = function (event) {
    /* 服务器端向客户端发送数据时，自动执行 */
    let response = event.data;
    addNewLine(response);
    console.log(response);
};

socket.onclose = function (event) {
    /* 服务器端主动断开连接时，自动执行 */
    let conSuccess = document.getElementsByClassName('connection-success')[0];
    let conFail = document.getElementsByClassName('connection-fail')[0];
    let conWait = document.getElementsByClassName('connection-wait')[0];
    console.log(conSuccess);
    conSuccess.style.display = "none";
    conFail.style.display = "block";
    conWait.style.display = "none";
    console.log("on close!");
};

// 核心功能函数：启动、急停、恢复、显示节点信息
function startMsg() {
    socket.send("start");
    console.log("startMsg()start");
}

function stopMsg() {
    socket.send("stop");
    console.log("stopMsg()start");
}

function resumeMsg() {
    socket.send("resume");
    console.log("resumeMsg()start");
}

function quitMsg() {
    socket.send("quit");
    console.log("resumeMsg()start");
}

// 断开连接
function closeConn() {
    socket.close();
    console.log("close connection!");
}
function cleanScreen() {
    let f = document.getElementById("content");
    let childs = f.childNodes;
    for (let i = childs.length - 1; i >= 0; i--) {
        f.removeChild(childs[i]);
    }
}

function testInit(){
    let button = document.getElementsByClassName('begin')[0];
    console.log(button);
    button.onclick = function(){
        let timer = setInterval(function(){
            if(cnt == 7){
                clearInterval(timer);
            }
            else{
                addNewLine(cnt % 7 + "");
            }
            
        },1000)
    }
}

// testInit();