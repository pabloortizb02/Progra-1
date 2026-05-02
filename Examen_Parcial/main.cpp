#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// --- CONFIGURACIÓN DE CONEXIÓN ---
const string servidor = "tcp://127.0.0.1:3306";
const string usuario = "root";
const string password = "tu_password"; // <--- Cambia esto por tu contraseña de MySQL
const string base_datos = "universidad";

sql::Connection* conectar() {
    try {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection* con = driver->connect(servidor, usuario, password);
        con->setSchema(base_datos);
        return con;
    } catch (sql::SQLException &e) {
        cout << "ERROR DE CONEXIÓN: " << e.what() << endl;
        return nullptr;
    }
}

// --- MÓDULO: ALUMNOS ---
void listarAlumnos() {
    sql::Connection* con = conectar();
    if (!con) return;

    try {
        // Uso de JOIN para el Listado Integrado
        sql::Statement* stmt = con->createStatement();
        sql::ResultSet* res = stmt->executeQuery(
            "SELECT a.id, a.nombres, a.apellidos, a.carnet, c.nombre_curso, s.nombre_seccion "
            "FROM alumnos a "
            "LEFT JOIN alumnos_cursos ac ON a.id = ac.alumno_id "
            "LEFT JOIN cursos c ON ac.curso_id = c.id "
            "LEFT JOIN alumnos_seccion asoc ON a.id = asoc.alumno_id "
            "LEFT JOIN secciones s ON asoc.seccion_id = s.id"
        );

        cout << "\n--- LISTADO INTEGRADO DE ALUMNOS ---" << endl;
        cout << "ID | Nombres | Apellidos | Carnet | Curso | Seccion" << endl;
        while (res->next()) {
            cout << res->getInt("id") << " | " 
                 << res->getString("nombres") << " | " 
                 << res->getString("apellidos") << " | " 
                 << res->getString("carnet") << " | " 
                 << (res->getString("nombre_curso") == "" ? "N/A" : res->getString("nombre_curso")) << " | "
                 << (res->getString("nombre_seccion") == "" ? "N/A" : res->getString("nombre_seccion")) << endl;
        }
        delete res;
        delete stmt;
        con->close();
    } catch (sql::SQLException &e) {
        cout << "ERROR: " << e.what() << endl;
    }
    delete con;
}

void insertarAlumno() {
    string nom, ape, car;
    cout << "Nombres: "; cin >> nom;
    cout << "Apellidos: "; cin >> ape;
    cout << "Carnet: "; cin >> car;

    sql::Connection* con = conectar();
    try {
        sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO alumnos(nombres, apellidos, carnet) VALUES(?,?,?)");
        pstmt->setString(1, nom);
        pstmt->setString(2, ape);
        pstmt->setString(3, car);
        pstmt->executeUpdate();
        cout << "Alumno grabado con éxito." << endl;
        delete pstmt;
    } catch (sql::SQLException &e) {
        cout << "ERROR AL GRABAR: " << e.what() << endl;
    }
    delete con;
}

// --- MÓDULO: ASIGNACIÓN (REGLA 6.4) ---
void asignarCurso() {
    int id_alumno, id_curso;
    cout << "ID Alumno: "; cin >> id_alumno;
    cout << "ID Curso: "; cin >> id_curso;

    sql::Connection* con = conectar();
    try {
        sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO alumnos_cursos(alumno_id, curso_id) VALUES(?,?)");
        pstmt->setInt(1, id_alumno);
        pstmt->setInt(2, id_curso);
        pstmt->executeUpdate();
        cout << "Asignación exitosa." << endl;
        delete pstmt;
    } catch (sql::SQLException &e) {
        // Captura el error de duplicado o llave foránea
        cout << "\nERROR DE BASE DE DATOS: " << e.what() << endl;
    }
    delete con;
}

// --- MENÚS ---
void menuAlumnos() {
    int op;
    do {
        cout << "\n--- MODULO ALUMNOS ---\n1. Listar\n2. Grabar\n5. Regresar\nOpcion: ";
        cin >> op;
        if (op == 1) listarAlumnos();
        if (op == 2) insertarAlumno();
    } while (op != 5);
}

int main() {
    int opcion;
    do {
        cout << "\n======================================" << endl;
        cout << "        SISTEMA UNIVERSIDAD" << endl;
        cout << "======================================" << endl;
        cout << "1. Alumnos\n2. Cursos\n3. Secciones\n4. Alumnos Cursos\n6. Salir\nOpcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: menuAlumnos(); break;
            case 4: asignarCurso(); break;
            case 6: cout << "Saliendo..." << endl; break;
            default: cout << "Opción válida para demo." << endl;
        }
    } while (opcion != 6);

    return 0;
}