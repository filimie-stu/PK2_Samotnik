#pragma once

/**
 * \file Common.h
 * \brief Funkcjonalności współdzielone przez liczne klasy.
*/

/**
 * \enum fieldType
 * \brief Reprezentuje typ (zawartość pola na planszy)
*/
typedef enum fieldType
{
    REGULAR_TOKEN,  //!< standardowy pionek
    ACTIVE_TOKEN,   //!< pionek na aktywnym polu
    EMPTY,          //!< puste pole
    FORBIDDEN       //!< brak interaktywnego pola
} FieldType;

/**
 * \fn FieldType FieldType_fromChar(char c)
 * \brief Konwertuje pojedynczy znak do typu FieldType.
 * 
 * Obsługiwane znaki to:
 *  - 'o' - REGULAR_TOKEN
 *  - 'O' - ACTIVE_TOKEN
 *  - '#' - FORBIDDEN
 *  - '_' - EMPTY
 * 
 * Użycie nieobsługiwanego znaku powoduje wyświetlenie komunikatu błędu, i zwrot wartości EMPTY.
 * 
 * \param c Znak, na podstawie którego ustalony zostaje typ pola.
 * \return W przypadku pomyślnego rozpoznania znaku - odpowiedni typ pola; w przeciwnym razie - EMPTY.
*/
FieldType FieldType_fromChar(char c);

/**
 * \fn FieldType_toChar(FieldType ft)
 * \brief Konwertuje typ FieldType do pojedynczego znaku.
 * 
 * W zależności od użytego typu pola zwrócony zostanie następujący znak:
 *  - 'o' - REGULAR_TOKEN
 *  - 'O' - ACTIVE_TOKEN
 *  - ' ' - EMPTY
 *  - '#' - FORBIDDEN
 *  - '?' - zabezpieczenie na wypadek rozwijania programu  
 * 
 * \param ft Typ, który będzie konwertowany.
 * \return Znak odpowiadający podanej wartości FieldType.
*/
char FieldType_toChar(FieldType ft);

/**
 * \fn const char* FieldType_toString(FieldType ft)
 * \brief Konwertuje typ FieldType do łańcucha znaków.
 * 
 * W zależności od użytego typu pola zwrócony zostanie następujący łańcuch znaków:
 *  - "o" - REGULAR_TOKEN
 *  - "O" - ACTIVE_TOKEN
 *  - " " - EMPTY
 *  - "#" - FORBIDDEN
 *  - "?" - zabezpieczenie na wypadek rozwijania programu
 * 
 * \param ft Typ, który będzie konwertowany.
 * \return Łańcuch znaków odpowiadający podanej wartości FieldType.
*/
const char* FieldType_toString(FieldType ft);
