#include <stdio.h>
#include <ctype.h>
#include <string.h>

// ----------------------------
// Variables globales
// ----------------------------
char input[1000];
int pos = 0;

// ----------------------------
// Fonctions utilitaires
// ----------------------------

// Retourne le caractère courant
char courant() {
    return input[pos];
}

// Avancer dans la chaîne
void avancer() {
    pos++;
}

// Ignorer les espaces et tabulations
void ignorer_espaces() {
    while (courant() == ' ' || courant() == '\t') {
        avancer();
    }
}

// ----------------------------
// Déclarations des fonctions
// ----------------------------
int parse_polynome();
int parse_monome();
int parse_xpuissance();
int parse_nombre();
int parse_naturel();

// ----------------------------
// polynome → [ '-' ] monome { ('+' | '-') monome }
// ----------------------------
int parse_polynome() {

    ignorer_espaces();

    // signe optionnel
    if (courant() == '-') {
        avancer();
    }

    ignorer_espaces();

    // premier monôme obligatoire
    if (!parse_monome()) {
        return 0;
    }

    ignorer_espaces();

    // (+ ou -) monome répété
    while (courant() == '+' || courant() == '-') {
        avancer();

        ignorer_espaces();

        if (!parse_monome()) {
            return 0;
        }

        ignorer_espaces();
    }

    return 1;
}

// ----------------------------
// monome → nombre '*' xpuissance | xpuissance | nombre
// ----------------------------
int parse_monome() {

    ignorer_espaces();

    int save = pos;

    // Cas : nombre ...
    if (parse_nombre()) {

        ignorer_espaces();

        // nombre * xpuissance
        if (courant() == '*') {
            avancer();

            ignorer_espaces();

            if (parse_xpuissance()) {
                return 1;
            } else {
                return 0;
            }
        }

        // juste nombre
        return 1;
    }

    // retour arrière
    pos = save;

    ignorer_espaces();

    // Cas : xpuissance seule
    if (parse_xpuissance()) {
        return 1;
    }

    return 0;
}

// ----------------------------
// xpuissance → 'X' | 'X' '^' naturel
// ----------------------------
int parse_xpuissance() {

    ignorer_espaces();

    if (courant() == 'X') {
        avancer();

        ignorer_espaces();

        // puissance optionnelle
        if (courant() == '^') {
            avancer();

            ignorer_espaces();

            if (!parse_naturel()) {
                return 0;
            }
        }

        return 1;
    }

    return 0;
}

// ----------------------------
// nombre → naturel [ '.' { chiffre } ]
// ----------------------------
int parse_nombre() {

    ignorer_espaces();

    if (!parse_naturel()) {
        return 0;
    }

    ignorer_espaces();

    // partie décimale optionnelle
    if (courant() == '.') {
        avancer();

        ignorer_espaces();

        // chiffres après le point (optionnels ici)
        while (isdigit(courant())) {
            avancer();
        }
    }

    return 1;
}

// ----------------------------
// naturel → chiffre { chiffre }
// ----------------------------
int parse_naturel() {

    ignorer_espaces();

    if (!isdigit(courant())) {
        return 0;
    }

    while (isdigit(courant())) {
        avancer();
    }

    return 1;
}
