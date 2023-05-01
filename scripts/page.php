<!DOCTYPE html>
<html>
<head>
	<title>PHP Form Example</title>
</head>
<body>
	<h2>Fill out the form below:</h2>
	<form action="process_form.php" method="POST">
		<label for="name">Name:</label>
		<input type="text" id="name" name="name"><br><br>
		<label for="email">Email:</label>
		<input type="email" id="email" name="email"><br><br>
		<label for="message">Message:</label><br>
		<textarea id="message" name="message" rows="5" cols="40"></textarea><br><br>
		<input type="submit" value="Submit">
	</form>
</body>
</html>