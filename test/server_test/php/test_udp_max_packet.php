<?php
$sock = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP);

$svr_ip = "10.130.2.105";
$svr_port = 9999; 

// 构造一个70KB长度的字符串
$msg1 = "0123456789";
$msg2 = '';
for ($i=0; $i<1024*7; $i++)
{
    $msg2 .= $msg1;
}

// 从63KB开始尝试发, 看是否发送失败
$send_len = 63*1024;
while (1)
{
    $msg = substr($msg2, 0, $send_len);
    $len = strlen($msg);

    print_r("will send data len : $len ...\n");

    $ret = socket_sendto($sock, $msg, $len, 0, $svr_ip, $svr_port);
    if ($ret == false)
    {
        print_r("$len failed.");
        exit;
    }
    $send_len ++;

    //print_r($msg);

    print_r("socket_sendto return : $ret\n");

}

socket_close($sock);
