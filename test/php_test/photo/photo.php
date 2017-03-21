<?php

/**
 * OpenAPI V3 SDK 示例代码，适用于大部分OpenAPI。如果是上传文件类OpenAPI，请参考本SDK包中的“Test_UploadFile.php”文件中的示例代码。
 *
 * @version 3.0.4
 * @author open.qq.com
 * @copyright © 2012, Tencent Corporation. All rights reserved.
 * @History:
 *               3.0.4 | coolinchen | 2012-09-07 10:20:12 | initialization
 */


//require_once './../v3/OpenApiV3.php';
require_once '/home/nemo/code/isd_qzoneappbase_proj/application/platform/openapi_v3/tools/sdk/php/v3/OpenApiV3.php';

// 应用基本信息
//$appid = 10883;
//$appkey = '408eb8a7ca6b47c8a7a9c5c04bc5f4f5';
$appid = 100680016;
$appkey = '5c9d412f7a776f3dddd3823a81b72ed5';

// OpenAPI的服务器IP 
// 最新的API服务器地址请参考wiki文档: http://wiki.open.qq.com/wiki/API3.0%E6%96%87%E6%A1%A3 
$server_name = '10.166.146.171';


// 用户的OpenID/OpenKey
$openid = trim("AE1CB53D4E51E2AA270D0BEEFE7CF3BF");
$openkey = trim("547DE8721781A4B2DCF21085F69263CF");
$pf = 'qzone';


$sdk = new OpenApiV3($appid, $appkey);
$sdk->setServerName($server_name);

$albums = get_albums($sdk, $openid, $openkey, $pf);

$head = <<<EOF
<head>
<meta http-equiv="content-type" content="text/html; charset=GBK">
</head>
EOF;

/*
            [3] => Array
                (
                    [desc] => 健身一个星期, 57.5KG
                    [hd_existed] => 1
                    [hd_image] => Array
                        (
                            [height] => 1600
                            [size] => 687974
                            [url] => http://or.photo.store.qq.com/psb?/V117BSg23DQFXG/juRzYiELbZpR4tL8MC5bQW.u86nljYzE0xfmGjky1*M!/o/dEftn0FTIQAA/gyoAAA!!&bo=VgIgA6sEqwQBAFA!
                            [width] => 1195
                        )

                    [large_image] => Array
                        (
                            [height] => 800
                            [url] => http://o110.photo.store.qq.com/psb?/V117BSg23DQFXG/juRzYiELbZpR4tL8MC5bQW.u86nljYzE0xfmGjky1*M!/b/dEftn0FTIQAA/gyoAAA!!&bo=VgIgA6sEqwQBAFA!
                            [width] => 598
                        )

                    [lloc] => NDJ0Tmn9AYzPsVD..P4ER.2fQVMhAAA!
                    [name] => 102APPLE_IMG_2961
                    [sloc] => NDJ0Tmn9AYzPsVD..P4ER.2fQVMhAAA!
                    [small_url] => http://o110.photo.store.qq.com/psb?/V117BSg23DQFXG/juRzYiELbZpR4tL8MC5bQW.u86nljYzE0xfmGjky1*M!/a/dEftn0FTIQAA/gyoAAA!!&bo=VgIgA6sEqwQBAFA!
                    [updated_time] => 1353830284
                    [uploaded_time] => 1353830284
                )
*/
$cnt = 1;
foreach ($albums['album'] as $album)
{
    $albumid = $album['albumid'];
    $name = $album['name'];

    //print_r($album);
    //if ($albumid != 'de7b6e1b-874e-4299-ac05-68117f0add45')
    //{
        //continue;
    //}

    $photos = get_photos($sdk, $openid, $openkey, $pf, $albumid);

    //print_r($photos);

    print_r("cnt : $cnt\n");

    $html = $head;
    $s_html = $head;
    foreach ($photos['photos'] as $v)
    {
        $url = $v['large_image']['url'];
        if ($v['hd_existed'] == 1)
        {
            $url = $v['hd_image']['url'];
        }

        $s_url = $v['small_url'];

        $html .= "</br>name:{$v['name']}</br>\n";
        $html .= "<img src=\"{$url}\"></img>\n";

        $s_html .= "</br>name:{$v['name']}</br>\n";
        $s_html .= "<img src=\"{$s_url}\"></img>\n";
    }
    
    error_log($html, 3, "/home/nemo/119/photos/{$cnt}_[{$name}]_{$albumid}.html"); 
    error_log($s_html, 3, "/home/nemo/119/photos/s_{$cnt}_[{$name}]_{$albumid}.html"); 
    //exit();
    $cnt ++;

}
echo "cnt : $cnt\n";

/**
 * 获取好友资料
 *
 * @param object $sdk OpenApiV3 Object
 * @param string $openid openid
 * @param string $openkey openkey
 * @param string $pf 平台
 * @return array 好友资料数组
 */
function get_albums($sdk, $openid, $openkey, $pf)
{
    global $appid;
	$params = array(
		'openid' => $openid,
		'openkey' => $openkey,
		'pf' => $pf,
        'oauth_consumer_key' => $appid,
	);
	
	$script_name = '/photo/list_album';
	return $sdk->api($script_name, $params,'post');
}

function get_photos($sdk, $openid, $openkey, $pf, $albumid)
{
    global $appid;
	$params = array(
		'openid' => $openid,
		'openkey' => $openkey,
		'pf' => $pf,
        'oauth_consumer_key' => $appid,
        'albumid' => $albumid,
	);
	
	$script_name = '/photo/list_photo';
	return $sdk->api($script_name, $params,'post');
	
	
}
// end of script
