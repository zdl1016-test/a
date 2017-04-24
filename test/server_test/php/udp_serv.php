<?php
error_reporting(E_ALL | E_STRICT);

$svr_ip = '0.0.0.0';
$svr_port = '9999';

$sock = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP);

$ret = socket_bind($sock, $svr_ip, $svr_port);
if ($ret === FALSE)
{
    echo "socket_bind failed. ".socket_strerror(socket_last_error()) . PHP_EOL;
    socket_close($sock);
    exit;
}

while (1)
{
    $recv_len_max = 1024;
    $from_ip = '';
    $from_port = 0;
    $recv_len = socket_recvfrom($sock, $recv_buf, $recv_len_max, 0, $from_ip, $from_port);
    if ($recv_len === FALSE)
    {
        echo "socket_recvfrom failed. ".socket_strerror(socket_last_error()) . PHP_EOL;
        socket_close($sock);
        exit;
    }

    echo "formip:$from_ip, form_port:$from_port, data_len:$recv_len, data:$recv_buf".PHP_EOL;

    $send_len = socket_sendto($sock, $recv_buf, strlen($recv_buf), 0, $from_ip, $from_port);
    if ($send_len != strlen($recv_buf))
    {
        echo "socket_sendto failed. ret:{$send_len}, msg:".socket_strerror(socket_last_error()) . PHP_EOL;
        socket_close($sock);
        exit;
    }

}
socket_close($sock);

echo "game over.".PHP_EOL;
return 0;

