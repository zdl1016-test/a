<?php
    $sock = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP);

    //$svr_ip = "127.0.0.1";
    //$svr_ip = "172.16.197.186";
    $svr_ip = "10.130.2.105";
    $svr_port = 9999; 

    $msg1 = "0123456789";
    $msg2 = '';
    for ($i=0; $i<1024*7; $i++)
    {
        $msg2 .= $msg1;
    }


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
    exit;

    $from_ip = '';
    $from_port = 0;
    $recv_buf = '';
    $recv_len = socket_recvfrom($sock, $recv_buf, 1024, 0, $from_ip, $from_port);
    if ($recv_len === FALSE)
    {
        echo "socket_recvfrom failed. ".socket_strerror(socket_last_error()) . PHP_EOL;
        exit;
    }

    echo "formip:$from_ip, form_port:$from_port, data_len:$recv_len, data:$recv_buf".PHP_EOL;

    socket_close($sock);
?> 
