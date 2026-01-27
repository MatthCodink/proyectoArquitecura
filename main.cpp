#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <sstream>

using namespace std;

/* ================= ESTRUCTURAS ================= */

vector<string> codigo;
set<string> variables;
int contadorLabel = 0;

/* ================= UTILIDADES ================= */

string nuevaEtiqueta() {
    return "L" + to_string(++contadorLabel);
}

string limpiar(string s) {
    while (!s.empty() && isspace(s.front()))
        s.erase(0, 1);
    while (!s.empty() && isspace(s.back()))
        s.pop_back();
    return s;
}

/* ================= PROCESADOR ================= */

void procesarLinea(
    const vector<string>& tokens,
    vector<string>& pilaLabels
) {
    // LEER A
    if (tokens.size() == 2 && tokens[0] == "LEER") {
        string var = tokens[1];
        variables.insert(var);
        codigo.push_back("INP");
        codigo.push_back("STA " + var);
    }

    // IMPRIMIR A
    else if (tokens.size() == 2 && tokens[0] == "IMPRIMIR") {
        string var = tokens[1];
        codigo.push_back("LDA " + var);
        codigo.push_back("OUT");
    }

    // C = A - B
    else if (tokens.size() == 5 && tokens[1] == "=" && tokens[3] == "-") {
        string destino = tokens[0];
        string A = tokens[2];
        string B = tokens[4];

        variables.insert(destino);
        variables.insert(A);
        variables.insert(B);

        codigo.push_back("LDA " + A);
        codigo.push_back("SUB " + B);
        codigo.push_back("STA " + destino);
    }

    // SI A > B ENTONCES
    else if (tokens.size() == 5 && tokens[0] == "SI" && tokens[2] == ">" && tokens[4] == "ENTONCES") {
        string A = tokens[1];
        string B = tokens[3];

        variables.insert(A);
        variables.insert(B);

        string labelTrue = nuevaEtiqueta();
        string labelEnd = nuevaEtiqueta();

        pilaLabels.push_back(labelEnd);
        pilaLabels.push_back(labelTrue);

        codigo.push_back("LDA " + A);
        codigo.push_back("SUB " + B);
        codigo.push_back("BRP " + labelTrue);
    }

    // SINO
    else if (tokens.size() == 1 && tokens[0] == "SINO") {
        string labelTrue = pilaLabels.back(); pilaLabels.pop_back();
        string labelEnd = pilaLabels.back();

        codigo.push_back("BRA " + labelEnd);
        codigo.push_back(labelTrue + ":");
    }

    // FIN
    else if (tokens.size() == 1 && tokens[0] == "FIN") {
        string labelEnd = pilaLabels.back();
        pilaLabels.pop_back();

        codigo.push_back(labelEnd + ":");
    }
}

/* ================= MAIN ================= */

int main() {
    ifstream entrada("entrada.txt");
    ofstream salida("salida.asm");

    vector<string> pilaLabels;

    if (!entrada.is_open()) {
        cout << "❌ Error al abrir entrada.txt\n";
        return 1;
    }

    string linea;
    while (getline(entrada, linea)) {
        linea = limpiar(linea);
        if (linea.empty()) continue;

        stringstream ss(linea);
        vector<string> tokens;
        string tok;

        while (ss >> tok)
            tokens.push_back(tok);

        procesarLinea(tokens, pilaLabels);
    }

    // Salida LMC
    for (const string& c : codigo)
        salida << c << endl;

    salida << "HLT" << endl;

    for (const string& v : variables)
        salida << v << " DAT 0" << endl;

    entrada.close();
    salida.close();

    cout << "Transpilación completada\n";
    return 0;
}
