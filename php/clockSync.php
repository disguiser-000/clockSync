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
    $status = curl_getinfo($ch, CURLINFO_HTTP_CODE);
    curl_close($ch);
    
    if ($status == 200) {
        $tbTime = json_decode($content, true);
        if (isset($tbTime['t']) && strlen($tbTime['t'])==13) {
            $timestamp = (float) substr($tbTime['t'],0,strlen($tbTime['t'])-3);
            if ($timestamp > strtotime("1980-01-01")) {
                $day = date("Y-m-d", $timestamp);
                $time = date("H:i:s", $timestamp);
                passthru("date ".date("Y/m/d", $timestamp));
                passthru("time ".date("H:i:s", $timestamp));
                echo "Connect to Time server Success.\n";
            } else {
                echo "Connect to Time server Failure.[Time Error]\n";
            }
        } else {
            echo "Connect to Time server Failure.[Data Error]\n";
        }
    } else {
        echo "Connect to Time server Failure.[{$status}]\n";
    }
?>