<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $name = $_POST["name"];
    $email = $_POST["email"];
    $message = $_POST["message"];

    // Process the form data, e.g. send an email, store it in a database, etc.
    // ...
	echo "Hello, world!";
	echo "My name is " . $name;
	echo "My email is " . $email;
	echo "My msg is " . $message;
    // Redirect the user to a "thank you" page
    header("Location: thankyou.html");
    exit();
}
?>