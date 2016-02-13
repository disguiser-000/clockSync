<?php
/**
 * Clock Synchronization
 * Created by free-disguise.
 * Date: 2016-02-03
 * Time: 13:48
 */
    date_default_timezone_set('Asia/Chongqing');

    $url = "http://adash.m.taobao.com/rest/sur";
    $data = array();
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_HEADER, 0);
    curl_setopt($ch, CURLOPT_TIMEOUT, 60);
    curl_setopt($ch, CURLOPT_POST, 1);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $data);
    $content = curl_exec($ch);
//        $errorCode = curl_errno($ch);
    $status = curl_getinfo($ch, CURLINFO_HTTP_CODE);
    curl_close($ch);
    
    if ($status == 200) {
        echo "Connect to time server.\n";
        //echo "status:".$status."\n";
        $tbTime = json_decode($content, true);
        $timestamp = substr($tbTime['t'],0,strlen($tbTime['t'])-3);
        $day = date("Y-m-d", $timestamp);
        $time = date("H:i:s", $timestamp);
        $cmd1 = "date {$day}";
        $cmd2 = "time {$time}";
        //echo $cmd1."\n";
        echo "Update Date => ";
        passthru($cmd1);
        echo "\n";
        //echo $cmd2."\n";
        echo "Update Time => ";
        passthru($cmd2);
        echo "\n";
//        echo "Clock Synchronization Success.";
    } else {
        echo "Clock Synchronization Failure.[{$status}]";
    }
?>