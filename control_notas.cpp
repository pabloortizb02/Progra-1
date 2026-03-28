#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

// Estructura para manejar datos en archivo binario (tamaño fijo)
struct nodoEstudiante {
    int clave;
    char etiqueta[45];
    float promedio;
};

// ----------------------
// Guarda datos en archivo de texto
// ----------------------
void cargarTXT() {
    ofstream flujoSalida("alumnos.txt", ios::app); // modo append

    // Verificar si abrió correctamente
    if (!flujoSalida.is_open()) {
        cout << "No se pudo acceder al archivo\n";
        return;
    }

    int claveAux;
    string nombreAux;
    float notaAux;

    // Captura de datos
    cout << "\nClave: "; cin >> claveAux;
    cout << "Nombre: "; cin >> nombreAux;
    cout << "Nota: "; cin >> notaAux;

    // Escritura en archivo
    flujoSalida << claveAux << " " << nombreAux << " " << notaAux << endl;

    flujoSalida.close(); // cerrar archivo
}

// ----------------------
// Muestra todos los registros del TXT
// ----------------------
void desplegarTXT() {
    ifstream flujoEntrada("alumnos.txt");

    // Validar archivo
    if (!flujoEntrada.good()) {
        cout << "Archivo no disponible\n";
        return;
    }

    int claveAux;
    string nombreAux;
    float notaAux;

    cout << "\n--- REGISTROS ---\n";

    // Lectura secuencial
    while (flujoEntrada >> claveAux >> nombreAux >> notaAux) {
        cout << claveAux << " | " << nombreAux << " | " << notaAux << endl;
    }

    flujoEntrada.close();
}

// ----------------------
// Busca un alumno por clave
// ----------------------
void localizarRegistro() {
    ifstream flujoEntrada("alumnos.txt");

    if (!flujoEntrada.is_open()) {
        cout << "Error de lectura\n";
        return;
    }

    int ref;
    cout << "\nClave a buscar: ";
    cin >> ref;

    int claveAux;
    string nombreAux;
    float notaAux;
    bool existe = false;

    // Recorre todo el archivo
    while (flujoEntrada >> claveAux >> nombreAux >> notaAux) {
        if (claveAux == ref) {
            cout << "Coincidencia -> " << nombreAux << " con " << notaAux << endl;
            existe = true;
        }
    }

    if (!existe)
        cout << "No encontrado\n";

    flujoEntrada.close();
}

// ----------------------
// Actualiza datos usando archivo temporal
// ----------------------
void reescribirTXT() {
    ifstream original("alumnos.txt");
    ofstream respaldo("swap.txt");

    // Validación de archivos
    if (!original.is_open() || !respaldo.is_open()) {
        cout << "Error en archivos\n";
        return;
    }

    int objetivo;
    cout << "\nClave a modificar: ";
    cin >> objetivo;

    int claveAux;
    string nombreAux;
    float notaAux;

    // Copia todo al temporal (modificando si coincide)
    while (original >> claveAux >> nombreAux >> notaAux) {
        if (claveAux == objetivo) {
            cout << "Nuevo valor: ";
            cin >> notaAux;
        }
        respaldo << claveAux << " " << nombreAux << " " << notaAux << endl;
    }

    original.close();
    respaldo.close();

    // Reemplazo del archivo original
    remove("alumnos.txt");
    rename("swap.txt", "alumnos.txt");

    cout << "Actualizacion aplicada\n";
}

// ----------------------
// Guarda datos en archivo binario
// ----------------------
void persistirBinario() {
    ofstream archivo("alumnos.dat", ios::binary | ios::app);

    if (!archivo.good()) {
        cout << "Error creando binario\n";
        return;
    }

    nodoEstudiante bloque;

    // Captura de datos
    cout << "\nClave: ";
    cin >> bloque.clave;

    cout << "Nombre: ";
    cin >> bloque.etiqueta;

    cout << "Nota: ";
    cin >> bloque.promedio;

    // Escritura binaria
    archivo.write((char*)&bloque, sizeof(bloque));

    archivo.close();
}

// ----------------------
// Acceso aleatorio (leer posición específica)
// ----------------------
void lecturaDirecta() {
    ifstream archivo("alumnos.dat", ios::binary);

    if (!archivo.is_open()) {
        cout << "Archivo no encontrado\n";
        return;
    }

    int index;
    cout << "\nIndice (0,1,2...): ";
    cin >> index;

    nodoEstudiante bloque;

    // Mover puntero a la posición deseada
    archivo.seekg(index * sizeof(bloque), ios::beg);

    // Mostrar posición actual (detalle importante)
    cout << "Posicion actual (bytes): " << archivo.tellg() << endl;

    // Leer registro
    archivo.read((char*)&bloque, sizeof(bloque));

    if (!archivo.fail()) {
        cout << bloque.clave << " | "
             << bloque.etiqueta << " | "
             << bloque.promedio << endl;
    } else {
        cout << "Indice fuera de rango\n";
    }

    archivo.close();
}

// ----------------------
// Recorre todo el archivo binario
// ----------------------
void barridoBinario() {
    ifstream archivo("alumnos.dat", ios::binary);

    if (!archivo.is_open()) {
        cout << "No hay datos binarios\n";
        return;
    }

    nodoEstudiante bloque;

    cout << "\n--- BINARIO ---\n";

    // Lectura secuencial del binario
    while (archivo.read((char*)&bloque, sizeof(bloque))) {
        cout << bloque.clave << " - "
             << bloque.etiqueta << " - "
             << bloque.promedio << endl;
    }

    archivo.close();
}

// ----------------------
// MENÚ PRINCIPAL
// ----------------------
int main() {
    int eleccion;

    do {
        cout << "\n===== CONTROL DE DATOS =====\n";
        cout << "1. Insertar TXT\n";
        cout << "2. Ver TXT\n";
        cout << "3. Buscar\n";
        cout << "4. Modificar\n";
        cout << "5. Guardar binario\n";
        cout << "6. Leer posicion\n";
        cout << "7. Ver binario\n";
        cout << "0. Salir\n>> ";

        cin >> eleccion;

        switch (eleccion) {
            case 1: cargarTXT(); break;
            case 2: desplegarTXT(); break;
            case 3: localizarRegistro(); break;
            case 4: reescribirTXT(); break;
            case 5: persistirBinario(); break;
            case 6: lecturaDirecta(); break;
            case 7: barridoBinario(); break;
        }

    } while (eleccion != 0);

    return 0;
}
