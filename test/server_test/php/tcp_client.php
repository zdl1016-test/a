<?php

/**
 * $ip: 地址
 * $service_port: 端口
 * $msg : 信息数据
 * $proto : 信息id
 */
function connect_socket($ip, $service_port, $msg, $proto)
{
    error_reporting(E_ALL);

    $socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);

    if ($socket === FALSE) {
        print_r("socket创建失败原因: " . socket_strerror($socket) . "\n");
        socket_close($socket);
        return false;
    } 

    socket_set_option($socket,SOL_SOCKET,SO_RCVTIMEO,array("sec"=>3,"usec"=>0));
    socket_set_option($socket,SOL_SOCKET,SO_SNDTIMEO,array("sec"=>3,"usec"=>0));

    $result = socket_connect($socket, $ip, $service_port);

    if ($result === FALSE) {
        print_r("SOCKET连接失败原因: " . socket_strerror($result) . "\n");
        socket_close($socket);
        return false;
    } 

    $data=json_encode($msg);
    $data = $data . "\0";
    $in = pack("v",$proto);
    $in .= pack("I",strlen($data)+10);
    //$in .= pack("I",strlen($data)+1000000000);
    $in .= pack("I",0);
    $in .= $data;

    socket_write($socket, $in, strlen($in));
    $out=socket_read( $socket, 200*1024);
    if($out === false)
    {
        print_r("socket read fail, reason : " . socket_strerror(socket_last_error($socket)));
        return false;
    }
    $result = parse_result($out);
    socket_close($socket);
    return  $result;
}

function parse_result($binary)
{
    $msg_id = unpack("v",substr($binary, 0, 2));

    $result_type = unpack("I",substr($binary, 10, 4));

    if($result_type[1]||$result_type[1] === 0)
    {
        // -1 是为了去掉\0结束符
        $result_data = substr($binary, 14, strlen($binary)-14-1);
        return json_decode($result_data, true);
    }

    return false;
}

$result = connect_socket("127.0.0.1", "4405", array(), 1);
var_dump($result);
