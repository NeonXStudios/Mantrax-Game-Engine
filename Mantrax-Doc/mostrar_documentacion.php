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

// Obtener los documentos de la base de datos
$sql = "SELECT id, title, created_at FROM docs ORDER BY created_at DESC";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    // Mostrar los documentos
    while($row = $result->fetch_assoc()) {
        echo "<div class='document'>";
        echo "<h3><a href='ver_documento.php?id=" . $row['id'] . "'>" . $row['title'] . "</a></h3>";
        echo "<p>Creado el: " . $row['created_at'] . "</p>";
        echo "</div>";
    }
} else {
    echo "No se encontraron documentos.";
}

// Cerrar la conexión
$conn->close();
?>
