/**
 * ============================================================================
 * TACHE 4 : CODAGE PAR DEGRE DECROISSANT
 * ============================================================================
 * Objectif : Modifier le programme de la question 2 de sorte qu'un polynôme
 *           soit représenté par une liste chaînée rangée par ordre décroissant
 *           des degrés de ses monômes.
 *
 * Compilation : javac polynome/Tache4.java
 * Utilisation : java polynome.Tache4 "polynome"
 *
 * Exemple : java polynome.Tache4 "X^2 + X^5 + 3*X^2"
 * ============================================================================
 */
package polynome;

public class Tache4 {
    
    // Variables globales
    private static String input;
    private static int pos;
    
    // Tête de la liste chaînée (polynôme nul = null)
    private static Polynome teteListe = null;
    
    // ==================== FONCTIONS D'ANALYSE ====================
    
    private static void erreur(String msg) {
        System.out.println("ERREUR DE SYNTAXE : " + msg);
        System.exit(1);
    }
    
    private static void ignorerEspaces() {
        while (pos < input.length() && (input.charAt(pos) == ' ' || input.charAt(pos) == '\t')) {
            pos++;
        }
    }
    
    private static char courant() {
        if (pos >= input.length()) return '\0';
        return input.charAt(pos);
    }
    
    private static void avancer() {
        if (pos < input.length()) pos++;
    }
    
    private static boolean estChiffre(char c) {
        return c >= '0' && c <= '9';
    }
    
    // ==================== ANALYSEURS AVEC EXTRACTION ====================
    
    private static int lireNaturel() {
        if (!estChiffre(courant())) erreur("Naturel attendu");
        int val = 0;
        while (estChiffre(courant())) {
            val = val * 10 + (courant() - '0');
            avancer();
        }
        return val;
    }
    
    private static double lireNombre() {
        int partieEntiere = lireNaturel();
        double val = (double) partieEntiere;
        if (courant() == '.') {
            avancer();
            double frac = 0.0;
            double div = 10.0;
            while (estChiffre(courant())) {
                frac += (courant() - '0') / div;
                div *= 10.0;
                avancer();
            }
            val += frac;
        }
        return val;
    }
    
    private static int lireXpuissance() {
        if (courant() != 'X') erreur("'X' attendu");
        avancer();
        if (courant() == '^') {
            avancer();
            return lireNaturel();
        }
        return 1;
    }
    
    private static Monome lireMonome(int signe) {
        ignorerEspaces();
        double coeff;
        int exp;
        
        if (estChiffre(courant())) {
            coeff = lireNombre();
            if (courant() == '*') {
                avancer();
                exp = lireXpuissance();
            } else {
                exp = 0;
            }
        } else if (courant() == 'X') {
            coeff = 1.0;
            exp = lireXpuissance();
        } else {
            erreur("Monome invalide");
            return null;
        }
        
        coeff *= signe;
        return new Monome(coeff, exp);
    }
    
    // ==================== GESTION DE LA LISTE TRIEE ====================
    
    private static Polynome creerMaillon(double coeff, int exp) {
        return new Polynome(new Monome(coeff, exp));
    }
    
    /**
     * Insère un monôme dans la liste triée par exposant décroissant
     * 
     * Algorithme :
     *   1. Si coefficient nul -> ignorer
     *   2. Parcourir la liste pour trouver la position (exposant décroissant)
     *   3. Si même exposant trouvé -> additionner les coefficients
     *   4. Si somme nulle -> supprimer le maillon
     *   5. Sinon -> insérer nouveau maillon à la bonne position
     */
    private static void insererTriee(double coeff, int exp) {
        if (coeff == 0.0) {
            return;
        }
        
        Polynome courant = teteListe;
        Polynome precedent = null;
        
        // Recherche de la position (exposant décroissant)
        while (courant != null && courant.getMonome().getExposant() > exp) {
            precedent = courant;
            courant = courant.getSuivant();
        }
        
        // Même exposant trouvé -> fusion
        if (courant != null && courant.getMonome().getExposant() == exp) {
            double nouveauCoeff = courant.getMonome().getCoefficient() + coeff;
            if (nouveauCoeff == 0.0) {
                // Supprimer le maillon
                if (precedent == null) {
                    teteListe = courant.getSuivant();
                } else {
                    precedent.setSuivant(courant.getSuivant());
                }
            } else {
                courant.getMonome().setCoefficient(nouveauCoeff);
            }
            return;
        }
        
        // Insertion d'un nouveau maillon
        Polynome nouveau = creerMaillon(coeff, exp);
        if (precedent == null) {
            nouveau.setSuivant(teteListe);
            teteListe = nouveau;
        } else {
            nouveau.setSuivant(courant);
            precedent.setSuivant(nouveau);
        }
    }
    
    // ==================== ANALYSEUR PRINCIPAL ====================
    
    /**
     * parsePolynome : analyse le polynôme complet et construit la liste triée
     * polynome → [ '-' ] monome { ( '+' | '-' ) monome }
     */
    private static void parsePolynome() {
        int signe = 1;
        
        ignorerEspaces();
        
        // Signe optionnel devant le premier monôme (seul '-' est autorisé)
        if (courant() == '-') {
            signe = -1;
            avancer();
        }
        
        ignorerEspaces();
        
        // Premier monôme
        Monome m = lireMonome(signe);
        insererTriee(m.getCoefficient(), m.getExposant());
        
        ignorerEspaces();
        
        // Monômes suivants
        while (courant() == '+' || courant() == '-') {
            if (courant() == '+') {
                avancer();
                m = lireMonome(1);
            } else {
                avancer();
                m = lireMonome(-1);
            }
            insererTriee(m.getCoefficient(), m.getExposant());
            ignorerEspaces();
        }
        
        // Vérifier qu'on a tout consommé
        if (courant() != '\0') {
            erreur("Caractere inattendu en fin de chaine");
        }
    }
    
    // ==================== MAIN ====================
    
    public static void main(String[] args) {
        if (args.length != 1) {
            System.out.println("Usage: java polynome.Tache4 \"polynome\"");
            System.out.println("Exemple: java polynome.Tache4 \"X^2 + X^5 + 3*X^2\"");
            System.exit(1);
        }
        
        input = args[1];
        pos = 0;
        teteListe = null;
        
        parsePolynome();
        
        // Pas d'affichage - conforme à la tâche 4
        // Le programme se termine silencieusement si tout est correct
    }
}