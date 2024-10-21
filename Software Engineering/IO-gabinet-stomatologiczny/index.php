<?php
    require_once 'php/validation.php';
    include 'php/utils.php';
    if(isset($_COOKIE['cred']) && !empty($_COOKIE['cred'])) {
        $result = sprawdzCiasteczko();
        if(!$result) {
            if (isset($_COOKIE)) {
                foreach ($_COOKIE as $cookieName => $cookieValue) {
                    setcookie($cookieName, '', time() - 3600, '/');
                }
            }
        }
    }
    ?>

<!doctype html>
<html lang="en" data-bs-theme="dark">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Gabinet stomatologiczny</title>
    <script src='https://code.jquery.com/jquery-3.7.0.min.js'></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.6.0/jquery.min.js"></script>

    <link rel="stylesheet" href="//code.jquery.com/ui/1.13.2/themes/base/jquery-ui.css">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-KK94CHFLLe+nY2dmCWGMq91rCGa5gtU4mk92HdvYe+M/SXH301p5ILy+dN9+nJOZ" crossorigin="anonymous">
    <link href="https://cdn.jsdelivr.net/npm/timepicker@1.14.0/jquery.timepicker.css" rel="stylesheet">

    <script src='https://cdnjs.cloudflare.com/ajax/libs/moment.js/2.29.1/moment.min.js'></script>
    <script src='https://cdn.jsdelivr.net/npm/fullcalendar@6.1.8/index.global.min.js'></script>
    <script src="https://cdn.jsdelivr.net/npm/dayjs@1.11.7/dayjs.min.js"></script>
    <script src="https://kit.fontawesome.com/5169f4649c.js" crossorigin="anonymous"></script>
    <script src="https://code.jquery.com/ui/1.13.2/jquery-ui.js"></script>

    <script src="https://cdn.jsdelivr.net/npm/timepicker@1.14.0/jquery.timepicker.min.js"></script>

    <script src="js/utils.js"></script>
    <link rel="stylesheet" href="css/style.css">

    <script>
        setInterval(function () {
            if (document.cookie.indexOf("cred") !== -1) {
                console.log("Ciasteczko 'cred' istnieje.");
            }
            else {
                console.log("Ciasteczko 'cred' nie istnieje.");
                var logoutButton = document.getElementById("logout-button");
                if (logoutButton) {
                    window.location.href = "/IO-gabinet-stomatologiczny/index.php";
                }
            }
        }, 5000);
    </script>
  </head>
  <body>
    <nav class="navbar navbar-expand-lg bg-dark navbar-dark">
        <div class="container-fluid"> 
            <a class="navbar-brand">Farion & Kozieł Corp.</a>
            <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarSupportedContent" aria-controls="navbarSupportedContent" aria-expanded="false" aria-label="Toggle navigation">
                <span class="navbar-toggler-icon"></span>
            </button>
            <div class="collapse navbar-collapse" id="navbarSupportedContent">
                <ul class="navbar-nav me-auto mb-2 mb-lg-0">
                    <li class="nav-item  to-change">
                        <a class="nav-link active" id="nav-link-change" aria-current="page" href="#">O nas</a>
                    </li>
                    <li class="nav-item to-change">
                        <a class="nav-link" aria-current="page" href="#">Nasi lekarze</a>
                    </li>
                    <li class="nav-item to-change">
                        <a class="nav-link" aria-current="page" href="#">Cennik</a>
                    </li>
                </ul>
            </div>

            
            <div class="d-flex">
                <div class="btn-group" role="group" aria-label="Login/register/logout outlined group">
                    <button type="button" class="btn btn-outline-info" data-bs-toggle="modal" data-bs-target="#registerModal">Zarejestruj się</button>
                    <button type="button" class="btn btn-outline-success" data-bs-toggle="modal" data-bs-target="#loginModal">Logowanie</button>
                </div>
            </div>
        </div>
    </nav>
    
    <div class="container">
        <section class="main">
            <div class="row" id="welcomeMessage"></div>
            <div class="row">
                <div id="mycalendar"></div>
            </div>
        </section>



        <section class="modals">
            <div class="modal fade text-black" data-bs-theme="light" id="loginModal" data-bs-backdrop="static" data-bs-keyboard="false" tabindex="-1" aria-labelledby="loginModalLabel" aria-hidden="true">
                <div class="modal-dialog modal-dialog-centered">
                    <div class="modal-content" data-bs-theme="light">
                        <div class="modal-header">
                            <h1 class="modal-title fs-5" id="staticBackdropLabel">Zaloguj</h1>
                            <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close" style="background-color: black;"></button>
                        </div>
                        <form id="login-form" method="POST" action="php/utils.php?action=login">
                            <div class="modal-body">
                                <input type="login" class="form-control" id="inputLogin-login" name="login-login" placeholder="Login">
                                <div id="validationServerLogin-LoginFeedback-fail" class="invalid-feedback"></div>
                                <div id="validationServerLogin-LoginFeedback-success"class="valid-feedback"></div>
                                <br />
                                <input type="password" id="inputPassword-login" name="haslo-login" class="form-control" aria-labelledby="passwordHelpBlock" placeholder="Hasło">
                                <div id="validationServerPassword-LoginFeedback-fail" class="invalid-feedback"></div>
                                <div id="validationServerPassword-LoginFeedback-success"class="valid-feedback"></div>
                                <br />
                                <div id="login-alert-placeholder"></div>
                            </div>
                            <div class="modal-footer">
                                <button type="button" class="btn btn-outline-secondary" data-bs-dismiss="modal">Close</button>
                                <button type="submit" id="login-button" class="btn btn-outline-success">Loguj!</button>
                            </div>
                        </form>
                    </div>
                </div>
            </div>
            <div class="modal fade text-black" id="registerModal" data-bs-theme="light" data-bs-backdrop="static" data-bs-keyboard="false" tabindex="-1" aria-labelledby="registerModalLabel" aria-hidden="true">
                <div class="modal-dialog modal-dialog-centered">
                    <div class="modal-content">
                        <div class="modal-header">
                            <h1 class="modal-title fs-5" id="staticBackdropLabel">Zarejestruj się u nas!</h1>
                            <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
                        </div>
                        <form id="register-form" method="POST" action="php/utils.php?action=register">
                            <div class="modal-body">
                                <input type="login" class="form-control" id="login-register" name="login" placeholder="Login*" aria-describedby="loginHelp">
                                <div id="validationServerUsernameFeedback-fail" class="invalid-feedback"></div>
                                <div id="validationServerUsernameFeedback-success" class="valid-feedback"></div>
                                <div id="loginHelp" class="form-text">Login musi zaczynać się wielką literą.</div>
                                <br />
                                <input type="password" id="inputPassword-register" name="haslo-register" class="form-control" aria-labelledby="passwordHelpBlock" placeholder="Hasło*" aria-describedby="passwordHelp">
                                <div id="validationServerPassword01Feedback-fail" class="invalid-feedback"></div>
                                <div id="validationServerPassword01Feedback-success" class="valid-feedback"></div>
                                <div id="passwordHelp" class="form-text">Hasło musi zawierać wielką literę, cyfrę, znak specjalny oraz być długie na conajmniej 10 znaków.</div>
                                <input type="password" id="inputPassword-register2" name="haslo-register" class="form-control" aria-labelledby="passwordHelpBlock" placeholder="Powtórz hasło*">
                                <div id="validationServerPasswordFeedback-fail" class="invalid-feedback"></div>
                                <div id="validationServerPasswordFeedback-success" class="valid-feedback"></div>
                                <br />
                                <input type="email" class="form-control" id="email-register" name="email" placeholder="Email*">
                                <div id="validationServerEmailFeedback-fail" class="invalid-feedback"></div>
                                <div id="validationServerEmailFeedback-success" class="valid-feedback"></div>
                                <br />
                                <input type="text" class="form-control" id="phone-register" name="phone-number" placeholder="Numer telefonu* np. (+48999999999)">
                                <div id="validationServerPhoneFeedback-fail" class="invalid-feedback"></div>
                                <div id="validationServerPhoneFeedback-success" class="valid-feedback"></div>
                                <br />
                                <input type="login" class="form-control" id="name-register" name="nameRegister" placeholder="Imie*" aria-describedby="nameHelp">
                                <div id="validationServerNameFeedback-fail" class="invalid-feedback"></div>
                                <div id="validationServerNameFeedback-success" class="valid-feedback"></div>
                                <div id="nameHelp" class="form-text">Imie musi zaczynać się wielką literą.</div>
                                <input type="login" class="form-control" id="surname-register" name="surnameRegister" placeholder="Nazwisko*" aria-describedby="surnameHelp">
                                <div id="validationServerSurnameFeedback-fail" class="invalid-feedback"></div>
                                <div id="validationServerSurnameFeedback-success" class="valid-feedback"></div>
                                <div id="surnameHelp" class="form-text">Nazwisko musi zaczynać się wielką literą.</div>
                                <br />
                                <div class="form-check form-switch">
                                    <input class="form-check-input" type="checkbox" role="switch" id="past-treatment-registration" name="past-treatment" checked>
                                    <label class="form-check-label" for="past-treatment-registration">Czy leczyłeś/aś się już u nas?</label>
                                </div>
                                <div id="alert-placeholder"></div>
                            </div>
                            <div class="modal-footer">
                                <button type="button" class="btn btn-outline-secondary" data-bs-dismiss="modal">Close</button>
                                <button type="submit" id="register-button" class="btn btn-outline-success">Zarejestruj!</button>
                            </div>
                        </form>
                    </div>
                </div>
            </div>
        </section>
        <section class="Offcanvas">
    
        </section>
    </div>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha3/dist/js/bootstrap.bundle.min.js" integrity="sha384-ENjdO4Dr2bkBIFxQpeoTz1HIcje39Wm4jDKdf19U8gI4ddQ3GYNS7NTKfAdVQSZe" crossorigin="anonymous"></script>
    <script>
        var login = getLoginFromCookie();
        var functionalCode = getCookieValue("function");
            if(functionalCode) {
                switch(functionalCode) {
                    case '10':
                        loggedCustomer();
                        var promiseName = getNameFromDB(login);
                        var name = "";
                        var promiseSurname = getSurnameFromDB(login);
                        var surname = "";
                        promiseName.then(nameAfter => {
                            name = nameAfter;
                        }).catch(error => {
                            console.error('Error: ',error);
                        });
                        setTimeout(function() {
                            addHeaderWithName(name);
                        },200);
                        promiseSurname.then(surnameAfter => {
                            surname = surnameAfter;
                        }).catch(error => {
                            console.error('Error: ',error);
                        });
                        function getNameFromDB(login) {
                            return fetch('php/utils.php?login=' + login + '&action=getName')
                            .then(response => response.text())
                            .then(data => {
                                return data;
                            })
                            .catch(error => {
                                console.error('Błąd: ', error);
                                throw error;
                            });
                        }
                        function getSurnameFromDB(login) {
                            return fetch('php/utils.php?login=' + login + '&action=getSurname')
                            .then(response => response.text())
                            .then(data => {
                                return data;
                            })
                            .catch(error => {
                                console.error('Błąd: ', error);
                                throw error;
                            });
                        }
                        properCreateCalendar();
                        $('#datetimepicker1').timepicker({
                            timeFormat: 'H:i',
                            minTime: '8:00',
                            maxTime: '20:00'
                        });
                        $(function() {
                            $("#datetimepicker2").datepicker({
                                dateFormat: "yy-mm-dd",
                                beforeShow: function(input, inst) {
                                setTimeout(function() {
                                    inst.dpDiv.css({
                                    zIndex: 9999
                                    });
                                }, 0);
                                }
                            });
                            $("#anim").on("change", function() {
                                $("#datetimepicker2").datepicker("option", "showAnim", $(this).val());
                            });
                        });
                        document.getElementById('proposal-form').addEventListener('submit', function(event) {
                            var dentysta = checkRadioButton('flexRadioDoctor');
                            function checkRadioButton(name) {
                                var response = {
                                    status: 'success'
                                }
                                var formData = new FormData();
                                var radioButtons = document.querySelectorAll('input[type="radio"][name="' + name + '"]');
                                for(var i=0; i < radioButtons.length; i++) {
                                    if(radioButtons[i].classList.contains("check")) {
                                        var dentysta = radioButtons[i].value;
                                        return dentysta;
                                    }
                                }
                                return 'Błąd wybranego lekarza';
                            }
                            event.preventDefault();
                            var isValid = validateProposalForm();
                            if(document.getElementById('datetimepicker1').value) {
                                if(isValid) {
                                    setTimeout(function() {
                                        var response = {
                                            status: 'success'
                                        };

                                        if(response.status === 'success') {
                                            var message = 'Wszystko się zgadza. Nastąpi przekierowanie!';
                                            var formData = new FormData();
                                            formData.append('login', login);
                                            formData.append('name', name);
                                            formData.append('surname', surname);
                                            formData.append('date', document.getElementById('datetimepicker2').value);
                                            formData.append('time', document.getElementById('datetimepicker1').value);
                                            formData.append('doctor', dentysta);
                                            fetch('php/utils.php?login=' + login + '&action=proposeAppointment', {
                                                method: 'POST',
                                                body: formData
                                            })
                                            .then(function(response) {
                                                return response.json();
                                            }) .then(function (data) {
                                                if(data.status === 'success') {
                                                    proposalAlertTriggering('Popozycja zakończona sukcesem. '+message, true);
                                                    setTimeout(function() {
                                                        window.location.href = 'php/propose-success.php';
                                                    },2000)
                                                } else {
                                                    proposalAlertTriggering('Błąd podczas składania propozycji: '+data.message, false);
                                                }
                                            })
                                        } else {
                                            proposalAlertTriggering('Wystąpił błąd podczas składania propozycji!', false);
                                        }
                                    },2000);
                                } else {
                                proposalAlertTriggering('Dane niepoprawne! Popraw dane.', false);
                                }
                            }
                        });
                    break;
                    case '3':
                        loggedSecretary();
                    break;
                }
            }
    </script>
    
    <script>
        document.getElementById('register-form').addEventListener('submit', function(event) {
            event.preventDefault();
            var isValid = validateRegistrationForm();
            if (isValid) {
                setTimeout(function() {
                    var response = {
                        status: 'success'
                    };

                    if (response.status === 'success') {
                        var message = 'Wszystkie dane są poprawne. Nastąpi przekierowanie!';
                        var formData = new FormData();
                        formData.append('login', document.getElementById('login-register').value);
                        formData.append('haslo-register', document.getElementById('inputPassword-register').value);
                        formData.append('email', document.getElementById('email-register').value);
                        formData.append('phone-number', document.getElementById('phone-register').value);
                        //formData.append('past-treatment', document.getElementById('past-treatment-registration').value);
                        formData.append('nameRegister', document.getElementById('name-register').value);
                        formData.append('surnameRegister', document.getElementById('surname-register').value);
                        fetch('php/utils.php?action=register', {
                            method: 'POST',
                            body: formData
                        }).then(function(response) {
                            return response.json();
                        }).then(function(data) {
                            if (data.status === 'success') {
                                alertTriggering('Rejestracja zakończona sukcesem.'+message, true);
                                setTimeout(function() {
                                    window.location.href = 'php/register-success.php';
                                },3000)
                            } else {
                                alertTriggering('Błąd podczas rejestracji: ' + data.message, false);
                            }
                        })
                    } else {
                        alertTriggering('Wystąpił błąd podczas rejestracji!', false);
                    }
                }, 2000);
            } else {
                alertTriggering('Dane niepoprawne! Popraw dane.', false);
            }
        });
    </script>
    <script>
        document.getElementById('login-form').addEventListener('submit', function(event) {
            event.preventDefault();
            validateLoginForm();
        });
    </script>
    <?php
        $test = sprawdzCiasteczko();
        json_encode($test);
    ?>
  </body>
</html>