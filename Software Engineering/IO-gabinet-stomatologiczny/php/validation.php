<?php
function sprawdzCiasteczko() {
    $ciasteczko = $_COOKIE['cred'];
    $cred_parts = explode('|', $ciasteczko);
    $login = $cred_parts[0];
    $password = $cred_parts[1];
    $dsn = 'mysql:host=localhost;dbname=IO';
    $username = 'root';
    $passwordroot = '';

    try {
        $pdo = new PDO($dsn, $username, $passwordroot);
        $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

        $query = "SELECT COUNT(*) FROM login WHERE login = :login AND password = :haslo";
        $stmt = $pdo->prepare($query);
        $stmt->bindParam(':login', $login);
        $stmt->bindParam(':haslo', $password);
        $stmt->execute();

        $result = $stmt->fetchColumn();

        if ($result > 0) {
            return true;
        } else {
            return false;
        }
    } catch (PDOException $e) {
        echo 'Błąd połączenia: ' . $e->getMessage();
        return false;
    }
}
?>