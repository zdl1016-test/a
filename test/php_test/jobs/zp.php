<?php

require_once 'SnsNetwork.php';

$host = 'zhaopin.oa.com';
$url = 'http://10.14.12.90/resume/pages/sysconfig/InnerMarketPostList.aspx';
//$url = 'http://zhaopin.oa.com/resume/pages/sysconfig/InnerMarketPostList.aspx';

$params = file_get_contents('params.txt');
$cookie = file_get_contents('cookie.txt');
$cookie = trim($cookie, "\n");
$method = 'post';

//print_r($params);
//print_r($cookie);
//exit;

SnsNetwork::setHeaderHost($host);
SnsNetwork::addHeader('User-Agent: Mozilla/5.0 (Windows NT 6.1; rv:7.0) Gecko/20100101 Firefox/7.0');
SnsNetwork::addHeader('Pragma: no-cache');
SnsNetwork::addHeader('X-Requested-With: XMLHttpRequest');
SnsNetwork::addHeader('X-MicrosoftAjax: Delta=true');
SnsNetwork::addHeader('Cache-Control: no-cache, no-cache');
SnsNetwork::addHeader('Referer: http://zhaopin.oa.com/resume/pages/sysconfig/InnerMarketPostList.aspx');
SnsNetwork::addHeader('Content-Type: application/x-www-form-urlencoded; charset=utf-8');

$pat = 'GotoIndex%2C2';
for ($i=1; $i<200; $i++)
{
    usleep(2000000);
    $new_pat ="GotoIndex%2C{$i}";
    $params2 = str_replace($pat, $new_pat, $params);
    //error_log($params, 3, 'params.log');
    echo "$i : $new_pat\n";
    $ret = SnsNetwork::makeRequest($url, $params2, $cookie, $method);
    if (strlen($ret['msg']) < 1000)
    {
        echo "{$ret['msg']} \n";
        echo "failed. \n";
        exit;
    }
    print_r($ret);
    error_log($ret['msg'], 3, '1.log');
}

