// question 3 et 6
// Noeud de la liste chainee representant un monome
class Noeud {
    double coefficient;
    int    exposant;
    Noeud  suivant;

    Noeud(double c, int e) {
        this.coefficient = c;
        this.exposant    = e;
        this.suivant     = null;
    }
}

public class Fonctions {

    // ============================================================
    //                        AFFICHAGE
    // ============================================================

    // etape 1 : afficher un seul monome
    static void afficherMonome(double c, int e) {
        if (e == 0) {
            // affichera directement le nombre
            System.out.printf("%s", formatCoeff(c));
        } else if (e == 1) {
            if (c == 1.0)       System.out.print("X");   // affichera X
            else if (c == -1.0) System.out.print("-X");  // affichera -X
            else                System.out.printf("%s*X", formatCoeff(c)); // affichera nombre*X
        } else {
            if (c == 1.0)       System.out.printf("X^%d", e);            // affichera X^nombre
            else if (c == -1.0) System.out.printf("-X^%d", e);           // affichera -X^nombre
            else                System.out.printf("%s*X^%d", formatCoeff(c), e); // affichera nombre*X^nombre
        }
    }

    // etape 2 : afficher le signe entre deux monomes consecutifs
    static void afficherSigne(double c) {
        if (c >= 0.0) System.out.print(" + ");
        else          System.out.print(" - ");
    }

    // etape 3 : afficher le polynome complet
    static void afficher(Noeud p) {
        if (p == null) {
            System.out.println("0");
            return;
        }

        afficherMonome(p.coefficient, p.exposant);
        p = p.suivant;

        while (p != null) {
            double c = p.coefficient;
            int    e = p.exposant;
            afficherSigne(c);
            afficherMonome((c < 0.0) ? -c : c, e);
            p = p.suivant;
        }

        System.out.println();
    }

    // helper : formater un double sans .0 inutile
    static String formatCoeff(double c) {
        if (c == (long) c) return String.valueOf((long) c);
        return String.valueOf(c);
    }

    // ============================================================
    //                        ADDITION
    // ============================================================

    static Noeud plus(Noeud a, Noeud b) {
        Noeud res   = null;
        Noeud queue = null;

        while (a != null && b != null) {

            Noeud n = null;

            if (a.exposant > b.exposant) {
                // le nouveau noeud cree va prendre les valeurs du premier noeud du polynome a
                n = new Noeud(a.coefficient, a.exposant);
                a = a.suivant;

            } else if (a.exposant < b.exposant) {
                // le nouveau noeud cree va prendre les valeurs du premier noeud du polynome b
                n = new Noeud(b.coefficient, b.exposant);
                b = b.suivant;

            } else {
                // meme exposant : additionner les coefficients
                double somme = a.coefficient + b.coefficient;
                if (somme == 0.0) {
                    // les termes s'annulent, on jette le noeud
                    a = a.suivant;
                    b = b.suivant;
                    continue;
                }
                // la somme des coef des monomes de meme degre va etre stockee dans n
                n = new Noeud(somme, a.exposant);
                // l'exposant de n sera le meme que les exposants des monomes qui ont permis de faire la somme
                a = a.suivant;
                b = b.suivant;
            }

            if (res == null) res = n;
            else queue.suivant = n; // va relier le nouveau noeud a la liste
            queue = n; // se place sur le dernier noeud
        }

// 4*X^3 + 2*X^2 + 3*X + 1 +
//5*X^4 + 6*X^3 + 7*X^2 =>B
//5*X^4 + 10 * X^3  +9 * X^2
        // recopier les monomes restants
        // cette boucle s'execute quand une des deux listes est vide mais l'autre a encore des monomes
        Noeud reste = (a != null) ? a : b; // on choisit la liste qui n'est pas encore vide

        while (reste != null) {

            Noeud n = new Noeud(reste.coefficient, reste.exposant); // on cree un nouveau noeud

            // on copie le monome (coefficient + exposant) tel quel
            // ce noeud est le dernier pour l'instant, rien apres lui

            if (res == null) res = n;          // si le resultat est vide ce noeud devient la tete
            else queue.suivant = n;            // sinon on l'accroche apres le dernier noeud deja insere

            queue = n; // queue avance sur ce nouveau dernier noeud
            reste = reste.suivant;
        }

        return res;
    }

    // ============================================================
    //                       SOUSTRACTION
    // ============================================================

