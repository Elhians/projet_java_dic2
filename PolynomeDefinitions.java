/**
 * ============================================================================
 * PolynomeDefinitions.java
 * ============================================================================
 * Définit les structures de données pour la manipulation des polynômes.
 * Équivalent du fichier polynome.h en C.
 * 
 * Ce fichier contient :
 *   - Les classes Monome et Polynome
 *   - Les structures de base pour la représentation des polynômes
 * ============================================================================
 */
package polynome;

/**
 * Monome : représente un terme du polynôme.
 * Un monôme est de la forme : coefficient * X^exposant
 * - coefficient : nombre réel (peut être décimal ou négatif)
 * - exposant    : entier naturel (>= 0)
 */
class Monome {
    double coefficient;
    int exposant;
    
    /**
     * Constructeur
     * @param coefficient Coefficient du monôme
     * @param exposant    Exposant du monôme (>= 0)
     */
    Monome(double coefficient, int exposant) {
        this.coefficient = coefficient;
        this.exposant = exposant;
    }
}

/**
 * Polynome : liste chaînée de monômes.
 * Chaque maillon contient :
 *   - un monôme (coefficient + exposant)
 *   - un pointeur vers le maillon suivant
 * 
 * Le polynôme nul est représenté par la liste vide (null).
 */
class Polynome {
    Monome m;
    Polynome suivant;
    
    /**
     * Constructeur d'un maillon
     * @param m       Monôme à stocker
     * @param suivant Maillon suivant
     */
    Polynome(Monome m, Polynome suivant) {
        this.m = m;
        this.suivant = suivant;
    }
    
    /**
     * Constructeur d'un maillon seul (fin de liste)
     * @param m Monôme à stocker
     */
    Polynome(Monome m) {
        this(m, null);
    }
}