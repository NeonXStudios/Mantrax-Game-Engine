<?php
// Conexión a la base de datos
$servername = "localhost";
$username = "admin"; // Usa tu usuario de base de datos
$password = "admin"; // Usa tu contraseña de base de datos
$dbname = "documentation_db"; // Nombre de la base de datos

// Crear la conexión
$conn = new mysqli($servername, $username, $password, $dbname);

// Verificar la conexión
if ($conn->connect_error) {
    die("Conexión fallida: " . $conn->connect_error);
}

// Obtener los datos del formulario
$title = $_POST['title'];
$content = $_POST['content'];
$cpp_code = $_POST['cpp_code'];

// Insertar los datos en la base de datos
$sql = "INSERT INTO docs (title, content, cpp_code) VALUES ('$title', '$content', '$cpp_code')";

if ($conn->query($sql) === TRUE) {
    echo "Documentación guardada correctamente.";
} else {
    echo "Error al guardar la documentación: " . $conn->error;
}

// Cerrar la conexión
$conn->close();
?>
