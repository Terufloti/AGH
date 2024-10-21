<?php
    $login = "";
    if(isset($_GET['login'])) {
        $login = $_GET['login'];
    }
    if(isset($_GET['action'])) {
        $action = $_GET['action'];
    } elseif(isset($_POST['action'])) {
        $action = $_POST['action'];
    }

    switch($action) {
        case 'login': login(); break;
        case 'register': register(); break;
        case 'proposeAppointment': proposeAppointments(); break;
        case 'getFunctionCode': getFunctionCode($login); break;
        case 'getName': getName($login); break;
        case 'getSurname': getSurname($login); break;
        case 'getAppointments': getAppointments($login); break;
        case 'countProposalAppointments': countProposalAppointments(); break;
        case 'showProposalAppointments': showProposalAppointments(); break;
        case 'secretaryAcceptProposal': secretaryAcceptProposal(); break;
        case 'secretaryDeclineProposal': secretaryDeclineProposal(); break;
        case 'secretaryProposeAppointment': secretaryProposeAppointment(); break;

    }

    function login() {
        if ($_SERVER["REQUEST_METHOD"] === "POST") {
            $login = $_POST['login-login'];
            $password = $_POST['haslo-login'];
            
            $servername = "localhost";
            $username = "root";
            $db_password = "";
            $dbname = "IO";
        
            $conn = new mysqli($servername, $username, $db_password, $dbname);
        
            if ($conn->connect_error) {
                die("Nie można połączyć z bazą danych: " . $conn->connect_error);
            }
        
            $sql = "SELECT * FROM login WHERE login = '$login'";
            $result = $conn->query($sql);
        
            if ($result->num_rows > 0) {
                $row = $result->fetch_assoc();
                if ($row['password'] === $password) {
                    $response = array(
                        "status" => "success",
                        "message" => "Logowanie zakończone sukcesem."
                    );
                    echo json_encode($response);
                } else {
                    $response = array(
                        "status" => "invalid_password",
                        "message" => "Niepoprawne hasło dla tego loginu!"
                    );
                    echo json_encode($response);
                }
            } else {
                $response = array(
                    "status" => "invalid_login",
                    "message" => "Niepoprawny login!"
                );
                echo json_encode($response);
            }
        
            $conn->close();
        }
    }
    function register() {
        ini_set('display_errors', 1);
        ini_set('display_startup_errors', 1);
        error_reporting(E_ALL);

        if ($_SERVER["REQUEST_METHOD"] === "POST") {
            $login = $_POST['login'];
            $password = $_POST['haslo-register'];
            $email = $_POST['email'];
            $phone = $_POST['phone-number'];
            //$past = $_POST['past-treatment'];
            $name = $_POST['nameRegister'];
            $surname = $_POST['surnameRegister'];

            $servername = "localhost";
            $username = "root";
            $db_password = "";
            $dbname = "IO";

            $conn = new mysqli($servername, $username, $db_password, $dbname);

            if ($conn->connect_error) {
                die("Nie można połączyć z bazą danych: " . $conn->connect_error);
            }

            $checkQuery = "SELECT * FROM `login` WHERE `login`='$login' OR `email`='$email'";
            $checkResult = $conn->query($checkQuery);

            if ($checkResult->num_rows > 0) {
                $response = array('status' => 'error', 'message' => 'Użytkownik o podanym loginie lub adresie e-mail już istnieje.');
                echo json_encode($response);
            } else {
                $sql = "INSERT INTO `login` (`login`, `password`, `function`, `email`, `phone`, `past`, `name`, `surname`) VALUES ('$login', '$password', '10', '$email', '$phone', '0', '$name', '$surname');";

                if ($conn->query($sql) === TRUE) {
                    $response = array('status' => 'success', 'message' => 'Rejestracja zakończona sukcesem.');
                    echo json_encode($response);
                    session_start();
                    $_SESSION['registration_success'] = true;
                    $_SESSION['registered_email'] = $email;
                } else {
                    $response = array('status' => 'error', 'message' => 'Błąd podczas rejestracji: ' . $conn->error);
                    echo json_encode($response);
                }
            }

            $conn->close();
        }
    }

    function proposeAppointments() {
        if ($_SERVER["REQUEST_METHOD"] === "POST") {
            $login = $_POST['login'];
            $date = $_POST['date'];
            $time = $_POST['time'];
            $name = $_POST['name'];
            $surname = $_POST['surname'];
            $doctor = $_POST['doctor'];

            $servername = "localhost";
            $username = "root";
            $db_password = "";
            $dbname = "spotkania";

            $conn = new mysqli($servername, $username, $db_password, $dbname);

            if ($conn->connect_error) {
                die("Nie można połączyć z bazą danych: " . $conn->connect_error);
            }

            $sqlCheck = "SELECT * FROM `zaplanowane` WHERE `dentysta` = '$doctor' AND `data` = '$date' AND (`godzina` = ADDTIME('$time', '00:30:00') OR `godzina` = SUBTIME('$time', '00:30:00'));";
            $resultCheck = $conn->query($sqlCheck);
            if($resultCheck->num_rows > 0) {
                $response = array('status' => 'error', 'message' => 'W tym samym czasie zaplanowane jest już inne spotkanie u tego dentysty.');
                header('Content-Type: application/json');
                echo json_encode($response);
            } else {
                $sql = "INSERT INTO `propozycje` (`login`, `name`, `surname`, `date`, `time`, `doctor`) VALUES ('$login', '$name', '$surname', '$date', '$time', '$doctor');";
                if($conn->query($sql) === TRUE) {
                    $response = array('status' => 'success', 'message' => 'Spotkanie wysłane do sprawdzenia.');
                    echo json_encode($response);
                } else {
                    $response = array('status' => 'error', 'message' => 'Błąd podczas tworzenia propozycji: ' . $conn->error);
                    echo json_encode($response);
                }
            }
            $conn->close();
        }
    }
    function getFunctionCode($login) {
        function checkLogin($login) {
            $dsn = 'mysql:host=localhost;dbname=IO';
            $username = 'root';
            $password = '';
        
            try {
                $pdo = new PDO($dsn, $username, $password);
                $query = 'SELECT function FROM login WHERE login = ?';
                $stmt = $pdo->prepare($query);
                $stmt->execute([$login]);
        
                $result = $stmt->fetch(PDO::FETCH_ASSOC);
        
                if ($result !== false) {
                    return $result['function'];
                } else {
                    throw new Exception('Błąd: Użytkownik o podanym loginie nie istnieje.');
                }
            } catch (PDOException $e) {
                echo 'Błąd podczas zapytania do bazy danych: ' . $e->getMessage();
                return false;
            }
        }
        
        if (isset($_GET['login'])) {
            $login = $_GET['login'];
            try {
                $functionCode = checkLogin($login);
                echo $functionCode;
            } catch (Exception $e) {
                echo $e->getMessage();
            }
        } else {
            echo 'Błąd: Brak przekazanego loginu.';
        }
    }

    function getName($login) {
        function checkLogin($login) {
            $dsn = 'mysql:host=localhost;dbname=IO';
            $username = 'root';
            $password = '';
        
            try {
                $pdo = new PDO($dsn, $username, $password);
                $query = 'SELECT name FROM login WHERE login = ?';
                $stmt = $pdo->prepare($query);
                $stmt->execute([$login]);
        
                $result = $stmt->fetch(PDO::FETCH_ASSOC);
        
                if ($result !== false) {
                    return $result['name'];
                } else {
                    throw new Exception('Błąd: Użytkownik o podanym loginie nie istnieje.');
                }
            } catch (PDOException $e) {
                echo 'Błąd podczas zapytania do bazy danych: ' . $e->getMessage();
                return false;
            }
        }
        
        if (isset($_GET['login'])) {
            $login = $_GET['login'];
            try {
                $imie = checkLogin($login);
                echo $imie;
            } catch (Exception $e) {
                echo $e->getMessage();
            }
        } else {
            echo 'Błąd: Brak przekazanego loginu.';
        }
    }

    function getSurname($login) {
        function checkLogin($login) {
            $dsn = 'mysql:host=localhost;dbname=IO';
            $username = 'root';
            $password = '';
        
            try {
                $pdo = new PDO($dsn, $username, $password);
                $query = 'SELECT surname FROM login WHERE login = ?';
                $stmt = $pdo->prepare($query);
                $stmt->execute([$login]);
        
                $result = $stmt->fetch(PDO::FETCH_ASSOC);
        
                if ($result !== false) {
                    return $result['surname'];
                } else {
                    throw new Exception('Błąd: Użytkownik o podanym loginie nie istnieje.');
                }
            } catch (PDOException $e) {
                echo 'Błąd podczas zapytania do bazy danych: ' . $e->getMessage();
                return false;
            }
        }
        
        if (isset($_GET['login'])) {
            $login = $_GET['login'];
            try {
                $nazwisko = checkLogin($login);
                echo $nazwisko;
            } catch (Exception $e) {
                echo $e->getMessage();
            }
        } else {
            echo 'Błąd: Brak przekazanego loginu.';
        }
    }

    function getAppointments($login) {
        $dbHost = 'localhost';
        $dbUser = 'root';
        $dbPassword = '';
        $dbName = 'spotkania';

        $conn = new mysqli($dbHost, $dbUser, $dbPassword, $dbName);

        if($conn->connect_error) {
            die("Błąd połączenia z bazą danych: ".$conn->connect_error);
        }

        $sql = "SELECT * FROM `zaplanowane` WHERE login = '$login'";
        $result = $conn->query($sql);
        if($result->num_rows > 0) {
            $appointments = array();
            while($row = $result->fetch_assoc()) {
                $appointment = array(
                    'lekarz' => $row['dentysta'],
                    'data' => $row['data'],
                    'godzina' => $row['godzina']
                );
                $appointments[] = $appointment;
            }
            header('Content-Type: application/json');
            echo json_encode($appointments);
        }
        else {
            header('Content-Type: application/json');
            echo json_encode([]);
        }
        $conn->close();
    }

    function countProposalAppointments() {
        $dbname = $_POST['db'];
        $table = $_POST['table'];

        $servername = "localhost";
        $username = "root";
        $password = "";

        $conn = new mysqli($servername, $username, $password, $dbname);

        if ($conn->connect_error) {
            die("Błąd połączenia z bazą danych: " . $conn->connect_error);
        }
        $sql = "SELECT COUNT(*) AS count FROM $table";
        $result = $conn->query($sql);
        if ($result === false) {
            die("Błąd zapytania: " . $conn->error);
        }
        $row = $result->fetch_assoc();
        $count = $row['count'];
        $conn->close();
        $response = array('count' => $count);
        header('Content-Type: application/json');
        echo json_encode($response);

    }
    function showProposalAppointments() {
        $dbname = $_POST['db'];
        $table = $_POST['table'];
    
        $servername = "localhost";
        $username = "root";
        $password = "";
        $conn = new mysqli($servername, $username, $password, $dbname);
    
        if ($conn->connect_error) {
            die("Błąd połączenia z bazą danych: " . $conn->connect_error);
        }
        $sql = "SELECT name, surname, date, time, doctor FROM $table";
        $result = $conn->query($sql);
        if ($result === false) {
            die("Błąd zapytania: " . $conn->error);
        }
        $rows = array();
        $rowNumber = 1;
        while ($row = $result->fetch_assoc()) {
            $row['rowNumber'] = $rowNumber;
            $rows[] = $row;
            $rowNumber++;
        }
    
        $conn->close();
    
        header('Content-Type: application/json');
        echo json_encode($rows);
    }

    function secretaryAcceptProposal() {
        $rowNumber = $_POST['rowNumber'];
        $tableSource = 'propozycje';
        $tableDestination = 'zaplanowane';

        $servername = 'localhost';
        $username = 'root';
        $password = '';
        $dbname = 'spotkania';

        $conn = new mysqli($servername, $username, $password, $dbname);
        if ($conn->connect_error) {
            die("Błąd połączenia z bazą danych: " . $conn->connect_error);
        }
        
        $selectSql = "SELECT * FROM $tableSource WHERE Lp = $rowNumber";
        $result = $conn->query($selectSql);
        if ($result === false) {
            die("Błąd zapytania: " . $conn->error);
        }
        
        if ($result->num_rows > 0) {
            $row = $result->fetch_assoc();

            $maxValueSql = "SELECT MAX(Lp) FROM $tableDestination";
            $result = $conn->query($maxValueSql);
            if ($result === false) {
                die("Błąd zapytania: " . $conn->error);
            }
            $maxValue = $result->fetch_row()[0];

            $newAutoIncrement = $maxValue + 1;
            $alterSql = "ALTER TABLE $tableDestination AUTO_INCREMENT = $newAutoIncrement";
            $result = $conn->query($alterSql);
            if ($result === false) {
                die("Błąd zapytania: " . $conn->error);
            }
            
            $insertSql = "INSERT INTO $tableDestination (login, imie, nazwisko, data, godzina, dentysta) VALUES ('{$row['login']}', '{$row['name']}', '{$row['surname']}', '{$row['date']}', '{$row['time']}', '{$row['doctor']}')";
            $result = $conn->query($insertSql);
            if ($result === false) {
                die("Błąd zapytania: " . $conn->error);
            }
            
            $deleteSql = "DELETE FROM $tableSource WHERE Lp = $rowNumber";
            $result = $conn->query($deleteSql);
            if ($result === false) {
                die("Błąd zapytania: " . $conn->error);
            }

            $alterSql = "ALTER TABLE $tableSource AUTO_INCREMENT = 1";
            $result = $conn->query($alterSql);
            if ($result === false) {
                die("Błąd zapytania: " . $conn->error);
            }
            
            $response = array('success' => true);
            header('Content-Type: application/json');
            echo json_encode($response);
        } else {
            die("Błąd: Nieprawidłowy numer wiersza");
        }

    $conn->close();
    }

    function secretaryDeclineProposal() {
        $rowNumber = $_POST['rowNumber'];
        $servername = 'localhost';
        $username = 'root';
        $password = '';
        $dbname = 'spotkania';
        $table = 'propozycje';

        $conn = new mysqli($servername, $username, $password, $dbname);
        if ($conn->connect_error) {
            die("Błąd połączenia z bazą danych: " . $conn->connect_error);
        }
        if ($rowNumber) {
            $deleteSql = "DELETE FROM $table WHERE Lp = $rowNumber";
            $updateSql = "UPDATE $table SET Lp = Lp - 1 WHERE Lp > $rowNumber";
            $alterSql = "ALTER TABLE $table AUTO_INCREMENT = $rowNumber";

            $result = $conn->query($deleteSql);
            if ($result === false) {
                die("Błąd zapytania: " . $conn->error);
            }

            $result = $conn->query($updateSql);
            if($result === false) {
                die("Błąd zapytania: " . $conn->error);
            }

            $result = $conn->query($alterSql);
            if ($result === false) {
                die("Błąd zapytania: " . $conn->error);
            }
            $response = array('success' => true);
            header('Content-Type: application/json');
            echo json_encode($response);
        } else {
            die("Błąd: Nieprawidłowy numer wiersza");
        }

        $conn->close();
    }

    function secretaryProposeAppointment() {

    }
    
?>