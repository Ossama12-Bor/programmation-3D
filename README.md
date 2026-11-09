==============================================
Utilisation du programme
==============================================

Toutes les parties du TP ont été implémentées.

Pour afficher les différents rendus, utilisez les touches suivantes :

1 : affiche la première partie de l’exercice 1 (scène des chaises).

2 : affiche le système solaire :

le soleil est fixe au centre de la scène,

la terre effectue une révolution autour du soleil et une rotation sur elle-même avec une inclinaison,

la lune effectue une révolution autour de la terre sur une orbite inclinée et une rotation sur elle-même.

3 : affiche le personnage lemming.off, incliné à l’aide d’une matrice de rotation alignant le vecteur vertical 
(0,1,0)
(0,1,0) avec le vecteur 
(1,1,1)
(1,1,1).

===========================
Réponse la question 3.3
===========================
On a :

    gl_Position=project_mat×view_mat×obj_mat×p


Pour faire tourner la caméra autour de l’objet, il est nécessaire d’agir dans les coordonnées réelles (repère monde), c’est-à-dire avant l’application de la matrice de projection. Une multiplication directe de view_mat par une matrice de rotation produirait uniquement une rotation locale de la caméra sur elle-même, ce qui ne correspond pas au comportement attendu.

Ainsi, la rotation doit être effectuée par rapport au repère monde. Pour cela, on applique successivement une translation afin de placer le point cible à l’origine, puis la rotation autour de l’axe souhaité, et enfin une translation inverse pour revenir à la position initiale. Cette combinaison de transformations permet de faire orbiter correctement la caméra autour de l’objet.