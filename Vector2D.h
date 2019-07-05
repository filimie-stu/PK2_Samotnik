#pragma once

/**
 * \brief Struktura reprezentująca dwuwymiarowy wektor o współrzędnych całkowitych.
 * 
 * Pomimo swojej nazwy kojarzącej się z geometryczymi przesunięciami, 'Vector2D' wykorzystywany
 * jest wszędzie tam, gdzie zachodzi potrzeba grupowania liczb całkowitych w parę - najczęściej w charakterze
 * współrzędnych oraz rozmiarów dwuwymiarowych tablic. 
*/
typedef struct vector_2d 
{
    int x; 
    int y;
} Vector2D;

/** \memberof vector_2d */
Vector2D Vector2D_create(int x, int y);

/** 
 * \memberof vector_2d 
 * 
 * \brief Wyznacza środek odcinka.
 *  
 * Oblicza współrzędne punktu stanowiącego geometryczny środek odcinka pomiędzy dwoma innymi punktami.
 * Z uwagi na całkowity charakter współrzędnych, wynik jest zaokrąglony w dół.
 * 
 * \param first współrzędne początka odcinka.
 * \param second współrzędne końca odcinka.
 * \return wektor współrzędnych środka odcinka pomiędzy dwoma punktami. Wartości niecałkowite zostają zaokrąglone w dół. 
 * */
Vector2D Vector2D_calculateMidpoint(Vector2D first, Vector2D second);

/** 
 * \memberof vector_2d 
 * \brief Porównuje ze sobą dwa wektory. 
 * 
 * \return 1 jeżeli odpowiednie współrzędne wektorów są sobie równe, w przeciwnym razie - 0. 
 * */
int Vector2D_equals(Vector2D a, Vector2D b);