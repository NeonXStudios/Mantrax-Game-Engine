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

// Obtener el ID del documento
$id = $_GET['id'];

// Obtener el documento de la base de datos
$sql = "SELECT title, content, cpp_code, created_at FROM docs WHERE id = $id";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    // Mostrar el documento
    $row = $result->fetch_assoc();
    echo "<h1>" . $row['title'] . "</h1>";
    echo "<p><strong>Creado el: </strong>" . $row['created_at'] . "</p>";
    echo "<p>" . nl2br($row['content']) . "</p>";

    // Mostrar el código C++ con Highlight.js
    if (!empty($row['cpp_code'])) {
        echo "<h3>Código C++</h3>";
        echo "<pre><code class='language-cpp'>" . htmlspecialchars($row['cpp_code']) . "</code></pre>";
    }
} else {
    echo "Documento no encontrado.";
}

// Cerrar la conexión
$conn->close();
?>

<!-- Scripts -->
<script src="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.7.0/highlight.min.js"></script>
<script>hljs.highlightAll();</script>
