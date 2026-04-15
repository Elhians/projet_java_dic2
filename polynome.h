/**
 * ============================================================================
 * polynome.h
 * ============================================================================
 * Définit les structures de données et les prototypes des fonctions
 * pour la manipulation des polynômes.
 * 
 * Ce fichier contient :
 *   - Les structures monome et polynome
 *   - Les variables globales (input, pos)
 *   - Les prototypes de toutes les fonctions d'analyse et de gestion
 * ============================================================================
 */

#ifndef POLYNOME_H
#define POLYNOME_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* ==================== STRUCTURES ==================== */

/**
 * monome : représente un terme du polynôme.
 * 
 * Un monôme est de la forme : coefficient * X^exposant
 * - coefficient : nombre réel (peut être décimal ou négatif)
 * - exposant    : entier naturel (>= 0)
 */
typedef struct monome {
    double       coefficient;
    unsigned int exposant;
} monome;

/**
 * polynome : liste chaînée de monômes.
 * 
 * Chaque maillon contient :
 *   - un monôme (coefficient + exposant)
 *   - un pointeur vers le maillon suivant
 * 
 * Le polynôme nul est représenté par la liste vide (NULL).
 */
typedef struct polynome {
    monome              m;
    struct polynome*    suivant;
} polynome;

/* ==================== VARIABLES GLOBALES ==================== */

/**
 * input : chaîne de caractères contenant le polynôme à analyser.
 * pos   : position courante dans la chaîne.
 */
extern char input[1000];
extern int pos;

/* ==================== PROTOTYPES DES FONCTIONS ==================== */

/* Fonctions utilitaires */
char courant(void);
void avancer(void);
void ignorer_espaces(void);
void erreur(const char* msg);
int estChiffre(char c);

/* Analyseurs */
int parse_polynome(void);
int parse_monome(void);
int parse_xpuissance(void);
int parse_nombre(void);
int parse_naturel(void);

/* Gestion de la liste (Tâche 2) */
polynome* creerMaillon(double coeff, unsigned int exp);
void ajouterEnFin(polynome** tete, double coeff, unsigned int exp);
void ajouterMonome(double coeff, unsigned int exp);

/* Gestion de la liste triée (Tâche 4) */
void insererTriee(polynome** tete, double coeff, unsigned int exp);

/* Construction du polynôme */
void construirePolynome(void);
polynome* getPolynome(void);
void resetPolynome(void);

#endif // POLYNOME_H