<?php

$pat_arr = array(
    '<tr><th class="">招聘岗位</th><td>',
    '<tr><th>部门:</th><td>',
    '<tr><th>组织单元:</th><td>',
    '<tr><th>职位:</th><td>',
    '<tr><th>工作地:</th><td>',
    '<tr><th>员工类型:</th><td>',
    '<tr><th>更新时间:</th><td>',
    '<tr><th>岗位要求:</th><td>',
);

$info = file_get_contents('2.log');
unlink('3.log');
$pos = 0;
$pos2 = 0;
while(1)
{
$jb = '';
foreach ($pat_arr as $v)
{
    $pos = strpos($info, $v, $pos2);
    if ($pos === FALSE)
    {
        echo "find [$v] in failed.\n";
        exit;
    }
    $pos += strlen($v);

    $pos2 = strpos($info, '</t', $pos);
    if ($pos === FALSE)
    {
        echo "find [$v][</td>] in failed.\n";
        exit;
    }

    $str = substr($info, $pos, $pos2-$pos);
    $jb = "$jb$str|";
    echo "$str";
    echo "\n";
}
error_log($jb."\n", 3, "3.log");

}
