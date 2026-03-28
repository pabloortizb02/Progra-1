#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;


struct nodoEstudiante {
    int clave;
    char etiqueta[45];
    float promedio;
};

void cargarTXT() {
    ofstream flujoSalida("alumnos.txt", ios::app);
    if (!flujoSalida.is_open()) {
        cout << "No se pudo acceder al archivo\n";
        return;
    }

    int claveAux;
    string nombreAux;
    float notaAux;

    cout << "\nClave: "; cin >> claveAux;
    cout << "Nombre: "; cin >> nombreAux;
    cout << "Nota: "; cin >> notaAux;

    flujoSalida << claveAux << " " << nombreAux << " " << notaAux << endl;
    flujoSalida.close();
}

void desplegarTXT() {
    ifstream flujoEntrada("alumnos.txt");
    if (!flujoEntrada.good()) {
        cout << "Archivo no disponible\n";
        return;
    }

    int claveAux;
    string nombreAux;
    float notaAux;

    cout << "\n--- REGISTROS ---\n";

    while (flujoEntrada >> claveAux >> nombreAux >> notaAux) {
        cout << claveAux << " | " << nombreAux << " | " << notaAux << endl;
    }

    flujoEntrada.close();
}

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

void reescribirTXT() {
    ifstream original("alumnos.txt");
    ofstream respaldo("swap.txt");

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

    while (original >> claveAux >> nombreAux >> notaAux) {
        if (claveAux == objetivo) {
            cout << "Nuevo valor: ";
            cin >> notaAux;
        }
        respaldo << claveAux << " " << nombreAux << " " << notaAux << endl;
    }

    original.close();
    respaldo.close();

    remove("alumnos.txt");
    rename("swap.txt", "alumnos.txt");

    cout << "Actualizacion aplicada\n";
}

void persistirBinario() {
    ofstream archivo("alumnos.dat", ios::binary | ios::app);
    if (!archivo.good()) {
        cout << "Error creando binario\n";
        return;
    }

    nodoEstudiante bloque;

    cout << "\nClave: ";
    cin >> bloque.clave;

    cout << "Nombre: ";
    cin >> bloque.etiqueta;

    cout << "Nota: ";
    cin >> bloque.promedio;

    archivo.write((char*)&bloque, sizeof(bloque));
    archivo.close();
}

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

    archivo.seekg(index * sizeof(bloque), ios::beg);
    cout << "Posicion actual (bytes): " << archivo.tellg() << endl;

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

void barridoBinario() {
    ifstream archivo("alumnos.dat", ios::binary);
    if (!archivo.is_open()) {
        cout << "No hay datos binarios\n";
        return;
    }

    nodoEstudiante bloque;

    cout << "\n--- BINARIO ---\n";

    while (archivo.read((char*)&bloque, sizeof(bloque))) {
        cout << bloque.clave << " - "
             << bloque.etiqueta << " - "
             << bloque.promedio << endl;
    }

    archivo.close();
}

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