    static Noeud moins(Noeud a, Noeud b) {
        Noeud res   = null;
        Noeud queue = null;

        while (a != null && b != null) {

            Noeud n;

            if (a.exposant > b.exposant) {
                // le monome de a a le plus grand degre, on le prend tel quel
                n = new Noeud(a.coefficient, a.exposant);
                a = a.suivant;

            } else if (a.exposant < b.exposant) {
                // le monome de b a le plus grand degre
                // on inverse son coefficient car on soustrait b
                n = new Noeud(-b.coefficient, b.exposant);
                b = b.suivant;

            } else {
                // meme exposant : on soustrait les coefficients
                double diff = a.coefficient - b.coefficient;
                if (diff == 0.0) {
                    // les termes s'annulent, on jette le noeud
                    a = a.suivant;
                    b = b.suivant;
                    continue;
                }
                n = new Noeud(diff, a.exposant);
                a = a.suivant;
                b = b.suivant;
            }

            if (res == null) res = n;
            else queue.suivant = n;
            queue = n;
        }

        // recopier les monomes restants de a tels quels
        while (a != null) {
            Noeud n = new Noeud(a.coefficient, a.exposant);
            if (res == null) res            = n;
            else             queue.suivant  = n;
            queue = n;
            a = a.suivant;
        }

/*
a= x^2 + 2*x
b=3*x^3 + 4*x + 7
a - b = -3*x^3  + 4*x^2 - 2*x -7
*/
        // recopier les monomes restants de b en inversant leur coefficient
        while (b != null) {
            Noeud n = new Noeud(-b.coefficient, b.exposant);
            if (res == null) res           = n;
            else             queue.suivant = n;
            queue = n;
            b = b.suivant;
        }

        return res;
    }

    // ============================================================
    //                      MULTIPLICATION
    // ============================================================

    /*
      MULTIPLICATION

      Pour chaque monome de a :
        1. on multiplie tous les monomes de b par ce monome
           → on obtient une petite liste intermediaire
        2. on additionne cette liste au resultat avec plus()

      Ex : (2*X^2 + 3) * (X + 1)
        tour 1 : 2*X^2 * (X + 1) = 2*X^3 + 2*X^2   → res = 2*X^3 + 2*X^2
        tour 2 : 3     * (X + 1) = 3*X   + 3        → res = 2*X^3 + 2*X^2 + 3*X + 3
    */

    static Noeud fois(Noeud a, Noeud b) {

        Noeud res = null;

        for (Noeud i = a; i != null; i = i.suivant) { // on parcourt chaque monome de a

            // construire la liste intermediaire : monome i * chaque monome de b
            Noeud inter = null;
            Noeud queue = null;

            for (Noeud j = b; j != null; j = j.suivant) {

                // coefficient = produit des deux coefficients
                // exposant    = somme des deux exposants
                Noeud n = new Noeud(i.coefficient * j.coefficient, i.exposant + j.exposant);

                // on accroche ce noeud en queue de la liste intermediaire
                if (inter == null) inter = n;
                else queue.suivant = n;
                queue = n;
            }

            // on additionne la liste intermediaire au resultat
            res = plus(res, inter); // l'ancien resultat est remplace par le nouveau
        }

        return res;
    }

    // ============================================================
    //                        DIVISION
    // ============================================================

    /*
      DIVISION EUCLIDIENNE  —  quotient(a, b, reste[])

      On cherche q et r tels que :  a = b * q + r
      avec deg(r) < deg(b)

      Algorithme :
      tant que le degre de rem >= degre de b :
        1. prendre le terme dominant de rem
        2. le diviser par le terme dominant de b  → prochain terme du quotient
        3. soustraire (ce terme * b) de rem

      rem (remainder) = le reste courant, commence comme copie de a
      et retrecit a chaque tour jusqu'a ce que son degre soit < deg(b)
    */

    // reste[0] contiendra le reste apres l'appel (equivalent du double pointeur en C)
    static Noeud quotient(Noeud a, Noeud b, Noeud[] reste) {

        Noeud q     = null;
        Noeud queue = null;

        // copie de a dans rem pour ne pas modifier a
        Noeud rem  = null;
        Noeud qrem = null;
        for (Noeud t = a; t != null; t = t.suivant) {
            Noeud n = new Noeud(t.coefficient, t.exposant);
            if (rem == null) rem = n;
            else qrem.suivant = n;
            qrem = n;
        }

        while (rem != null && rem.exposant >= b.exposant) {

            // etape 1 : prochain terme du quotient
            double c = rem.coefficient / b.coefficient;
            int    e = rem.exposant    - b.exposant;

            // etape 2 : ajouter ce terme au quotient
            Noeud terme = new Noeud(c, e);
            if (q == null) q = terme;
            else queue.suivant = terme;
            queue = terme;

            // etape 3 : rem = rem - (terme * b)
            Noeud mono    = new Noeud(c, e);
            Noeud produit = fois(mono, b);
            rem           = moins(rem, produit);
        }

        // rem est maintenant le reste final, on le renvoie via le tableau
        reste[0] = rem;

        return q;
    }

}
