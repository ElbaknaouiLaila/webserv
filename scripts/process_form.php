<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $name = $_POST["name"];
    $email = $_POST["email"];
    $message = $_POST["message"];

    // Process the form data, e.g. send an email, store it in a database, etc.
    // ...
	echo "Hello, world! <br>";
	echo "My name is " . $name;
	echo "<br>My email is " . $email;
	echo "<br>My msg is " . $message;
    // Redirect the user to a "thank you" page

    exit();
}
?>