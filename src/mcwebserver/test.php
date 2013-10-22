<?php

header("Content-Type: text/html");

echo "<h1>Hello</h1>";
echo "hello, php!\n";
echo "QUERY_STRING: " . $_SERVER['QUERY_STRING'] . "\n";
var_dump($_POST);

// var_dump($_SERVER);
// echo "POST name: " . $_POST['name'] . "\n";
// echo "stdin: " . fgets(STDIN) . "\n";
// echo file_get_contents('http://www.258i.com');
// echo file_get_contents('http://news.baidu.com');
// echo fread('php://stdin', 21) . "\n";

