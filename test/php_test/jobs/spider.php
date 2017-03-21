<?php

$msg = file_get_contents('1.log');
$ids = array();
preg_match_all('/f_viewpost1\((.*)\)/i', $msg, $ids);
$ids = $ids[1];
$ids = array_unique($ids);
print_r($ids);
require_once 'SnsNetwork.php';

$host = 'zhaopin.oa.com';
$url = 'http://10.14.12.90/resume/pages/sysconfig/ViewRecruitPost.aspx';
//
$cookie = file_get_contents('cookie.txt');
$cookie = trim($cookie, "\n");
$method = 'get';

SnsNetwork::setHeaderHost($host);
SnsNetwork::addHeader('User-Agent: Mozilla/5.0 (Windows NT 6.1; rv:7.0) Gecko/20100101 Firefox/7.0');
SnsNetwork::addHeader('Pragma: no-cache');
SnsNetwork::addHeader('X-Requested-With: XMLHttpRequest');
SnsNetwork::addHeader('X-MicrosoftAjax: Delta=true');
SnsNetwork::addHeader('Cache-Control: no-cache, no-cache');
SnsNetwork::addHeader('Referer: http://zhaopin.oa.com/resume/pages/sysconfig/InnerMarketPostList.aspx');
SnsNetwork::addHeader('Content-Type: application/x-www-form-urlencoded; charset=utf-8');

//print_r($params);
//print_r($cookie);
//exit;
//
unlink("2.log");
$i = 0;
foreach ($ids as $id)
{
    $params = array();
    $params = array(
        'recruitpostid' => $id,
    );

    usleep(1000000);
    $i++;
    if ($i > 5)
    {
        //exit;
    }


$ret = SnsNetwork::makeRequest($url, $params, $cookie, $method);
error_log($ret['msg'], 3, "2.log");

print_r($ret);
print_r("index : $i\n");

}
