<?php
$conn = mysqli_connect('localhost', 'root', '', 'IO');
if (!$conn) {
  die("Connection failed: " . mysqli_connect_error());
}

$login = $_POST['login'];
$haslo = $_POST['haslo'];

$sql = "SELECT * FROM Loginy WHERE Login='$login' AND Haslo='$haslo'";
$result = mysqli_query($conn, $sql);

if (mysqli_num_rows($result) > 0) {
  echo "OK";
} else {
  echo "Error";
}

mysqli_close($conn);
?>
