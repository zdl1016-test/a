<?php
    $sock = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP);

    $svr_ip = "119.29.151.192";
    $svr_port = 10000; 

    $msg = "hello";
    $len = strlen($msg);

    $total_ms = 0;
    $diff_ms = 0;

    for ($i=0; $i<100; $i++)
    {
        $g_req_start_usec = 0;
        $g_req_start_sec = 0;
        list($g_req_start_usec, $g_req_start_sec) = explode(" ", microtime());

        $ret = socket_sendto($sock, $msg, $len, 0, $svr_ip, $svr_port);

        $from_ip = '';
        $from_port = 0;
        $recv_buf = '';



        $recv_len = socket_recvfrom($sock, $recv_buf, 1024, 0, $from_ip, $from_port);

        $g_req_start_usec2 = 0;
        $g_req_start_sec2 = 0;
        list($g_req_start_usec2, $g_req_start_sec2) = explode(" ", microtime());

        $diff_sec = $g_req_start_sec2-$g_req_start_sec;
        $diff_usec = $g_req_start_usec2-$g_req_start_usec;
        $diff_ms = ($diff_sec+$diff_usec) * 1000;
        $total_ms += $diff_ms;
    echo "formip:$from_ip, form_port:$from_port, data_len:$recv_len, data:$recv_buf".PHP_EOL;
    }
    echo "last diff_ms:$diff_ms\n";
    $avg_ms = $total_ms / 100;
    echo "avg_diff_ms:$avg_ms\n";

    if ($recv_len === FALSE)
    {
        echo "socket_recvfrom failed. ".socket_strerror(socket_last_error()) . PHP_EOL;
        exit;
    }

    echo "formip:$from_ip, form_port:$from_port, data_len:$recv_len, data:$recv_buf".PHP_EOL;

    socket_close($sock);
?> 
